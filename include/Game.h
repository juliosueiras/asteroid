#ifndef GAME_H_
#define GAME_H_

#include <vector>

#include "System.h"
#include "Gameplay.h"
#include "MainMenu.h"

class Game
{
    Gameplay*               mGameplayState;
    MainMenu*               mMainMenuState;

    GameState*              mCurrentState;

public:
                            Game();
                            ~Game();

    bool				    Initialize();
    void				    Shutdown();

    void				    Update(float dt);
    void				    Draw(SDL_Renderer* renderer);
    
    void				    OnWindowResized(int w, int h);
    void				    OnKeyDown(const SDL_KeyboardEvent& kbe);
    void				    OnKeyUp(const SDL_KeyboardEvent& kbe);
    void				    OnMouseDown(const SDL_MouseButtonEvent& mbe);
    void				    OnMouseUp(const SDL_MouseButtonEvent& mbe);
    void				    OnMouseMotion(const SDL_MouseMotionEvent& mme);

    Gameplay*               GetGameplayState() const    { return mGameplayState; }
    MainMenu*               GetMainMenuState() const    { return mMainMenuState; }

    void                    EnterMainMenu();
    void                    EnterGameplay();
};

#endif
