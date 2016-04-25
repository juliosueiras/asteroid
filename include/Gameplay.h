#ifndef GAMEPLAY_H_
#define GAMEPLAY_H_

#include "GameState.h"
#include "System.h"
#include "Player.h"
#include "Missile.h"
#include "Asteroid.h"
#include "Explosion.h"
#include "Random.h"

#include <vector>

class Gameplay : public GameState
{
    Texture*                mShuttleTex;
    Texture*                mShotTex;
    Texture*                mAsteroidTex;
    Texture*                mBackgroundTex;
    Texture*                mExplosionTex;

    Player*				    mPlayer;

    std::vector<Missile*>   mMissiles;
    std::vector<Asteroid*>  mAsteroids;
    std::vector<Explosion*>  mExplosions;

    bool                    mIsActive;

public:
                            Gameplay(Game* game);
                            ~Gameplay() override;

    bool                    Initialize() override;
    void                    Shutdown() override;

    void                    Update(float dt) override;
    void                    Draw(SDL_Renderer* renderer) override;

    void		            OnKeyDown(const SDL_KeyboardEvent& kbe) override;

    float                   WorldLeft() const       { return 0.0f; }
    float                   WorldRight() const      { return (float)System::GetWindowWidth(); }
    float                   WorldTop() const        { return 0.0f; }
    float                   WorldBottom() const     { return (float)System::GetWindowHeight(); }

    void                    LoadLevel();
    void                    ClearLevel();

    bool                    IsActive() const        { return mIsActive; }
};

#endif
