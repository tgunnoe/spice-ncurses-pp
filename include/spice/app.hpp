#ifndef SPICE_APP_H
#define SPICE_APP_H

#include <memory>
#include <iostream>
#include <cursesapp.h>
#include <spice/display.hpp>
#include <spice/HD_Wallet.hpp>

namespace spice {

class Application : public NCursesApplication
{
  private:
    static std::shared_ptr<Application> m_app;
    std::shared_ptr<StartupMenu> m_menu;
    std::shared_ptr<HD_Wallet> wallet;
  protected:
    int titlesize() const { return 1; }
    void title();

  public:
    Application() : NCursesApplication(FALSE) {}
    int run();
};

} // namespace spice

#endif
