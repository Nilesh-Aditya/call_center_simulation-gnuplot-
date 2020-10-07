#include "include/screen.hpp"
// #include "include/gnuplot-iostream.h"

using namespace std;

int main()
{
    // Gnuplot gp;
    // vector<float> v1 = {0.4, 1.2, 1.6, 2.1, 2.4, 3.1, 3.3, 3.8, 5.6, 7.8};
    // vector<float> v2 = {1.6, 1.9, 2.65, 3.9, 4.5, 4.9, 6.8, 7.9, 8.2, 9.0};
    // map<float, float> mp;

    // for (int i = 0; i < v1.size(); i++)
    // {
    //     mp[v1[i]] = v2[i];
    // }

    // gp << "set xrange [0:9]\nset yrange [0:9]\n";
    // gp << "plot '-' with lines title 'v'\n";
    // gp.send(mp);

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
    // For Windows, prompt for a keystroke before the Gnuplot object goes out of scope so that
    // the gnuplot window doesn't get closed.
    std::cout << "Press enter to exit." << std::endl;
    std::cin.get();
#endif

    return 0;
}