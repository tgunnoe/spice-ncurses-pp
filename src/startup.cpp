#include <spice/startup.hpp>

////////////////////////////////////////////////////////
// Menu Items //////////////////////////////////////////
////////////////////////////////////////////////////////
bool NewWalletItem::action()
{
    prev_.hide();
    menu_();
    // if(p_submenu->prev_menu()->getWalletPtr() != nullptr)
    //     return true;
    // else
    return false;
}

bool ImportWalletItem::action()
{
    //p_form_->getMenu().prev_menu()->hide();
    //p_submenu->prev_menu()->hide();

    prev_.hide();
    form_.box();

    form_();
    return true;
}
bool BackItem::action()
{
    prev_.top();
    prev_.show();
    return true;
}
bool AcceptItem::action()
{
    // Fix this crazy spaghetti. almost looks like lisp..
    //spice::Application::getSingleton()->getWallet() =
    //    std::make_shared<HD_Wallet>(
    //    bc::split( menu_.getTempWallet().displayMnemonic()) );
    return true;
}
////////////////////////////////////////////////////////
// Menus ///////////////////////////////////////////////
////////////////////////////////////////////////////////
StartupMenu::StartupMenu()
        : NCursesMenu (6, 25, (lines()-5)/2, (cols()-23)/2)
{
    V_.reserve(4U);
    V_ = {
        new NewWalletItem("Generate Wallet", *this),
        new ImportWalletItem("Import Wallet", *this),
        new QuitItem(),
        new NCursesMenuItem()
    };

    InitMenu(&V_[0], true, false);

    boldframe("Start Wizard");
    set_mark(">");
}
NewWalletMenu::NewWalletMenu(StartupMenu& prev) :
    NCursesMenu(6, 25, (lines()-5)/2, (cols()-23)/2),
    prev_(prev)
{
    V_.reserve(4U);
    V_ = {
        new AcceptItem("Keep Wallet", *this),
        new PassiveItem("Regenerate"),
        new BackItem(prev),
        new NCursesMenuItem()
    };

    InitMenu(&V_[0], true, false);

    boldframe("Keep?");
    set_mark(">");
}
AcceptMenu::AcceptMenu(StartupMenu& prev) :
    NCursesMenu(6, 20, (lines()/2)-1, (cols()/2)-15),
    prev_(prev)
{
    V_.reserve(3U);
    V_ = {
        new PassiveItem("Accept"),
        new PassiveItem("Back"),
        //new BackItem(prev),
        new NCursesMenuItem()
    };
    //set_format(1, 2);
    //options_off(O_SHOWDESC);
    InitMenu(&V_[0], true, false);

    set_mark(">");
}
ImportWalletForm::ImportWalletForm(StartupMenu& prev)
    : NCursesForm( 7, 65, (lines()-5)/4, (cols()/2)-32 ),
      menu_(prev)
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
    //box();

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
        //if (current_field() == *(V_.end()-2))
        //{
        menu_.top();
        menu_.drive(REQ_FIRST_ITEM);
        //}
        //p_menu_->driver(REQ_FIRST_ITEM);
        return MAX_COMMAND + 3;
    // Pass the rest of the keys to predefined handler.
    default: return NCursesForm::virtualize(c);
    }
}
void ImportWalletForm::On_Form_Init()
{
    menu_.post();
    menu_.boldframe("Wallet mnemonic");
}
void NewWalletMenu::On_Menu_Init()
{
    p_panel = std::make_shared<NCursesPanel>(3, cols()-10, 5, 5);
    p_panel->box();
    tryWallet_ = randomGen();
    p_panel->centertext(1, tryWallet_.displayMnemonic().c_str());
}
void NewWalletMenu::On_Menu_Termination()
{
    p_panel->hide();
}
