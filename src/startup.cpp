#include <spice/startup.hpp>

////////////////////////////////////////////////////////
// Menu Items //////////////////////////////////////////
////////////////////////////////////////////////////////
bool NewWalletItem::action()
{
    prev_.hide();
    menu_.boldframe("Keep?");
    menu_();
    if(prev_.getWallet() != nullptr)
        return true;
    else
        return false;
}

bool ImportWalletItem::action()
{
    //p_form_->getMenu().prev_menu()->hide();
    //p_submenu->prev_menu()->hide();
    FormPanel form_menu;

    prev_.hide();
    prev_.bottom();

    form_menu.box();
    form_menu.show();
    form_menu.top();

    //form_menu_.getForm().setSubWindow(form_menu_);
    //form_menu.getMenu().setSubWindow(form_menu.getForm().w);
    form_menu.getMenu();
    form_menu.getForm()();

    return false;
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

    menu_.prev_menu().getWallet() =
        std::make_shared<HD_Wallet>(
            bc::split( menu_.getTempWallet().displayMnemonic()) );
    return true;
}
////////////////////////////////////////////////////////
// Menus ///////////////////////////////////////////////
////////////////////////////////////////////////////////
StartupMenu::StartupMenu(std::shared_ptr<HD_Wallet>& wallet)
    : NCursesMenu (6, 25, (lines()-5)/2, (cols()-23)/2),
      p_wallet_(wallet)
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
    set_mark(">");
}
FormPanel::FormPanel() :
    NCursesPanel(lines()-2, cols()-5, 1, 2),
    form_(),
    menu_()
{}
AcceptMenu::AcceptMenu() :
    NCursesMenu(6, 10, (lines())-10, (cols()/2)-5)
{
    V_.reserve(3U);
    V_ = {
        new PassiveItem("Accept"),
        new QuitItem(),
        //new BackItem(prev),
        new NCursesMenuItem()
    };
    //set_format(1, 2);
    //options_off(O_SHOWDESC);
    InitMenu(&V_[0], true, false);

    //setSubWindow(win);

    set_mark(">");
}
ImportWalletForm::ImportWalletForm()
    : NCursesForm( 8, cols()-15, 3, 7 )
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

    box();

    InitForm(&V_[0], true, false);

    //setSubWindow(win);

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
    case 'T':
    case '\n':
        //if (current_field() == *(V_.end()-2))
        //{
        //driver(REQ_PREV_FIELD);
        //driver(REQ_NEXT_FIELD);
        //bottom();

        //drive(REQ_FIRST_ITEM);
        //menu_.set_foreground(A_REVERSE);
        //}
        //p_menu_->driver(REQ_FIRST_ITEM);
        return MAX_COMMAND + 3;
    // Pass the rest of the keys to predefined handler.
    default: return NCursesForm::virtualize(c);
    }
}
void ImportWalletForm::On_Form_Init()
{
    //menu_.post();
    //menu_.boldframe("Wallet mnemonic");
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
