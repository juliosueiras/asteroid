#include "Asteroid.h"
#include "Random.h"
#include "Rotation.h"

Asteroid::Asteroid(const Vec2& pos, const Texture* tex, float angle)
    : Entity(pos, tex)
    , mVelocity(0.0f, 0.0f)
    , mSpeed(0.0f)
{
    SetAngle(angle);

    mSpeed = RandomFloat(100, 200);

    Vec2 dir = GetDirectionD(angle);

    mVelocity = mSpeed * dir;
}

void Asteroid::Update(float dt)
{
    mCenter += dt * mVelocity;
}

void Asteroid::SetVelocity(const Vec2& v)
{
    mVelocity = v;
    SetAngle(GetAngleD(v));
}
