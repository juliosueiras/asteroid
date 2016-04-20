#include <iostream>

#include "Game.h"
#include "Random.h"

Game::Game()
    : mGameplayState(NULL)
    , mMainMenuState(NULL)
    , mCurrentState(NULL)
{
    // nothing to do: all setup should be done in Initialize method
}

Game::~Game()
{
    // nothing to do: all cleanup should be done in Shutdown method
}

bool Game::Initialize()
{
    std::cout << "Initializing game" << std::endl;

    // set some window properties
    System::SetWindowSize(640, 640);
    System::SetWindowTitle("Space Shootah!");

    // initialize random seed
    InitRandom();

    //
    // create all game states
    //
    mGameplayState = new Gameplay(this);
    if (!mGameplayState->Initialize()) {
        return false;
    }

    mMainMenuState = new MainMenu(this);
    if (!mMainMenuState->Initialize()) {
        return false;
    }

    // set initial state
    mCurrentState = mMainMenuState;

	return true;
}

void Game::Shutdown()
{
    std::cout << "Shutting down game" << std::endl;

    mGameplayState->Shutdown();
    mMainMenuState->Shutdown();

    delete mGameplayState;
    delete mMainMenuState;
}

void Game::Draw(SDL_Renderer* renderer)
{
    mCurrentState->Draw(renderer);
}

void Game::Update(float dt)
{
    mCurrentState->Update(dt);
}

void Game::OnKeyDown(const SDL_KeyboardEvent& kbe)
{
    mCurrentState->OnKeyDown(kbe);
}

void Game::OnKeyUp(const SDL_KeyboardEvent& kbe)
{
    mCurrentState->OnKeyUp(kbe);
}

void Game::OnMouseDown(const SDL_MouseButtonEvent& mbe)
{
    mCurrentState->OnMouseDown(mbe);
}

void Game::OnMouseUp(const SDL_MouseButtonEvent& mbe)
{
    mCurrentState->OnMouseUp(mbe);
}

void Game::OnMouseMotion(const SDL_MouseMotionEvent& mme)
{
    mCurrentState->OnMouseMotion(mme);
}

void Game::OnWindowResized(int w, int h)
{
    //std::cout << "Window resized to " << w << 'x' << h << std::endl;

    mCurrentState->OnWindowResized(w, h);
}

void Game::EnterMainMenu()
{
    mCurrentState = mMainMenuState;
}

void Game::EnterGameplay()
{
    mCurrentState = mGameplayState;
}
