#include "Player.h"
#include "System.h"
#include "Rotation.h"

#include <iostream>

Player::Player(const Vec2& pos, const Texture* tex)
    : Entity(pos, tex)
    , mSpeed(100.0f)        // pixels/second
    , mRotSpeed(180.0f)     // degrees/second
{
}

void Player::Update(float dt)
{
    const Uint8* keys = System::GetKeyStates();

    //
    // apply rotations
    //
    if (keys[SDL_SCANCODE_D]) {
        mAngle += mRotSpeed * dt;
    }
    if (keys[SDL_SCANCODE_A]) {
        mAngle -= mRotSpeed * dt;
    }

    // keep the angle in standard range (-180, 180]
    mAngle = StandardizeAngle(mAngle);

    //std::cout << (int)mAngle << std::endl;

    float radAngle = Deg2Rad(mAngle);

    Vec2 dir = GetDirectionR(radAngle);

    if (keys[SDL_SCANCODE_W]) {
        //mCenter.x += mSpeed * dt * std::cos(radAngle);
        //mCenter.y += mSpeed * dt * std::sin(radAngle);
        mCenter += mSpeed * dt * dir;
    }
    if (keys[SDL_SCANCODE_S]) {
        //mCenter.x -= mSpeed * dt * std::cos(radAngle);
        //mCenter.y -= mSpeed * dt * std::sin(radAngle);
        mCenter -= mSpeed * dt * dir;
    }
}

bool Player::IsDead(bool dead) {
   mDead = dead; 
}
