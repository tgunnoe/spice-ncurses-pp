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
        new BackItem(prev),
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
        return true;
    else
        return false;
}

bool ImportWalletItem::action()
{
    p_form_->getMenu()->prev_menu()->hide();
    //p_submenu->prev_menu()->hide();
    p_form_->getMenu()->boldframe("Wallet mnemonic");

    p_form_->box();
    p_form_->getMenu()->post();
    (*p_form_)();
    return true;
}
bool BackItem::action()
{
    p_prev_->top();
    p_prev_->show();
    return true;
}
bool AcceptItem::action()
{
    // Fix this crazy spaghetti. almost looks like lisp..
    p_submenu->prev_menu()->getWalletPtr() =
            std::make_shared<HD_Wallet>(
                bc::split( p_submenu->getTempWallet().displayMnemonic()) );

    return true;
}
////////////////////////////////////////////////////////
// Menus ///////////////////////////////////////////////
////////////////////////////////////////////////////////
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

AcceptMenu::AcceptMenu(std::shared_ptr<StartupMenu> prev) :
    SubMenu<StartupMenu>(6, 30, (lines()/2)-1, (cols()/2)-15, prev)
{
    V_.reserve(3U);
    V_ = {
        new PassiveItem("Accept"),
        new BackItem(prev),
        new NCursesMenuItem()
    };
    set_format(1, 2);
    options_off(O_SHOWDESC);
    InitMenu(&V_[0], true, false);

    set_mark(">");
}
ImportWalletForm::ImportWalletForm(int nlines, int ncols,
                     std::shared_ptr<StartupMenu> prev)
    : NCursesForm( nlines, ncols, (lines()-5)/4,
                   (cols()/2)-32 ),
      p_menu_(std::make_shared<AcceptMenu>(prev))
{
    //is having this local going to bite me?
    Alpha_Field* aft = new Alpha_Field(5);

    V_.reserve(13U);

    for (auto i = 0, x = 1, y = 1; i != 12; ++i, y += 10)
    {
        if( i == 6 ) { x = 3; y = 1; }
        V_.push_back( new NCursesFormField(1, 8, x, y) );
    }
    V_.push_back( new NCursesFormField() );

    InitForm(&V_[0], true, false);

    for(auto beg = V_.begin(); beg != V_.end(); ++beg)
        (*beg)->set_fieldtype(*aft);

}
int ImportWalletForm::virtualize(int c)
{
    switch (c) {
    case '\t':
        return REQ_NEXT_FIELD;
    case 127:
        //driver(REQ_PREV_CHAR);
        return REQ_DEL_PREV;
    case '\n':
        if (current_field() == *(V_.end()-2))
        {
            p_menu_->top();
            p_menu_->drive(REQ_FIRST_ITEM);
        }
        //p_menu_->driver(REQ_FIRST_ITEM);
        return MAX_COMMAND + 1;
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
