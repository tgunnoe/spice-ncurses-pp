#ifndef SPICE_DISPLAY_H
#define SPICE_DISPLAY_H

#include <spice/HD_Wallet.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include "cursesm.h"
#include "cursesf.h"

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
class QuitItem : public NCursesMenuItem
{
  public:
    QuitItem(): NCursesMenuItem("Quit") { }
    bool action()
    {
        return TRUE;
    }
};

class PassiveItem : public NCursesMenuItem
{
  public:
    PassiveItem(const char* text) : NCursesMenuItem(text) {
        options_off(O_SELECTABLE);
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
        return FALSE;
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

class BackItem : public NCursesMenuItem
{
  private:
    const std::shared_ptr<SubMenu<StartupMenu>> p_submenu;
  public:
    BackItem(const std::shared_ptr<SubMenu<StartupMenu>>& submenu)
            : NCursesMenuItem("Back"), p_submenu(submenu) {}
    bool action();
};

class NewWalletItem : private ItemsList<NCursesMenuItem>, public NCursesMenuItem
{
  private:
    std::shared_ptr<NewWalletMenu> p_submenu;
  public:
    NewWalletItem(const char* s,
                  const std::shared_ptr<StartupMenu> prev);
    bool action();

};

class ImportWalletForm : public NCursesForm
{
  private:
    NCursesFormField** F;
  public:
    ImportWalletForm(int nlines, int ncols)
        : NCursesForm( nlines, ncols, 2, 2 ){}
    virtual int virtualize(int c);
    inline void init_form(NCursesFormField* F[],
                          bool with_frame,
                          bool autoDeleteItems)
    {
        InitForm(F, with_frame, autoDeleteItems);
    }
};

class ImportWalletAction : private ItemsList<NCursesFormField>, public NCursesMenuItem
{
  private:
    std::shared_ptr<ImportWalletForm> p_submenu;
  public:
    ImportWalletAction(const char *s,
                       const std::shared_ptr<StartupMenu> prev);
    bool action();
};

class AcceptItem : public NCursesMenuItem
{
  private:
    NCursesMenuItem** I;
    std::shared_ptr<NewWalletMenu> p_submenu;
  public:
    AcceptItem(const char* s,
               const std::shared_ptr<NewWalletMenu> prev)
            : NCursesMenuItem(s), p_submenu(prev) {}
    bool action();

};

class AddressesPanel : private ItemsList<NCursesMenuItem>, public NCursesMenu
{
  public:
    AddressesPanel(std::shared_ptr<HD_Wallet> wallet);
};


#endif
