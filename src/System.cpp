/**
 * One line to give the program's name and a brief description.
 * Copyright © 2016 yourname
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>

#include "System.h"
#include "Timer.h"
#include "Game.h"

namespace
{
    SDL_Window*     g_Window                = NULL;
    int             g_WindowWidth           = 0;
    int             g_WindowHeight          = 0;

    SDL_Renderer*   g_Renderer              = NULL;
    const Uint8*    g_KeyStates             = NULL;

    float           g_Time                  = 0.0f;
    float           g_TimeSinceLastFrame    = 0.0f;

    Uint64          g_FrameNo               = 0;

    bool            g_ShouldQuit            = false;

    bool            g_IsPaused              = false;

    Timer           g_Timer;
}

namespace System
{
    SDL_Window* GetWindow()
    {
        return g_Window;
    }

    int GetWindowWidth()
    {
        return g_WindowWidth;
    }

    int GetWindowHeight()
    {
        return g_WindowHeight;
    }

    void SetWindowSize(int w, int h)
    {
        if (g_Window) {
            SDL_SetWindowSize(g_Window, w, h);
            g_WindowWidth = w;
            g_WindowHeight = h;
        }
    }

    void SetWindowTitle(const char* title)
    {
        if (g_Window) {
            SDL_SetWindowTitle(g_Window, title);
        }
    }

    SDL_Renderer* GetRenderer()
    {
        return g_Renderer;
    }

    const Uint8* GetKeyStates()
    {
        return g_KeyStates;
    }

    float GetTime()
    {
        return g_Time;
    }

    float GetTimeSinceLastFrame()
    {
        return g_TimeSinceLastFrame;
    }

    Uint64 GetFrameNumber()
    {
        return g_FrameNo;
    }

    void Quit()
    {
        g_ShouldQuit = true;
    }

    void Pause()
    {
        if (!g_IsPaused) {
            g_Timer.Pause();
            g_IsPaused = true;
        }
    }

    void Unpause()
    {
        if (g_IsPaused) {
            g_Timer.Unpause();
            g_IsPaused = false;
        }
    }

    bool IsPaused()
    {
        return g_IsPaused;
    }
}


int main(int argc, char** argv)
{
    std::cout << "Hello" << std::endl;

    //
    // initialize SDL
    //
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "*** Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    //
    // initialize SDL_image library
    //
    if (!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)) {
        std::cerr << "*** Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    //
    // create a window
    //
    g_WindowWidth = 800;
    g_WindowHeight = 600;
    g_Window = SDL_CreateWindow("Hello, SDL2!",
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                g_WindowWidth, g_WindowHeight,
                                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!g_Window) {
        std::cerr << "*** Failed to create window: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    //
    // get a pointer to keyboard state managed by SDL
    //
    g_KeyStates = SDL_GetKeyboardState(NULL);

    //
    // create a renderer that takes care of drawing stuff to the window
    //
    g_Renderer = SDL_CreateRenderer(g_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!g_Renderer) {
        std::cerr << "*** Failed to create renderer: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    //
    // texture scaling mode
    //
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    //
    // create game
    //

    Game game;

    if (!game.Initialize()) {
        std::cerr << "*** Game initialization failed" << std::endl;
        return EXIT_FAILURE;
    }

    //
    // main loop
    //

    // start tracking total game time
    g_Timer.Start();

    // start tracking frame time
    Timer frameTimer;
    frameTimer.Start();

    while (!g_ShouldQuit) {

        ++g_FrameNo;

        //
        // draw current frame
        //

        game.Draw(g_Renderer);

        SDL_RenderPresent(g_Renderer);

        //
        // process events
        //

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                g_ShouldQuit = true;
                break;
            case SDL_KEYDOWN:
                game.OnKeyDown(event.key);
                break;
            case SDL_KEYUP:
                game.OnKeyUp(event.key);
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    g_WindowWidth = event.window.data1;
                    g_WindowHeight = event.window.data2;
                    game.OnWindowResized(g_WindowWidth, g_WindowHeight);
                }
                break;
            default:
                break;
            }
        }

        //
        // time tracking
        //

        // update total game time
        g_Time = g_Timer.GetTime();

        // get time elapsed since last frame
        g_TimeSinceLastFrame = frameTimer.GetTime();

        // reset frame timer
        frameTimer.Start();

        //
        // game update
        //

        if (!g_IsPaused) {
            game.Update(g_TimeSinceLastFrame);
        }
    }

    //
    // cleanup
    //

    // shut down the game
    game.Shutdown();

    // shut down the image library
    IMG_Quit();

    // shut down SDL (closes window)
    SDL_Quit();

    std::cout << "Goodbye" << std::endl;

    return 0;
}
