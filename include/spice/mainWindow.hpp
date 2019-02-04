#ifndef SPICE_MAINWINDOW_H
#define SPICE_MAINWINDOW_H

#include <spice/HD_Wallet.hpp>
#include <spice/startup.hpp>
#include <cursesm.h>

template<class T> class ItemsList2
{
protected:
    std::vector<T *> V_;
    ~ItemsList2() {
        using it = typename std::vector<T *>::const_iterator;
        for (it i = V_.begin(); i != V_.end(); ++i) {
            delete *i;
        }
    }
};

// Separate file needed for this class
class AddressesPanel : private ItemsList2<NCursesMenuItem>, public NCursesMenu
{
  public:
    AddressesPanel(std::shared_ptr<HD_Wallet> wallet);
};

#endif
