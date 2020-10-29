#include "include/screen.hpp"
// #include "include/gnuplot-iostream.h"

using namespace std;

int main()
{
    Screen screen;

    if (screen.init())
    {
        if (screen.loadmedia())
        {
            screen.update();
        }
        else
        {
            cout << "Unable to load media" << endl;
            screen.quit = true;
        }

        while (!screen.quit)
        {
            screen.eventManager();
        }
    }
    else
    {
        cout << "Unable to load" << endl;
    }

    screen.close();

#ifdef _WIN32
    std::cout << "Press enter to exit." << std::endl;
    std::cin.get();
#endif

    return 0;
}
