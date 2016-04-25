#include "Gameplay.h"
#include "Texture.h"
#include "Game.h"
#include "Random.h"
#include "Rotation.h"

#include <iostream>

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

    mAsteroidTex = Texture::Load("media/alien.png", renderer);
    if (not is_texture_loaded(mAsteroidTex, "alien")) { return false; }

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
    // add aliens
    //

    int numAsteroids = 3;
    for (int i = 0; i < numAsteroids; i++) {
        float margin = 50;
        Vec2 pos;
        pos.x = RandomFloat(WorldLeft() + margin, WorldRight() - margin);
        pos.y = RandomFloat(WorldTop() + margin, WorldBottom() - margin);
        float angle = RandomFloat(-180.0f, 180.0f);
        Asteroid* alien = new Asteroid(pos, mAsteroidTex, angle);
        mAsteroids.push_back(alien);
    }
}

void Gameplay::ClearLevel()
{
    delete mPlayer;
    mPlayer = NULL;

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

    // keep the aliens from intersecting each other
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

    // keep the aliens from intersecting the player
    for (unsigned i = 0; i < mAsteroids.size(); i++) {
        Asteroid* alien = mAsteroids[i];
        float d = Distance(alien->Center(), mPlayer->Center());
        if (d < alien->Radius() + mPlayer->Radius()) {
            float depth = alien->Radius() + mPlayer->Radius() - d;      // penetration depth
            Vec2 axis = mPlayer->Center() - alien->Center();            // collision axis
            axis.Normalize();
            alien->SetCenter(alien->Center() - depth * axis);           // push away the alien
        }
    }

    // keep the aliens within world bounds
    for (unsigned i = 0; i < mAsteroids.size(); i++) {
        Asteroid* alien = mAsteroids[i];
        Vec2 vel = alien->Velocity();
        if (alien->Left() < worldLeft) {
            alien->SetRight(worldRight);
            /* if (vel.x < 0) { */
            /*     alien->SetVelocity(Vec2(-vel.x, vel.y)); */
            /* } */
        }
        else if (alien->Right() > worldRight) {
            alien->SetLeft(worldLeft);
            /* if (vel.x > 0) { */
            /*     alien->SetVelocity(Vec2(-vel.x, vel.y)); */
            /* } */
        }
        if (alien->Top() < worldTop) {
            alien->SetBottom(worldBottom);
            /* if (vel.y < 0) { */
            /*     alien->SetVelocity(Vec2(vel.x, -vel.y)); */
            /* } */
        }
        else if (alien->Bottom() > worldBottom) {
            alien->SetTop(worldTop);
            /* if (vel.y > 0) { */
            /*     alien->SetVelocity(Vec2(vel.x, -vel.y)); */
            /* } */
        }
    }

    // update missiles
    for (unsigned i = 0; i < mMissiles.size();) {
        Missile* m = mMissiles[i];

        // update missile
        m->Update(dt);

        // remove the missile if it went off screen
        if (m->Left() > worldRight || m->Right() < worldLeft || m->Top() > worldBottom || m->Bottom() < worldTop) {
            // missile is out of world bounds: remove it
            delete m;
            mMissiles[i] = mMissiles.back();
            mMissiles.pop_back();
        }
        else {
            // missile is still within world bounds: keep it and move on to the next one
            ++i;
        }
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

    // draw player
    mPlayer->Draw(renderer);
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
    }
}
