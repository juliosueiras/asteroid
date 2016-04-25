#include "Gameplay.h"
#include "Texture.h"
#include "Game.h"
#include "Random.h"
#include "Rotation.h"

#include <iostream>

int score = 0;

bool is_texture_loaded(Texture* tex, std::string texName)
{
    if (not tex) {
        std::cerr << "*** Failed to load " << texName << " texture" << std::endl;
        return false;
    }
    return true;
}

void reverse_direction(Asteroid* asteroid, Vec2 vel) {
    if ((vel.y > 0 || vel.y < 0) && (vel.x > 0 || vel.x < 0)) {
        asteroid->SetVelocity(Vec2(-vel.x, -vel.y));
    } else if (vel.y > 0 || vel.y < 0) {
        asteroid->SetVelocity(Vec2(vel.x, -vel.y));
    } else if (vel.x > 0 || vel.x < 0) {
        asteroid->SetVelocity(Vec2(-vel.x, vel.y));
    }
}



Gameplay::Gameplay(Game* game)
    : GameState(game)
    , mShuttleTex(NULL)
    , mShotTex(NULL)
    , mAsteroidTex(NULL)
    , mBackgroundTex(NULL)
    , mExplosionTex(NULL)
    , mPlayer(NULL)
    , mMissiles()
    , mExplosions()
    , mIsActive(false)
{
}

Gameplay::~Gameplay()
{
}

bool Gameplay::Initialize()
{
    // get renderer
    SDL_Renderer* renderer = System::GetRenderer();

    SDL_Rect stretchRect;
    stretchRect.x = 0;
    stretchRect.y = 0;
    stretchRect.w = System::GetWindowWidth();
    stretchRect.h = System::GetWindowHeight();

    /* SDL_RenderCopy(renderer, , renderer), NULL, NULL); */


    //
    // load all textures
    //
    mShuttleTex = Texture::Load("media/shuttle.png", renderer);
    if (not is_texture_loaded(mShuttleTex, "shuttle")) { return false; }

    mShotTex = Texture::Load("media/shot.png", renderer);
    if (not is_texture_loaded(mShotTex, "shot")) { return false; }

    mAsteroidTex = Texture::Load("media/asteroid.png", renderer);
    if (not is_texture_loaded(mAsteroidTex, "asteroid")) { return false; }

    mBackgroundTex = Texture::Load("media/background.jpg", renderer);
    if (not is_texture_loaded(mBackgroundTex, "background")) { return false; }

    mExplosionTex = Texture::Load("media/explosion.tga", renderer);
    if (not is_texture_loaded(mExplosionTex, "explosion")) { return false; }

    LoadLevel();

    return true;
}

void Gameplay::Shutdown()
{
    ClearLevel();

    // destroy all textures
    Texture::Destroy(mShuttleTex);
    Texture::Destroy(mShotTex);
    Texture::Destroy(mAsteroidTex);
}

void Gameplay::LoadLevel()
{
    ClearLevel();

    //
    // spawn player
    //

    Vec2 spawnPos;
    spawnPos.x = 0.5f * System::GetWindowWidth();
    spawnPos.y = 0.75f * System::GetWindowHeight();

    mPlayer = new Player(spawnPos, mShuttleTex);
    mPlayer->SetSpeed(150.0f);
    mPlayer->SetAngle(-90.0f);

    //
    // add asteroids
    //

    int numAsteroids = 5;
    for (int i = 0; i < numAsteroids; i++) {
        float margin = 50;
        Vec2 pos;
        pos.x = RandomFloat(WorldLeft() + margin, WorldRight() - margin);
        pos.y = RandomFloat(WorldTop() + margin, WorldBottom() - margin);
        float angle = RandomFloat(-180.0f, 180.0f);
        Asteroid* asteroid = new Asteroid(pos, mAsteroidTex, angle);
        mAsteroids.push_back(asteroid);
    }
}

void Gameplay::ClearLevel()
{
    delete mPlayer;
    mPlayer = NULL;
    score = 0;

    for (auto it = mMissiles.begin(); it != mMissiles.end(); it++) {
        Missile* m = *it;
        delete m;
    }
    mMissiles.clear();

    for (auto it = mAsteroids.begin(); it != mAsteroids.end(); it++) {
        Asteroid* a = *it;
        delete a;
    }

    mAsteroids.clear();
}

