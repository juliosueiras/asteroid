#ifndef GAME_H_
#define GAME_H_

#include <SDL.h>

#include <vector>

#include "System.h"
#include "Player.h"
#include "Enemy.h"
#include "Missile.h"
#include "Explosion.h"
#include "Random.h"

class Game
{
    Texture*                mShuttleTex;
    Texture*                mEnemyTex;
    Texture*                mShotTex;
    Texture*                mShot2Tex;
    Texture*                mExplosionTex;

    Player*                 mPlayer;

    std::vector<Enemy*>     mEnemies;

    std::vector<Missile*>   mEnemyMissiles;

    std::vector<Missile*>   mMissiles;

    std::vector<Explosion*>   mExplosions;

    float                   mMinSpawnDelay;
    float                   mMaxSpawnDelay;

    float                   mNextSpawnTime;

public:
                            Game();
                            ~Game();

    bool                    Initialize();
    void                    Shutdown();

    /* Update method of the Game
     * @dt no ideal what is that
     *
     * Update every frame of the game(including refresh enemy, player, missle, and explosion)
     */
    void                    Update(float dt);

    void                    Reset();

    void                    Draw(SDL_Renderer* renderer);

    void                    OnWindowResized(int w, int h);
    void                    OnKeyDown(const SDL_KeyboardEvent& kbe);
    void                    OnKeyUp(const SDL_KeyboardEvent& kbe);

    float                   WorldLeft() const       { return 0.0f; }
    float                   WorldRight() const      { return (float)System::GetWindowWidth(); }
    float                   WorldTop() const        { return 0.0f; }
    float                   WorldBottom() const     { return (float)System::GetWindowHeight(); }
};

#endif
