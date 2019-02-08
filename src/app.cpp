#include <spice/app.hpp>

namespace spice {

void Application::title()
{
    const char * const titleText = "SpiceWallet";
    const int len = ::strlen(titleText);

    titleWindow->bkgd(screen_titles());
    titleWindow->addstr(0, (titleWindow->cols() - len)/2, titleText);
    titleWindow->noutrefresh();
}
// std::shared_ptr<Application> Application::getSingleton()
// {
//     if (m_app == nullptr)
//         m_app = std::shared_ptr<Application>(new Application());
//     return m_app;
// }
int Application::run()
{
    // TODO: Insert here no wallet logic

    StartupMenu M(p_wallet_);
    M();

    if (p_wallet_ != nullptr) {
        AddressesPanel W(p_wallet_);
        W();
    }
    endwin();
    return 0;
}

} // namespace spice
