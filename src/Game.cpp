#include <iostream>

#include "Game.h"

int lastTime = 0, currentTime;

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
	System::SetWindowSize(480, 640);
	System::SetWindowTitle("Space Shootah!");

	// get renderer
	SDL_Renderer* renderer = System::GetRenderer();
	//
	// load all textures
	//
	mShuttleTex = Texture::Load("media/shuttle.png", renderer);
	if (!mShuttleTex) {
		std::cerr << "*** Failed to load shuttle texture" << std::endl;
		return false;
	}

	mEnemyTex = Texture::Load("media/alien.png", renderer);
	if (!mEnemyTex) {
		std::cerr << "*** Failed to load alien texture" << std::endl;
		return false;
	}

	mShotTex = Texture::Load("media/shot.png", renderer);
	if (!mShotTex) {
		std::cerr << "*** Failed to load shot texture" << std::endl;
		return false;
	}

	mShot2Tex = Texture::Load("media/shot2.png", renderer);
	if (!mShotTex) {
		std::cerr << "*** Failed to load shot2 texture" << std::endl;
		return false;
	}

	mExplosionTex = Texture::Load("media/explosion.tga", renderer);
	if (!mExplosionTex) {
		std::cerr << "*** Failed to load explosion texture" << std::endl;
		return false;
	}

	//
	// spawn player
	//

	Vec2 spawnPos;
	spawnPos.x = 0.5f * System::GetWindowWidth();
	spawnPos.y = 0.75f * System::GetWindowHeight();


	InitRandom();

	mPlayer = new Player(spawnPos, mShuttleTex);

	mPlayer->SetSpeed(150.0f);

	mMinSpawnDelay = 0.1f;
	mMaxSpawnDelay = 1.5f;
	mNextSpawnTime = System::GetTime() + 1.0f;

	return true;
}

void Game::Shutdown()
{
	std::cout << "Shutting down game" << std::endl;

	//
	// delete all entities
	//

	// delete player
	delete mPlayer;

	// delete enemy
	for (unsigned i = 0; i < mEnemies.size(); i++) {
		delete mEnemies[i];
	}
	mEnemies.clear();

	// delete missiles
	for (unsigned i = 0; i < mMissiles.size(); i++) {
		delete mMissiles[i];
	}
	mMissiles.clear();

	// delete enemy missiles
	for (unsigned i = 0; i < mEnemyMissiles.size(); i++) {
		delete mEnemyMissiles[i];
	}
	mEnemyMissiles.clear();

	// delete all explosion
	for (unsigned i = 0; i < mExplosions.size(); i++) {
		delete mExplosions[i];
	}
	mExplosions.clear();

	//
	// destroy all textures
	//
	Texture::Destroy(mShuttleTex);
	Texture::Destroy(mEnemyTex);
	Texture::Destroy(mShotTex);
	Texture::Destroy(mShot2Tex);
	Texture::Destroy(mExplosionTex);
}

void Game::Draw(SDL_Renderer* renderer)
{
	// clear the screen
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// draw missiles
	for (unsigned i = 0; i < mMissiles.size(); i++) {
		mMissiles[i]->Draw(renderer);
	}

	// draw enemy missiles
	for (unsigned i = 0; i < mEnemyMissiles.size(); i++) {
		mEnemyMissiles[i]->Draw(renderer);
	}

	// draw enemies
	for (unsigned i = 0; i < mEnemies.size(); i++) {
		mEnemies[i]->Draw(renderer);
	}

	// draw explosions
	for (unsigned i = 0; i < mExplosions.size(); i++) {
		mExplosions[i]->Draw(renderer);
	}

	// draw player
	if(!mPlayer->IsDead()) {
		mPlayer->Draw(renderer);
	}

}

