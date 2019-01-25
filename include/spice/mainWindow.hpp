#ifndef SPICE_MAINWINDOW_H
#define SPICE_MAINWINDOW_H

#include <spice/HD_Wallet.hpp>
#include <spice/startup.hpp>

// Separate file needed for this class
class AddressesPanel : private ItemsList<NCursesMenuItem>, public NCursesMenu
{
  public:
    AddressesPanel(std::shared_ptr<HD_Wallet> wallet);
};

#endif
