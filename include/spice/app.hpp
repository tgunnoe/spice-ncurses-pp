#ifndef SPICE_APP_H
#define SPICE_APP_H

#include <memory>
#include <iostream>
#include <cursesapp.h>
#include <spice/startup.hpp>
#include <spice/mainWindow.hpp>
#include <spice/HD_Wallet.hpp>

namespace spice {

class Application : public NCursesApplication
{
  private:
    //static std::shared_ptr<Application> m_app;
    std::shared_ptr<HD_Wallet> p_wallet_;
  protected:
    int titlesize() const { return 1; }
    void title();

  public:
    Application() : NCursesApplication(FALSE) {}
    //static std::shared_ptr<Application> getSingleton();
    std::shared_ptr<HD_Wallet> getWallet() {
        return p_wallet_;
    }
    int run();
};

} // namespace spice

#endif