void Gameplay::Update(float dt)
{
    // get world bounds
    float worldLeft = WorldLeft();
    float worldRight = WorldRight();
    float worldTop = WorldTop();
    float worldBottom = WorldBottom();

    // update player
    mPlayer->Update(dt);

    // keep the player within world bounds
    if(! mPlayer->IsDead()) {
        if (mPlayer->Left() < worldLeft) {
            mPlayer->SetRight(worldRight);
        }
        else if (mPlayer->Right() > worldRight) {
            mPlayer->SetLeft(worldLeft);
        }
        if (mPlayer->Top() < worldTop) {
            mPlayer->SetBottom(worldBottom);
        }
        else if (mPlayer->Bottom() > worldBottom) {
            mPlayer->SetTop(worldTop);
        }
    }

    // keep the asteroids from intersecting each other
    for (unsigned i = 0; i < mAsteroids.size(); i++) {
        Asteroid* asteroid1 = mAsteroids[i];
        for (unsigned j = i + 1; j < mAsteroids.size(); j++) {
            Asteroid* asteroid2 = mAsteroids[j];
            float d = Distance(asteroid1->Center(), asteroid2->Center());
            if (d < asteroid1->Radius() + asteroid2->Radius()) {
                float depth = asteroid1->Radius() + asteroid2->Radius() - d;      // penetration depth
                float halfDepth = 0.5f * depth;
                Vec2 axis = asteroid2->Center() - asteroid1->Center();            // collision axis
                axis.Normalize();
                asteroid1->SetCenter(asteroid1->Center() - halfDepth * axis);     // push away asteroid 1

            reverse_direction(asteroid1, asteroid1->Velocity());

            reverse_direction(asteroid2, asteroid2->Velocity());

            asteroid2->SetCenter(asteroid2->Center() + halfDepth * axis);     // push away asteroid 2
            }
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

    // check asteroid collide with player
    if (not Gameplay::IsGodMode()) {
        for (unsigned i = 0; i < mAsteroids.size(); i++) {
            Asteroid* asteroid = mAsteroids[i];
            float d = Distance(asteroid->Center(), mPlayer->Center());
            if (d < asteroid->Radius() + mPlayer->Radius()) {

                Explosion* e = new Explosion(mPlayer->Center(), mExplosionTex, 3.0, 0.5);
                mExplosions.push_back(e);

                mPlayer->IsDead(true);

                mPlayer->SetTop(System::GetWindowHeight() + 10000);
            }
        }
    }

    // detect collision with missle and asteroid
	for (unsigned i = 0; i < mMissiles.size();   ) {
		Missile* m = mMissiles[i];

		// update missile
		m->Update(dt);

		for (unsigned j = 0; j < mAsteroids.size();   ) {
			Asteroid* asteroid = mAsteroids[j];

			// detect if missle collide with a asteroid
			if (Distance(m->Center(), asteroid->Center()) < 35) {

				// create explosion
				Explosion* e = new Explosion(m->Center(), mExplosionTex, 1.5, 0.5);
				mExplosions.push_back(e);

				// remove both missle and asteroid
				delete m;
				mMissiles[i] = mMissiles.back(); mMissiles.pop_back();

				delete asteroid;
				mAsteroids[j] = mAsteroids.back();
				mAsteroids.pop_back();

                score++;
                std::cout << "Score:" << score << std::endl;
			} else {
				// missile is still within world bounds: keep it and move on to the next one
				++j;
			}
		}

		++i;
	}

    // warp the asteroid
    for (unsigned i = 0; i < mAsteroids.size(); i++) {
        Asteroid* asteroid = mAsteroids[i];
        Vec2 vel = asteroid->Velocity();
        if (asteroid->Left() < worldLeft) {
            asteroid->SetRight(worldRight);
        }
        else if (asteroid->Right() > worldRight) {
            asteroid->SetLeft(worldLeft);
        }
        if (asteroid->Top() < worldTop) {
            asteroid->SetBottom(worldBottom);
        }
        else if (asteroid->Bottom() > worldBottom) {
            asteroid->SetTop(worldTop);
        }
    }

    // update missiles
    for (unsigned i = 0; i < mMissiles.size();) {
        Missile* m = mMissiles[i];

        // update missile
        m->Update(dt);

        // remove the missile if it went off screen
        if (m->Left() < worldLeft) {
            m->SetRight(worldRight);
        }
        else if (m->Right() > worldRight) {
            m->SetLeft(worldLeft);
        }
        if (m->Top() < worldTop) {
            m->SetBottom(worldBottom);
        }
        else if (m->Bottom() > worldBottom) {
            m->SetTop(worldTop);
        }

        ++i;
    }

    for (unsigned i = 0; i < mAsteroids.size(); i++) {
        mAsteroids[i]->Update(dt);
    }

    mIsActive = true;
}

void Gameplay::Draw(SDL_Renderer* renderer)
{
    // clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // draw missiles
    for (unsigned i = 0; i < mMissiles.size(); i++) {
        mMissiles[i]->Draw(renderer);
    }

    for (unsigned i = 0; i < mAsteroids.size(); i++) {
        mAsteroids[i]->Draw(renderer);
    }
    //
	// draw explosions
	for (unsigned i = 0; i < mExplosions.size(); i++) {
		mExplosions[i]->Draw(renderer);
	}

    // draw player
    if(! mPlayer->IsDead()) {
        mPlayer->Draw(renderer);
    }
}

void Gameplay::OnKeyDown(const SDL_KeyboardEvent& kbe)
{
    switch (kbe.keysym.sym) {
    case SDLK_ESCAPE:
        //std::cout << "User pressed Escape" << std::endl;
        mGame->EnterMainMenu();
        break;

    case SDLK_SPACE:
        // fire a missile
        {
            Missile* missile = new Missile(mPlayer->Center(), mShotTex, mPlayer->Angle(), 400.0f);
            mMissiles.push_back(missile);
        }
        break;
    case SDLK_g:
        {
            if (Gameplay::IsGodMode()) {
                std::cout << "God Mode disable" << std::endl;
                Gameplay::SetGodMode(false);
            } else {
                std::cout << "God Mode enable" << std::endl;
                Gameplay::SetGodMode(true);
            }
        }
        break;
    }
}
bool Gameplay::SetGodMode(const bool godMode)  {
    mGodMode = godMode;
}
