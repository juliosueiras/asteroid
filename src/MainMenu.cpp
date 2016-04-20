#include "MainMenu.h"
#include "Gameplay.h"
#include "Texture.h"
#include "Game.h"

#include <iostream>

MainMenu::MainMenu(Game* game)
    : GameState(game)
    , mBtnNewTex(NULL)
    , mBtnResumeTex(NULL)
    , mBtnExitTex(NULL)
    , mBtnNew(NULL)
    , mBtnResume(NULL)
    , mBtnExit(NULL)
{
}

MainMenu::~MainMenu()
{
}

bool MainMenu::Initialize()
{
    SDL_Renderer* renderer = System::GetRenderer();

    mBtnNewTex = Texture::Load("media/button_new.png", renderer);
    mBtnResumeTex = Texture::Load("media/button_resume.png", renderer);
    mBtnExitTex = Texture::Load("media/button_exit.png", renderer);

    mBtnNew = new Button(mBtnNewTex);
    mBtnResume = new Button(mBtnResumeTex);
    mBtnExit = new Button(mBtnExitTex);

    int cx = System::GetWindowWidth() / 2;
    int cy = System::GetWindowHeight() / 2;

    mBtnResume->SetCenter(cx, cy - 100);
    mBtnNew->SetCenter(cx, cy);
    mBtnExit->SetCenter(cx, cy + 100);

    return true;
}

void MainMenu::Shutdown()
{
    delete mBtnNew;
    delete mBtnResume;
    delete mBtnExit;

    Texture::Destroy(mBtnNewTex);
    Texture::Destroy(mBtnResumeTex);
    Texture::Destroy(mBtnExitTex);
}

void MainMenu::Update(float dt)
{
}

void MainMenu::Draw(SDL_Renderer* renderer)
{
    if (mGame->GetGameplayState()->IsActive()) {

        mGame->GetGameplayState()->Draw(renderer);

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 192);
        SDL_RenderFillRect(renderer, NULL);

        mBtnResume->Draw(renderer);

    } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderClear(renderer);
    }

    mBtnNew->Draw(renderer);
    mBtnExit->Draw(renderer);
}

void MainMenu::OnKeyDown(const SDL_KeyboardEvent& kbe)
{
    switch (kbe.keysym.sym) {
    case SDLK_ESCAPE:
        mGame->EnterGameplay();
        break;
    }
}

void MainMenu::OnMouseDown(const SDL_MouseButtonEvent& mbe)
{
    if (mbe.button == SDL_BUTTON_LEFT) {

        if (mGame->GetGameplayState()->IsActive()) {
            if (mBtnResume->Contains(mbe.x, mbe.y)) {
                mGame->EnterGameplay();
            }
        }

        if (mBtnNew->Contains(mbe.x, mbe.y)) {
            mGame->GetGameplayState()->LoadLevel();     // reset everything
            mGame->EnterGameplay();
        }

        if (mBtnExit->Contains(mbe.x, mbe.y)) {
            System::Quit();
        }
    }
}
