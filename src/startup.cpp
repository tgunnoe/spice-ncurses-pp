#include <spice/startup.hpp>

////////////////////////////////////////////////////////
// Menu Items //////////////////////////////////////////
////////////////////////////////////////////////////////
NewWalletItem::NewWalletItem(const char* s,
                             const std::shared_ptr<StartupMenu> prev)
        : NCursesMenuItem(s),
          p_submenu(std::make_shared<NewWalletMenu>(6, 25, prev))
{
    V_.reserve(4U);
    V_ = {
        new AcceptItem("Keep Wallet", p_submenu),
        new PassiveItem("Regenerate"),
        new BackItem(p_submenu),
        new NCursesMenuItem()
    };

    p_submenu->init_menu(&V_[0], true, false);
    p_submenu->mvwin( (p_submenu->lines()-5)/2,
                      (p_submenu->cols()-23)/2 );
}
bool NewWalletItem::action()
{
    p_submenu->prev_menu()->hide();
    p_submenu->boldframe("Keep?");
    p_submenu->set_mark(">");
    (*p_submenu)();

    if(p_submenu->prev_menu()->getWalletPtr() != nullptr)
        return TRUE;
    else
        return FALSE;
}
ImportWalletItem::ImportWalletItem(const char* s,
                                       const std::shared_ptr<StartupMenu> prev)
    : NCursesMenuItem(s),
      p_submenu(std::make_shared<ImportWalletForm>(6, 80))
{
    //is having this local going to bite me?
    Alpha_Field* aft = new Alpha_Field(5);

    V_.reserve(13U);

    for (auto i = 0, x = 1, y = 1; i != 12; ++i, y += 10)
    {
        if( i == 6 ) {
            x = 3;
            y = 1;
        }
        V_.push_back( new NCursesFormField(1, 8, x, y) );
    }
    V_.push_back( new NCursesFormField() );

    p_submenu->init_form(&V_[0], true, false);

    //p_submenu->mvwin( (p_submenu->lines()-5)/2,
    //                  (p_submenu->cols()-2) );
    for(auto beg = V_.begin(); beg != V_.end(); ++beg)
        (*beg)->set_fieldtype(*aft);
}
bool ImportWalletItem::action()
{
    //p_submenu->prev_menu()->hide();
    p_submenu->box();
    (*p_submenu)();

    return TRUE;
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
////////////////////////////////////////////////////////
// Menus ///////////////////////////////////////////////
////////////////////////////////////////////////////////
int ImportWalletForm::virtualize(int c)
{
    switch (c) {
    case '\t':
        return REQ_NEXT_FIELD;
    case 127:
        //driver(REQ_PREV_CHAR);
        return REQ_DEL_PREV;
    // Pass the rest of the keys to predefined handler.
    default: return NCursesForm::virtualize(c);
    }
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
          p_menu(this),
          p_wallet(wallet)
{
    V_.reserve(4U);
    V_ = {
        new NewWalletItem("Generate Wallet", p_menu),
        new ImportWalletItem("Import Wallet", p_menu),
        new QuitItem(),
        new NCursesMenuItem()
    };

    InitMenu(&V_[0], true, false);

    boldframe("Start Wizard");
    set_mark(">");
}

// Needs a separate file