void Game::Update(float dt)
{
	//std::cout << dt << std::endl;

	// get world bounds
	float worldLeft = WorldLeft();
	float worldRight = WorldRight();
	float worldTop = WorldTop();
	float worldBottom = WorldBottom();


	float t = System::GetTime();

	// is it time to spawn next effect?
	if (t >= mNextSpawnTime) {

		Enemy* enemy = new Enemy(Vec2(RandomInt(System::GetWindowWidth()),worldTop), mEnemyTex);
		enemy->SetVelocity(0.0f, 200.0f);
		mEnemies.push_back(enemy);

		// set next spawn time
		mNextSpawnTime = t + RandomFloat(mMinSpawnDelay, mMaxSpawnDelay);
	}

	// update player
	if(!mPlayer->IsDead()){

		mPlayer->Update(dt);

		// keep the player within world bounds
		if (mPlayer->Left() < worldLeft) {
			mPlayer->SetLeft(worldLeft);
		} else if (mPlayer->Right() > worldRight) {
			mPlayer->SetRight(worldRight);
		}
		if (mPlayer->Top() < worldTop) {
			mPlayer->SetTop(worldTop);
		} else if (mPlayer->Bottom() > worldBottom) {
			mPlayer->SetBottom(worldBottom);
		}
   	}


	// detect collision with missle and enemies
	for (unsigned i = 0; i < mMissiles.size();   ) {
		Missile* m = mMissiles[i];

		// update missile
		m->Update(dt);

		for (unsigned j = 0; j < mEnemies.size();   ) {
			Enemy* enemy = mEnemies[j];

			// detect if missle collide with a enemy
			if (Distance(m->Center(), enemy->Center()) < 35) {

				// create explosion
				Explosion* e = new Explosion(m->Center(), mExplosionTex, 1.5, 0.5);
				mExplosions.push_back(e);

				// remove both missle and enemy
				delete m;
				mMissiles[i] = mMissiles.back(); mMissiles.pop_back();

				delete enemy;
				mEnemies[j] = mEnemies.back();
				mEnemies.pop_back();
			} else {
				// missile is still within world bounds: keep it and move on to the next one
				++j;
			}
		}

		++i;
	}

	// update missiles
	for (unsigned i = 0; i < mMissiles.size();   ) {
		Missile* m = mMissiles[i];

		// update missile
		m->Update(dt);

		// remove the missile if it went off screen
		if (m->Left() > worldRight || m->Right() < worldLeft || m->Top() > worldBottom || m->Bottom() < worldTop) {
			// missile is out of world bounds: remove it
			delete m;
			mMissiles[i] = mMissiles.back();
			mMissiles.pop_back();
		} else {
			// missile is still within world bounds: keep it and move on to the next one
			++i;
		}
	}

	// detect collision with missle and enemy missle
	for (unsigned i = 0; i < mMissiles.size();   ) {
		Missile* m = mMissiles[i];

		// update missile
		m->Update(dt);

		for (unsigned j = 0; j < mEnemyMissiles.size();   ) {
			Missile* enemyM= mEnemyMissiles[j];

			// detect if missle collide with a enemy
			if (Distance(m->Center(), enemyM->Center()) < 35) {

				// create explosion for player missle
				Explosion* e = new Explosion(m->Center(), mExplosionTex, 0.5, 0.5);
				mExplosions.push_back(e);

				// remove both missle
				delete m;
				mMissiles[i] = mMissiles.back();
				mMissiles.pop_back();

				delete enemyM;
				mEnemyMissiles[j] = mEnemyMissiles.back();
				mEnemyMissiles.pop_back();
			} else {
				// missile is still within world bounds: keep it and move on to the next one
				++j;
			}
		}

		++i;
	}

	// detect collision with player and enemies
	for (unsigned i = 0; i < mEnemies.size();   ) {
		Enemy* enemy = mEnemies[i];

		// remove the enemy if it went off screen
		if (Distance(enemy->Center(), mPlayer->Center()) < 58 ) {

			// create explosion
			Explosion* e = new Explosion(mPlayer->Center(), mExplosionTex, 3.0, 0.5);
			mExplosions.push_back(e);

			// remove enemy and player
			delete enemy;
			mEnemies[i] = mEnemies.back();
			mEnemies.pop_back();

			mPlayer->IsDead(true);

			mPlayer->SetTop(System::GetWindowHeight() + 200);

		} else {
			// enemy is still within world bounds: keep it and move on to the next one
			++i;
		}
	}

	// update enemies
	for (unsigned i = 0; i < mEnemies.size();   ) {
		Enemy* enemy = mEnemies[i];

		// update enemy
		enemy->Update(dt);

		float currentTime = SDL_GetTicks();

		if(currentTime > lastTime + RandomInt(8000, 100)) {

			Missile* missile = new Missile(mEnemies[i]->Center(), mShot2Tex);
			missile->SetVelocity(0.0f, 200.0f);
			mEnemyMissiles.push_back(missile);
			lastTime = currentTime;

		}

		// remove the enemy if it went off screen
		if (enemy->Left() > worldRight ||enemy->Right() < worldLeft || enemy->Top() > worldBottom || enemy->Bottom() < worldTop) {
			// enemy is out of world bounds: remove it
			delete enemy;
			mEnemies[i] = mEnemies.back();
			mEnemies.pop_back();
		} else {
			// enemy is still within world bounds: keep it and move on to the next one
			++i;
		}
	}

	// update explosions
	for (unsigned i = 0; i < mExplosions.size(); ) {
		Explosion* e = mExplosions[i];

		// update explosion
		e->Update(dt);

		// remove the explosion if it is finished
		if (e->IsFinished()) {
			delete e;
			mExplosions[i] = mExplosions.back();
			mExplosions.pop_back();
		} else {
			// keep current effect and move on to the next one
			++i;
		}
	}

	// update enemy missiles
	for (unsigned i = 0; i < mEnemyMissiles.size();   ) {

		Missile* m = mEnemyMissiles[i];

		// update missile
		m->Update(dt);

		// remove the missile if it went off screen
		if (m->Left() > worldRight || m->Right() < worldLeft || m->Top() > worldBottom || m->Bottom() < worldTop) {
			// missile is out of world bounds: remove it
			delete m;
			mEnemyMissiles[i] = mEnemyMissiles.back();
			mEnemyMissiles.pop_back();
		} else {
			// missile is still within world bounds: keep it and move on to the next one
			++i;
		}
	}

	// detect collision with enemy missle and player
	for (unsigned i = 0; i < mEnemyMissiles.size();   ) {
		Missile* m = mEnemyMissiles[i];

		// update missile
		m->Update(dt);

		// detect if enemy missle collide with player
		if (Distance(m->Center(), mPlayer->Center()) < 35) {

			// create explosion
			Explosion* e = new Explosion(mPlayer->Center(), mExplosionTex, 3.0, 0.5);
			mExplosions.push_back(e);
			// remove both missle and enemy
			delete m;
			mEnemyMissiles[i] = mEnemyMissiles.back();
			mEnemyMissiles.pop_back();

			mPlayer->IsDead(true);

			mPlayer->SetTop(System::GetWindowHeight() + 200);
		} else {
			++i;
		}

	}
}

