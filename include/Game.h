#ifndef GAME_H_
#define GAME_H_

#include <SDL.h>
#include <vector>
#include "System.h"
#include "Player.h"
#include "Explosion.h"
#include "Enemy.h"
#include "Missile.h"

class Game
{
    Texture*                mShuttleTex;
    Texture*                mShotTex;
	Texture*				mShot2Tex;
	Texture*				mEnemyTex;
	Texture*				mExplosionTex;

    Player*				    mPlayer;

    std::vector<Missile*>   mMissiles;
	std::vector<Missile*>   mEnemyMissiles;
	std::vector<Enemy*>		mEnemies;
	std::vector<Explosion*>	mExplosions;


	float                   mMinSpawnDelay;
	float                   mMaxSpawnDelay;
	float                   mMinSpawnDelay1;
	float                   mMaxSpawnDelay2;
	float                   mNextSpawnTime;
	float                   mNextSpawnTime2;

	bool					mGameOver;


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

    float                   WorldLeft() const       { return 0.0f; }
    float                   WorldRight() const      { return (float)System::GetWindowWidth(); }
    float                   WorldTop() const        { return 0.0f; }
    float                   WorldBottom() const     { return (float)System::GetWindowHeight(); }

	bool					isGameOver() const		{ return mGameOver; }
	void					SetGameOver(bool isGameOver)	{ mGameOver = isGameOver; }
};

#endif
