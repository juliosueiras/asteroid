#ifndef ASTEROID_H_
#define ASTEROID_H_

#include "Entity.h"
#include "Animation.h"

class Asteroid : public Entity
{
    Vec2                    mVelocity;
    float                   mSpeed;

public:
                            Asteroid(const Vec2& pos, const Texture* tex, float angle);

    void                    Update(float dt) override;

    const Vec2&             Velocity()                      { return mVelocity; }
    void                    SetVelocity(const Vec2& v);
};

#endif
