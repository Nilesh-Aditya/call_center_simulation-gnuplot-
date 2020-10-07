#include "screen.hpp"
#include <fstream>
#include "gnuplot-iostream.h"
#include <time.h>
#include <cmath>

// #include <array>

Screen::Screen() : gScreenSurface(NULL), gCurrentSurface(NULL), m_window(NULL) {}

bool Screen::init()
{
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Create window
        m_window = SDL_CreateWindow("CALL CENTER SIMULATION", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (m_window == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Initialize PNG loading
            gScreenSurface = SDL_GetWindowSurface(m_window);
        }
    }

    return success;
}

bool Screen::loadmedia()
{
    bool success = true;
    gCurrentSurface = SDL_LoadBMP("src/mns1.bmp");
    if (gCurrentSurface == NULL)
    {
        std::cout << "Unable to load image SDL_IMAGE :" << SDL_GetError() << std::endl;
        success = false;
    }

    return success;
}

void Screen::update()
{
    SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
    SDL_UpdateWindowSurface(m_window);
}

void Screen::close()
{
    //Free loaded image
    SDL_FreeSurface(gCurrentSurface);
    gCurrentSurface = NULL;

    //Destroy window
    SDL_DestroyWindow(m_window);
    m_window = NULL;
    SDL_Quit();
}

void Screen::eventManager()
{
    SDL_Event e;
    bool ds1;

    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            Screen::quit = true;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            auto [x, y] = mousePress(e.button);
            std::cout << x << "," << y << std::endl;
            if (x <= 480 && x >= 120 && y <= 230 && y >= 175)
            {
                std::cout << "DATA SET 1 selected" << std::endl;
                ds1 = true;
            }
            else if (x <= 490 && x >= 120 && y <= 418 && y >= 360)
            {
                std::cout << "DATA SET 2 selected" << std::endl;
                ds1 = false;
                // graph_making(x, y, ds1);
            }
            else if (x >= 235 && x <= 290 && y <= 600 && y >= 580)
            {
                std::cout << "Quiting....." << std::endl;
                Screen::quit = true;
            }
            else
            {
                graph_making(x, y, ds1);
            }
            // auto [x, y] = mousePress(e.button);
        }
    }
}

std::array<int, 2> Screen::mousePress(SDL_MouseButtonEvent &b)
{
    int x, y;
    if (b.button == SDL_BUTTON_LEFT)
    {
        SDL_GetMouseState(&x, &y);
    }
    return {x, y};
}

void Screen::graph_making(int &x, int &y, bool ds1)
{
    srand(time(NULL));
    int RANDOM = cos(rand()) + 1;
    Gnuplot gp;
    std::map<float, float> inQueue;
    std::vector<std::pair<float, float>> v = read_file(ds1);
    std::vector<float> arrival;
    std::vector<float> departure;
    bool M_idle = false;
    long dep = v[0].first;
    for (auto it = v.begin(); it != v.end(); it++)
    {
        arrival.push_back(it->first);
        if (dep <= it->first)
        {
            dep = it->first + it->second;
            departure.push_back(dep);
        }
        else
        {
            dep += it->second;
            departure.push_back(dep);
        }
    }
    int in_queue = 0, j = 0;
    for (size_t i = 0; i < arrival.size(); i++)
    {
        if (arrival[i + 1] < departure[j])
        {
            in_queue++;
            inQueue[arrival[i + 1]] = in_queue;
            inQueue[departure[j]] = in_queue;
            j++;
        }
        else
        {
            j++;
            // i = i - 1;
            if (in_queue > 0)
                in_queue--;
            inQueue[departure[j - 1]] = in_queue;
            inQueue[arrival[i]] = in_queue;
            inQueue[arrival[i + 1]] = in_queue;
        }
    }

    if (x <= 1005 && x >= 855)
    {
        if (y <= 278 && y >= 252)
        {
            std::cout << "GRAPH 1" << std::endl;
            gp << "set style fill transparent solid 0.7\n";
            gp << "set xrange [0:9]\nset yrange [0:9]\n";
            gp << "plot '-' with boxes title 'v'\n";
            gp.send(inQueue);
        }
        else if (y <= 344 && y >= 318)
        {
            std::cout << "GRAPH 2" << std::endl;
            gp << "set style fill transparent solid 0.7\n";
            gp << "set xrange [0:9]\nset yrange [0:9]\n";
            gp << "plot '-' with boxes title 'v'\n";
            for (auto &x : inQueue)
            {
                if (x.second > 0)
                    x.second = 1;
            }
            gp.send(inQueue);
        }
    }
}

std::vector<std::pair<float, float>> Screen::read_file(bool data_set1 /*= true*/)
{
    std::vector<std::pair<float, float>> v;
    if (data_set1)
    {
        std::ifstream ds("dataSets/ds1.csv");
        std::string line;
        getline(ds, line);
        // stringstream ss;
        // std::cout << line << std::endl;

        float col1, col2;
        char c;
        while (ds >> col1)
        {
            ds >> c;
            ds >> col2;
            v.push_back(std::make_pair(col1, col2));
            // std::cout << col1 << "," << col2 << std::endl;
        }
    }
    else
    {
        std::ifstream ds("dataSets/ds2.csv");
        std::string line;
        getline(ds, line);
        // stringstream ss;
        // std::cout << line << std::endl;
        float col1, col2;
        char c;
        while (ds >> col1)
        {
            ds >> c;
            ds >> col2;
            v.push_back(std::make_pair(col1, col2));
            // std::cout << col1 << "," << col2 << std::endl;
        }
    }

    return v;
}
