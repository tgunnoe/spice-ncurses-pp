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

////////////////////////////////////////////////////////
// Menus ///////////////////////////////////////////////
////////////////////////////////////////////////////////
class StartupMenu : private ItemsList<NCursesMenuItem>,
                    public NCursesMenu
{
  private:
    //wallet parameter is nullptr so here p_wallet_
    // needs to reference the ptr
    std::shared_ptr<HD_Wallet>& p_wallet_;
  public:
    StartupMenu(std::shared_ptr<HD_Wallet>& wallet);
    std::shared_ptr<HD_Wallet>& getWallet() {
        return p_wallet_;
    }
};

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
    StartupMenu& prev_menu() const {
        return prev_;
    }

};

class FormPanel;

class AcceptMenu : private ItemsList<NCursesMenuItem>,
                   public NCursesMenu
{
  private:
  public:
    AcceptMenu();
};
class ImportWalletForm : private ItemsList<NCursesFormField>,
                         public NCursesForm
{
  private:
  public:
    ImportWalletForm();
    void On_Form_Init();
    virtual int virtualize(int c);
};
class FormPanel : public NCursesPanel
{
  private:
    ImportWalletForm form_;
    AcceptMenu menu_;
    //StartupMenu& prev_;
    bool isOnMenu = false;
  public:
    FormPanel();
    AcceptMenu getMenu() {
        return menu_;
    }
    ImportWalletForm getForm() {
        return form_;
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
    StartupMenu& prev_;
  public:
    ImportWalletItem(const char *s, StartupMenu& prev)
        : NCursesMenuItem(s), prev_(prev){}
    bool action();
};
class AcceptItem : public NCursesMenuItem
{
  private:
    NewWalletMenu& menu_;
  public:
    AcceptItem(const char* s, NewWalletMenu& prev)
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
