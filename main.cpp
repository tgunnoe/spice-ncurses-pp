#include <spice/app.hpp>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");

    int res;

    try {
        spice::Application spice;
        spice.handleArgs(argc, argv);
        //res = spice.getSingleton()->run();
        res = spice();
        endwin();
    } catch (const NCursesException *e) {
        endwin();
        std::cerr << e->message << std::endl;
        res = e->errorno;
    } catch (const NCursesException &e) {
        endwin();
        std::cerr << e.message << std::endl;
        res = e.errorno;
    } catch (const std::exception &e) {
        endwin();
        std::cerr << "Exception: " << e.what() << std::endl;
        res = EXIT_FAILURE;
    }
    return res;
}
