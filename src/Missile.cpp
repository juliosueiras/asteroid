#include <iostream>

#include "Missile.h"
#include "System.h"
#include "Rotation.h"

Missile::Missile(const Vec2& pos, const Texture* tex, float angle, float speed)
    : Entity(pos, tex)
{
    SetAngle(angle);

    //float radAngle = angle * (3.1415f / 180.0f);
    //mVelocity.x = speed * std::cos(radAngle);
    //mVelocity.y = speed * std::sin(radAngle);

    mVelocity = speed * GetDirectionD(angle);
}

void Missile::Update(float dt)
{
    //mCenter.x += mVelocity.x * dt;
    //mCenter.y += mVelocity.y * dt;

    mCenter += dt * mVelocity;
}
