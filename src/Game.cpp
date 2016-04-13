#include <iostream>
#include "Game.h"
#include "Random.h"

Game::Game()
	: mShuttleTex(NULL)
	, mEnemyTex(NULL)
	, mShotTex(NULL)
	, mShot2Tex(NULL)
	, mExplosionTex(NULL)
	, mPlayer(NULL)
	, mMissiles()
	, mEnemyMissiles()
	, mEnemies()
	, mExplosions()
	, mMinSpawnDelay(0.0f)
	, mMaxSpawnDelay(0.0f)
	, mMinSpawnDelay1(0.0f)
	, mMaxSpawnDelay2(0.0f)
	, mNextSpawnTime(0.0f)
	, mNextSpawnTime2(0.0f)
	, mGameOver(false)
{
    // Nothing to do: all setup should be done in Initialize method
}

Game::~Game()
{
    // Nothing to do: all cleanup should be done in Shutdown method
}

bool Game::Initialize()
{
    std::cout << "Initializing game" << std::endl;
	std::cout << "PRESS R TO RESTART" << std::endl;
	std::cout << "PRESS ESC TO QUIT" << std::endl;

    // Set some window properties
    System::SetWindowSize(480, 640);
    System::SetWindowTitle("MIDTERM 2016 OP VERSION!!!");

    // Get renderer
    SDL_Renderer* renderer = System::GetRenderer();

    //
    // Load all textures
    //
    mShuttleTex = Texture::Load("media/shuttle.png", renderer);
    if (!mShuttleTex) {
        std::cerr << "*** Failed to load shuttle texture" << std::endl;
        return false;
    }
	mEnemyTex = Texture::Load("media/alien.png", renderer);
	if (!mEnemyTex) {
		std::cerr << "*** Failed to load enemy texture" << std::endl;
		return false;
	}
    mShotTex = Texture::Load("media/shot.png", renderer);
    if (!mShotTex) {
        std::cerr << "*** Failed to load shot texture" << std::endl;
        return false;
    }
	mShot2Tex = Texture::Load("media/shot2.png", renderer);
	if (!mShot2Tex) {
		std::cerr << "*** Failed to load shot2 texture" << std::endl;
		return false;
	}
	mExplosionTex = Texture::Load("media/explosion.tga", renderer);
	if (!mExplosionTex) {
		std::cerr << "*** Failed to load explosion texture" << std::endl;
		return false;
	}

	//
	// Initialize random number generator
	//
	InitRandom();

	//
	// Initialize spawner
	//
	mMinSpawnDelay = 2.0f;
	mMaxSpawnDelay = 1.5f;
	mMinSpawnDelay1 = 0.2f;
	mMaxSpawnDelay2 = 1.0f;
	mNextSpawnTime = System::GetTime() + 1.0f;
	mNextSpawnTime2 = System::GetTime() + 1.0f;


    //
    // Spawn player
    //
    Vec2 spawnPos;
    spawnPos.x = 0.5f * System::GetWindowWidth();
    spawnPos.y = 0.75f * System::GetWindowHeight();

    mPlayer = new Player(spawnPos, mShuttleTex);

    mPlayer->SetSpeed(190.0f);
	return true;
}

void Game::Shutdown()
{
    std::cout << "Shutting down game" << std::endl;

    //
    // Delete all entities
    //
    delete mPlayer;

    for (unsigned i = 0; i < mMissiles.size(); i++) {
        delete mMissiles[i];
    }
    mMissiles.clear();

	for (unsigned i = 0; i < mEnemyMissiles.size(); i++) {
		delete mEnemyMissiles[i];
	}
	mEnemyMissiles.clear();

	for (unsigned i = 0; i < mEnemies.size(); i++) {
		delete mEnemies[i];
	}
	mEnemies.clear();

	for (unsigned i = 0; i < mExplosions.size(); i++) {
		delete mExplosions[i];
	}
	mExplosions.clear();

    //
    // Destroy all textures
    //
    Texture::Destroy(mShuttleTex);
    Texture::Destroy(mShotTex);
	Texture::Destroy(mShot2Tex);
	Texture::Destroy(mEnemyTex);
	Texture::Destroy(mExplosionTex);

	mGameOver = false;
}

void Game::Draw(SDL_Renderer* renderer)
{
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw missiles
    for (unsigned i = 0; i < mMissiles.size(); i++) {
        mMissiles[i]->Draw(renderer);
    }

	// Draw enemy missiles
	for (unsigned i = 0; i < mEnemyMissiles.size(); i++) {
		mEnemyMissiles[i]->Draw(renderer);
	}
	
	// Draw ememies
	for (unsigned i = 0; i < mEnemies.size(); i++) {
		mEnemies[i]->Draw(renderer);
	}

	// Draw explosion
	for (unsigned i = 0; i < mExplosions.size(); i++) {
		mExplosions[i]->Draw(renderer);
	}

    // Draw player
    mPlayer->Draw(renderer);
}

