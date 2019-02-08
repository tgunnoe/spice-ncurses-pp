#ifndef SPICE_STARTUP_H
#define SPICE_STARTUP_H

#include <spice/app.hpp>
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
class NewWalletItem;
class ImportWalletItem;
////////////////////////////////////////////////////////
// Menus ///////////////////////////////////////////////
////////////////////////////////////////////////////////
class StartupMenu : private ItemsList<NCursesMenuItem>,
                    public NCursesMenu
{
  private:
    //std::shared_ptr<StartupMenu> p_menu;
  public:
    StartupMenu();
};

// I derived this class because I wanted to make
// derivatives of On_Menu_{Init, Termination}
// which would not be specific to a submenu template
class NewWalletMenu : private ItemsList<NCursesMenuItem>,
                      public NCursesMenu
{
  private:
    std::shared_ptr<NCursesPanel> p_panel;
    StartupMenu& prev_;
    HD_Wallet tryWallet_;
  public:
    NewWalletMenu(StartupMenu& prev);
    void On_Menu_Init();
    void On_Menu_Termination();
    HD_Wallet getTempWallet() const{
        return tryWallet_;
    }

};
class AcceptMenu : private ItemsList<NCursesMenuItem>,
                   public NCursesMenu
{
  private:
    StartupMenu& prev_;
  public:
    AcceptMenu(StartupMenu& prev);
    inline int drive(int c) {
        return driver(c);
    }
    bool action();
};
class ImportWalletForm : private ItemsList<NCursesFormField>,
                         public NCursesForm
{
  private:
    AcceptMenu menu_;
    bool isOnButton = false;
  public:
    ImportWalletForm(StartupMenu& prev);
    void On_Form_Init();

    virtual int virtualize(int c);

    AcceptMenu getMenu() {
        return menu_;
    }
};


////////////////////////////////////////////////////////
// Menu Items //////////////////////////////////////////
////////////////////////////////////////////////////////
class NewWalletItem : public NCursesMenuItem
{
  private:
    NewWalletMenu menu_;
    StartupMenu& prev_;
  public:
    NewWalletItem(const char* s, StartupMenu& prev)
        : NCursesMenuItem(s), menu_(prev), prev_(prev) {}
    bool action();
};
class ImportWalletItem : public NCursesMenuItem
{
  private:
    ImportWalletForm form_;
    StartupMenu& prev_;
  public:
    ImportWalletItem(const char *s, StartupMenu& prev)
        : NCursesMenuItem(s), form_(prev), prev_(prev){}
    bool action();
};
class AcceptItem : public NCursesMenuItem
{
  private:
    const NewWalletMenu& menu_;
  public:
    AcceptItem(const char* s, const NewWalletMenu& prev)
        : NCursesMenuItem(s), menu_(prev) {}
    bool action();

};
class BackItem : public NCursesMenuItem
{
  private:
    StartupMenu& prev_;
  public:
    BackItem(StartupMenu& prev)
        : NCursesMenuItem("Back"), prev_(prev) {}
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
