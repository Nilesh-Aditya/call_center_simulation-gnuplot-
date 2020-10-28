#pragma once
#ifdef _WIN32
#include "SDL.h"

#else
#include "SDL2/SDL.h"
#endif

#include <array>
#include <iostream>
#include <vector>
#include <utility>

class Screen
{
public:
    const static unsigned SCREEN_HEIGHT = 633;
    const static unsigned SCREEN_WIDTH = 1063;
    bool quit = false;

    Screen();
    bool init();
    bool loadmedia();
    void close();
    void update();
    void eventManager();
    std::array<int, 2> mousePress(SDL_MouseButtonEvent &);
    std::vector<std::pair<float, float>> read_file(bool data_set1 = true);
    void graph_making(int &, int &, bool);
    void make_table(std::vector<float> arrival, std::vector<float> departure);
    // void graph_select(int &, int &, bool);

private:
    SDL_Surface *gScreenSurface;
    SDL_Surface *gCurrentSurface;
    SDL_Window *m_window;
    // SDL_Event e;
};
