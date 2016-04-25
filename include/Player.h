#ifndef PLAYER_H_
#define PLAYER_H_

#include "Entity.h"

class Player : public Entity
{
    float                   mSpeed;     // pixels/second
    float                   mRotSpeed;  // degrees/second
    bool                    mDead = false;

public:
                            Player(const Vec2& pos, const Texture* tex);

    float                   Speed() const                   { return mSpeed; }
    void                    SetSpeed(float speed)           { mSpeed = speed; }

    float                   RotSpeed() const                { return mRotSpeed; }
    void                    SetRotSpeed(float speed)        { mRotSpeed = speed; }

    void                    Update(float dt) override;

    bool                    IsDead()                        { return mDead; }
    bool                    IsDead(bool dead);
};

#endif