void Game::Update(float dt)
{
	// Get world bounds
	float worldLeft = WorldLeft();
	float worldRight = WorldRight();
	float worldTop = WorldTop();
	float worldBottom = WorldBottom();

	// Update player
	mPlayer->Update(dt);

	// Keep the player within world bounds
	if (mPlayer->Left() < worldLeft) {
		mPlayer->SetLeft(worldLeft);
	}
	else if (mPlayer->Right() > worldRight) {
		mPlayer->SetRight(worldRight);
	}
	if (mPlayer->Top() < worldTop) {
		mPlayer->SetTop(worldTop);
	}
	else if (mPlayer->Bottom() > worldBottom) {
		mPlayer->SetBottom(worldBottom);
	}

	// Update enemies out of bounds & hit collision against player
	// Update enemy explosions
	for (unsigned i = 0; i < mEnemies.size();) {
		Enemy* e = mEnemies[i];
		e->Update(dt);

		float distance = Distance(e->Center(), mPlayer->Center());

		if (e->Left() > worldRight || e->Right() < worldLeft || e->Top() > worldBottom || e->Bottom() < worldTop) {
			delete e;
			mEnemies[i] = mEnemies.back();
			mEnemies.pop_back();
		}
		else if (distance < 60 && !mPlayer->isDead()){

			float explosionX = e->Center().x;
			float explosionY = e->Center().y;

			delete e;
			mEnemies[i] = mEnemies.back();
			mEnemies.pop_back();

			Explosion* enemyExplosion = new Explosion(Vec2(explosionX, explosionY), mExplosionTex, 1.5f, 1.0f);
			mExplosions.push_back(enemyExplosion);

			explosionX = mPlayer->Center().x;
			explosionY = mPlayer->Center().y;
			Explosion* enemyExplosion2 = new Explosion(Vec2(explosionX, explosionY), mExplosionTex, 3.0f, 1.5f);
			mExplosions.push_back(enemyExplosion2);

			mPlayer->SetDead(true);
		}
		else {
			i++;
		}
	}

	// Is it time to spawn new enemy?
	float t = System::GetTime();
	if (t >= mNextSpawnTime) {

		// Generate random position, scale, and duration
		float x = RandomFloat(WorldLeft(), WorldRight());
		float y = RandomFloat(WorldTop(), WorldBottom());
		float scale = RandomFloat(0.75f, 2.0f);
		float duration = RandomFloat(5.0f, 5.0f);

		// Initializing enemy spawn
		int enemyStartXPos = rand() % 480;
		int randomX = RandomInt(40, 45);

		if (enemyStartXPos >= 440){
			enemyStartXPos = enemyStartXPos - randomX;
		}
		else if (enemyStartXPos <= 30){
			enemyStartXPos = enemyStartXPos + randomX;
		}

		Vec2 enemyStartVec = Vec2(enemyStartXPos, 0);

		// Create new enemy
		Enemy* e = new Enemy(enemyStartVec, mEnemyTex);
		float speed = RandomFloat(30, 70);
		e->SetVelocity(0.0f, speed);

		// Add enemy to our list
		mEnemies.push_back(e);

		// Set next spawn time
		mNextSpawnTime = t + RandomFloat(mMinSpawnDelay, mMaxSpawnDelay);
	}


	// Update player missiles
	for (unsigned i = 0; i < mMissiles.size();) {
		Missile* m = mMissiles[i];

		m->Update(dt);

		// Remove the missile if it went off screen
		if (m->Left() > worldRight || m->Right() < worldLeft || m->Top() > worldBottom || m->Bottom() < worldTop) {
			// Missile is out of world bounds: remove it
			delete m;
			mMissiles[i] = mMissiles.back();
			mMissiles.pop_back();
		}
		else {
			// Missile is still within world bounds: keep it and move on to the next one
			++i;
		}
	}

	// Update enemy missiles 
	for (unsigned i = 0; i < mEnemyMissiles.size();) {
		Missile* m = mEnemyMissiles[i];

		m->Update(dt);

		// Remove the missile if it went off screen
		if (m->Left() > worldRight || m->Right() < worldLeft || m->Top() > worldBottom || m->Bottom() < worldTop) {
			// Missile is out of world bounds: remove it
			delete m;
			mEnemyMissiles[i] = mEnemyMissiles.back();
			mEnemyMissiles.pop_back();
		}
		else {
			// Missile is still within world bounds: keep it and move on to the next one			
			// Also checking if player gets hit by enemy missiles
			// Setting up collision explosion
			float distance = Distance(mPlayer->Center(), m->Center());

			if (distance < 30){
				delete m;
				mEnemyMissiles[i] = mEnemyMissiles.back();
				mEnemyMissiles.pop_back();

				if (!mPlayer->isDead()){
					float explosionX = mPlayer->Center().x;
					float explosionY = mPlayer->Center().y;
					Explosion* enemyExplosion = new Explosion(Vec2(explosionX, explosionY), mExplosionTex, 3.0f, 1.5f);
					mExplosions.push_back(enemyExplosion);
					mPlayer->SetDead(true);
				}
			}
			++i;
		}
	}

	// Update enemy shots from random enemies
	for (unsigned i = 0; i < mEnemies.size(); i++) {
		Enemy* e = mEnemies[i];
		e->Update(dt);

		if (!mPlayer->isDead()){
			if (e->Center().x > mPlayer->Left() && e->Center().x < mPlayer->Right()){
				if (t >= mNextSpawnTime2) {
					Missile* enemyMissile = new Missile(mEnemies[i]->Center(), mShot2Tex);
					enemyMissile->SetVelocity(0.0f, +250.0f);
					mEnemyMissiles.push_back(enemyMissile);
					mNextSpawnTime2 = t + RandomFloat(mMinSpawnDelay1, mMaxSpawnDelay2);
				}
			}
		}
	} 

	// Update collision if player hits enemy ship
	// Also setting up explosion if collision happens
	for (unsigned a = 0; a < mEnemies.size(); a++) {
		Enemy* e = mEnemies[a];
		e->Update(dt);

		for (unsigned i = 0; i < mMissiles.size();) {

			Missile* x = mMissiles[i];
			x->Update(dt);

			float distance = Distance(e->Center(), x->Center());

			if (distance < 40){

				float explosionX = e->Center().x;
				float explosionY = e->Center().y;
				Explosion* enemyExplosion = new Explosion(Vec2(explosionX, explosionY), mExplosionTex, 1.5f, 1.0f);
				mExplosions.push_back(enemyExplosion);

				delete e;
				mEnemies[a] = mEnemies.back();
				mEnemies.pop_back();

				delete x;
				mMissiles[i] = mMissiles.back();
				mMissiles.pop_back();
			}
			else {
				i++;
			}
		}
		a++;
	}

	// Update missile on missile collision
	// Also setting up explosion if collision happens
	for (unsigned a = 0; a < mEnemyMissiles.size(); a++) {
		Missile* e = mEnemyMissiles[a];
		e->Update(dt);

		for (unsigned i = 0; i < mMissiles.size();) {

			Missile* x = mMissiles[i];
			x->Update(dt);

			float distance = Distance(e->Center(), x->Center());

			if (distance < 10){

				float explosionX = e->Center().x;
				float explosionY = e->Center().y;
				Explosion* enemyExplosion = new Explosion(Vec2(explosionX, explosionY), mExplosionTex, 0.9f, 0.5f);
				mExplosions.push_back(enemyExplosion);

				explosionX = x->Center().x;
				explosionY = x->Center().y;
				Explosion* enemyExplosion2 = new Explosion(Vec2(explosionX, explosionY), mExplosionTex, 0.9f, 0.5f);
				mExplosions.push_back(enemyExplosion2);

				delete e;
				mEnemyMissiles[a] = mEnemyMissiles.back();
				mEnemyMissiles.pop_back();

				delete x;
				mMissiles[i] = mMissiles.back();
				mMissiles.pop_back();
			}
			else {
				i++;
			}
		}
		a++;
	}

	// Update explosions
	for (unsigned i = 0; i < mExplosions.size();) {
		Explosion* e = mExplosions[i];

		e->Update(dt);

		// Remove the explosion if it is finished
		if (e->IsFinished()) {
			delete e;
			mExplosions[i] = mExplosions.back();
			mExplosions.pop_back();
		}
		else {
			// Keep current effect and move on to the next one
			++i;
		}
	}
}

void Game::OnKeyDown(const SDL_KeyboardEvent& kbe)
{
    switch (kbe.keysym.sym) {
    case SDLK_ESCAPE:
        // Tell the system that we want to quit
        System::Quit();
        break;

    case SDLK_SPACE:
        // Fire a missile
        {
			if (!mPlayer->isDead()){

            Missile* missile = new Missile(mPlayer->Center(), mShotTex);
            missile->SetVelocity(0.0f, -350.0f);
            mMissiles.push_back(missile);
			}
            break;
        }
    }
}

void Game::OnKeyUp(const SDL_KeyboardEvent& kbe)
{
}

void Game::OnMouseDown(const SDL_MouseButtonEvent& mbe)
{
}

void Game::OnMouseUp(const SDL_MouseButtonEvent& mbe)
{
}

void Game::OnMouseMotion(const SDL_MouseMotionEvent& mme)
{
}

void Game::OnWindowResized(int w, int h)
{
    std::cout << "Window resized to " << w << 'x' << h << std::endl;
}
