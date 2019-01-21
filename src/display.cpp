#include <spice/display.hpp>

MenuItems::~MenuItems()
{
    typedef std::vector<NCursesMenuItem *>::const_iterator it;
    for (it i = V_.begin(); i != V_.end(); ++i) {
        delete *i;
    }
}
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
ImportWalletAction::ImportWalletAction(const char* s,
                                       const std::shared_ptr<StartupMenu> prev)
    : NCursesMenuItem(s),
      p_submenu(std::make_shared<ImportWalletForm>(6, 80))
{
    //is having this local going to bite me?
    Alpha_Field* aft = new Alpha_Field(5);

    F = new NCursesFormField*[13];
    F[0]  = new NCursesFormField(1, 8, 1, 1);
    F[1]  = new NCursesFormField(1, 8, 1, 11);
    F[2]  = new NCursesFormField(1, 8, 1, 21);
    F[3]  = new NCursesFormField(1, 8, 1, 31);
    F[4]  = new NCursesFormField(1, 8, 1, 41);
    F[5]  = new NCursesFormField(1, 8, 1, 51);
    F[6]  = new NCursesFormField(1, 8, 3, 1);
    F[7]  = new NCursesFormField(1, 8, 3, 11);
    F[8]  = new NCursesFormField(1, 8, 3, 21);
    F[9]  = new NCursesFormField(1, 8, 3, 31);
    F[10]  = new NCursesFormField(1, 8, 3, 41);
    F[11]  = new NCursesFormField(1, 8, 3, 51);
    F[12]  = new NCursesFormField();

    p_submenu->init_form(F, TRUE, TRUE);
    p_submenu->box();
    //p_submenu->mvwin( (p_submenu->lines()-5)/2,
    //                  (p_submenu->cols()-2) );
    for(int i = 0; i != 12; i++)
        F[i]->set_fieldtype(*aft);
}
bool ImportWalletAction::action()
{
    //p_submenu->prev_menu()->hide();

    (*p_submenu)();
    //ImportWalletForm F;
    //F();
    return TRUE;
}

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
          p_menu(this),
          p_wallet(wallet)
{
    V_.reserve(4U);
    V_ = {
        new NewWalletItem("Generate Wallet", p_menu),
        new ImportWalletAction("Import Wallet", p_menu),
        new QuitItem(),
        new NCursesMenuItem()
    };

    InitMenu(&V_[0], true, false);

    boldframe("Start Wizard");
    set_mark(">");
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
