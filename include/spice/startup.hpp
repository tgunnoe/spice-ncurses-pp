#ifndef SPICE_STARTUP_H
#define SPICE_STARTUP_H

#include <spice/HD_Wallet.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include "cursesm.h"
#include "cursesf.h"

////////////////////////////////////////////////////////
// Templates ///////////////////////////////////////////
////////////////////////////////////////////////////////
template<class T> class ItemsList
{
protected:
    std::vector<T *> V_;
    ~ItemsList() {
        using it = typename std::vector<T *>::const_iterator;
        for (it i = V_.begin(); i != V_.end(); ++i) {
            delete *i;
        }
    }
};

template<class T> class SubMenuItem : public NCursesUserItem<T>
{
  public:
    SubMenuItem (const char* p_name,
                  std::shared_ptr<T> curr_menu)
            : NCursesUserItem<T>(p_name,
                                 static_cast<const char*>(0),
                                 curr_menu) {}
    virtual ~SubMenuItem() {}
    bool action() {
        //NCursesUserItem<T>::UserData()->prev_menu()->unpost();
        return true;
    }
};

// General template for a submenu that takes a shared_ptr
// to the previous menu
template<class T> class SubMenu : public NCursesMenu
{
  protected:
    std::shared_ptr<T> p_back;
  public:
    //maybe make Items const..
    SubMenu (int nlines, int ncol, std::shared_ptr<T> prev_menu)
            : NCursesMenu(nlines, ncol), p_back(prev_menu) {}
    SubMenu (int nlines, int ncol, int start_x, int start_y,
             std::shared_ptr<T> prev_menu)
            : NCursesMenu(nlines, ncol, start_x, start_y),
              p_back(prev_menu) {}

    inline void init_menu(NCursesMenuItem* menu[],
                   bool with_frame,
                   bool autoDeleteItems)
    {
        InitMenu(menu, with_frame, autoDeleteItems);
    }
    std::shared_ptr<T> prev_menu()
    {
        return p_back;
    }
};

////////////////////////////////////////////////////////
// Menus ///////////////////////////////////////////////
////////////////////////////////////////////////////////
class StartupMenu : private ItemsList<NCursesMenuItem>, public NCursesMenu
{
  private:
    std::shared_ptr<StartupMenu> p_menu;
    std::shared_ptr<HD_Wallet>& p_wallet;
  public:
    StartupMenu(std::shared_ptr<HD_Wallet>& wallet);
    std::shared_ptr<HD_Wallet>& getWalletPtr()
    {
        return p_wallet;
    }
};

// I derived this class because I wanted to make
// derivatives of On_Menu_{Init, Termination}
// which would not be specific to a submenu template
class NewWalletMenu : public SubMenu<StartupMenu>
{
  private:
    std::shared_ptr<NCursesPanel> p_panel;
    HD_Wallet tryWallet;
  public:
    NewWalletMenu(int nlines, int ncols,
                  const std::shared_ptr<StartupMenu> prev)
            : SubMenu<StartupMenu>(nlines, ncols, prev){}
    void On_Menu_Init();
    void On_Menu_Termination();
    HD_Wallet getTempWallet()
    {
        return tryWallet;
    }

};
class AcceptMenu : private ItemsList<NCursesMenuItem>,
                   public SubMenu<StartupMenu>
{
  private:
    std::shared_ptr<StartupMenu> p_prev_;
  public:
    AcceptMenu(std::shared_ptr<StartupMenu> prev);
    inline int drive(int c) {
        return driver(c);
    }
    bool action();
};
class ImportWalletForm : private ItemsList<NCursesFormField>,
                         public NCursesForm
{
  private:
    std::shared_ptr<AcceptMenu> p_menu_;
    bool isOnButton = false;
  public:
    ImportWalletForm(int nlines, int ncols,
                     std::shared_ptr<StartupMenu> prev);

    virtual int virtualize(int c);

    std::shared_ptr<AcceptMenu> getMenu() {
        return p_menu_;
    }
};


////////////////////////////////////////////////////////
// Menu Items //////////////////////////////////////////
////////////////////////////////////////////////////////
class NewWalletItem : private ItemsList<NCursesMenuItem>,
                      public NCursesMenuItem
{
  private:
    std::shared_ptr<NewWalletMenu> p_submenu;
  public:
    NewWalletItem(const char* s,
                  const std::shared_ptr<StartupMenu> prev);
    bool action();

};
class ImportWalletItem : public NCursesMenuItem
{
  private:
    std::shared_ptr<ImportWalletForm> p_form_;
  public:
    ImportWalletItem(const char *s,
                     const std::shared_ptr<StartupMenu> prev)
        : NCursesMenuItem(s),
          p_form_(std::make_shared<ImportWalletForm>(7, 65, prev)){}
    bool action();
};
class AcceptItem : public NCursesMenuItem
{
  private:
    std::shared_ptr<NewWalletMenu> p_submenu;
  public:
    AcceptItem(const char* s,
               const std::shared_ptr<NewWalletMenu> prev)
            : NCursesMenuItem(s), p_submenu(prev) {}
    bool action();

};
class BackItem : public NCursesMenuItem
{
  private:
    const std::shared_ptr<StartupMenu> p_prev_;
  public:
    BackItem(const std::shared_ptr<StartupMenu>& prev)
        : NCursesMenuItem("Back"), p_prev_(prev) {}
    bool action();
};
class QuitItem : public NCursesMenuItem
{
  public:
    QuitItem(): NCursesMenuItem("Quit") { }
    bool action()
    {
        return true;
    }
};
class PassiveItem : public NCursesMenuItem
{
  public:
    PassiveItem(const char* text) : NCursesMenuItem(text) {
        options_off(O_SELECTABLE);
    }
};



#endif
