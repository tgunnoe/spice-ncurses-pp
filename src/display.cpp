#include <spice/display.hpp>

NewWalletItem::NewWalletItem(const char* s,
                             const std::shared_ptr<StartupMenu> prev)
        : NCursesMenuItem(s),
          p_submenu(std::make_shared<NewWalletMenu>(6, 25, prev))
{
    I = new NCursesMenuItem*[4];
    I[0] = new AcceptItem("Keep Wallet", p_submenu);
    I[1] = new PassiveItem("Regenerate");
    I[2] = new BackItem(p_submenu);
    I[3] = new NCursesMenuItem();

    p_submenu->init_menu(I, TRUE, TRUE);
    p_submenu->mvwin( (p_submenu->lines()-5)/2,
                      (p_submenu->cols()-23)/2 );
}
bool NewWalletItem::action()
{
    p_submenu->prev_menu()->hide();
    p_submenu->boldframe("Keep?");
    (*p_submenu)();

    // Back button wont work unless this is false,
    // need to fix
    if(p_submenu->prev_menu()->getWalletPtr() != nullptr)
        return TRUE;
    else
        return FALSE;

}
bool BackItem::action()
{
    p_submenu->prev_menu()->top();
    p_submenu->prev_menu()->show();
    return TRUE;
}
bool AcceptItem::action()
{
    // Fix this crazy spaghetti. almost looks like lisp..
    p_submenu->prev_menu()->getWalletPtr() =
            std::make_shared<HD_Wallet>(
                bc::split( p_submenu->getTempWallet().displayMnemonic()) );

    return TRUE;
}
void NewWalletMenu::On_Menu_Init()
{
    p_panel = std::make_shared<NCursesPanel>(3, cols()-10, 5, 5);
    p_panel->box();
    tryWallet = randomGen();
    p_panel->centertext(1, tryWallet.displayMnemonic().c_str());
}
void NewWalletMenu::On_Menu_Termination()
{
    p_panel->hide();
}
StartupMenu::StartupMenu(std::shared_ptr<HD_Wallet>& wallet)
        : NCursesMenu (6, 25, (lines()-5)/2, (cols()-23)/2),
          p_menu(this), p_wallet(wallet)
{

    I = new NCursesMenuItem*[4];
    I[0] = new NewWalletItem("Generate Wallet", p_menu);
    I[1] = new PassiveItem("Import Wallet");
    I[2] = new QuitItem();
    I[3] = new NCursesMenuItem();

    InitMenu(I, TRUE, TRUE);

    boldframe("Start Wizard");
}
AddressesPanel::AddressesPanel(std::shared_ptr<HD_Wallet> wallet)
        : NCursesMenu (lines(), cols()/2, 0, 0)
{

    I = new NCursesMenuItem*[lines()];
    for(int i = 0; i!=lines(); ++i) {
        //whelp std::string to const char* was a goddamn learning
        //experience, c_str doesnt copy
        std::string addr = wallet->displayAddress(i);
        char *g = new char[addr.size() + 1];
        std::copy(begin(addr), end(addr), g);
        g[addr.size()] = '\0';
        I[i] = new PassiveItem(g);
    }
    set_format(lines()-2, 1);

    I[lines()-1] = new NCursesMenuItem();

    InitMenu(I, TRUE, TRUE);
    box();
}
