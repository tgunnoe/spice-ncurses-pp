#include <spice/mainWindow.hpp>

AddressesPanel::AddressesPanel(std::shared_ptr<HD_Wallet> wallet)
        : NCursesMenu (lines(), cols()/2, 0, 0)
{
    V_.reserve(lines());

    for( int i = 0; i != lines(); ++i ) {
        //whelp std::string to const char* was a goddamn learning
        //experience, c_str doesnt copy
        std::string addr = wallet->displayAddress(i);
        char *g = new char[addr.size() + 1];
        std::copy(begin(addr), end(addr), g);
        g[addr.size()] = '\0';
        V_.push_back(new PassiveItem(g));
    }
    set_format(lines()-2, 1);
    V_.push_back(new NCursesMenuItem());

    InitMenu(&V_[0], true, false);
    box();
}