void Game::OnKeyDown(const SDL_KeyboardEvent& kbe)
{
	switch (kbe.keysym.sym) {
		case SDLK_ESCAPE:
			// tell the system that we want to quit
			System::Quit();
			break;

		case SDLK_SPACE:
			// fire a missile
			{
				Missile* missile = new Missile(mPlayer->Center(), mShotTex);
				missile->SetVelocity(0.0f, -400.0f);
				mMissiles.push_back(missile);
				break;
			}
		case SDLK_r:
			{
				Game::Reset();
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

void Game::Reset()
{
	//
	// delete all entities
	//

	// delete enemy
	for (unsigned i = 0; i < mEnemies.size(); i++) {
		delete mEnemies[i];
	}
	mEnemies.clear();

	// delete missiles
	for (unsigned i = 0; i < mMissiles.size(); i++) {
		delete mMissiles[i];
	}
	mMissiles.clear();

	// delete enemy missiles
	for (unsigned i = 0; i < mEnemyMissiles.size(); i++) {
		delete mEnemyMissiles[i];
	}
	mEnemyMissiles.clear();

	// delete all explosion
	for (unsigned i = 0; i < mExplosions.size(); i++) {
		delete mExplosions[i];
	}
	mExplosions.clear();

	Vec2 spawnPos;
	spawnPos.x = 0.5f * System::GetWindowWidth();
	spawnPos.y = 0.75f * System::GetWindowHeight();

	mPlayer->SetCenter(spawnPos);
	mPlayer->IsDead(false);

}
