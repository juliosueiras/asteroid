#ifndef MISSILE_H_
#define MISSILE_H_

#include "Entity.h"

class Missile : public Entity
{
    Vec2                    mVelocity;

public:
                            Missile(const Vec2& pos, const Texture* tex, float angle, float speed);

    void                    Update(float dt) override;
};

#endif
