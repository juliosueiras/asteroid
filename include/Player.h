#ifndef PLAYER_H_
#define PLAYER_H_

#include "Texture.h"
#include "Vec2.h"

class Player
{
    const Texture*          mTex;
    Vec2                    mCenter;
    float                   mSpeed;
	bool					mDead;

public:
                            Player(const Vec2& pos, const Texture* tex);
                            ~Player();

    const Vec2&             Center() const                  { return mCenter; }
    void                    SetCenter(const Vec2& pos)      { mCenter = pos; }
    void                    SetCenter(float x, float y)     { mCenter = Vec2(x, y); }

    float                   Speed() const                   { return mSpeed; }
    void                    SetSpeed(float speed)           { mSpeed = speed; }

    float                   Width() const                   { return (float)mTex->GetWidth(); }
    float                   Height() const                  { return (float)mTex->GetHeight(); }

	void					SetDead(bool isDead)			{ mDead = isDead;  }
	bool					isDead() const					{ return mDead; }

    float                   Left() const                    { return mCenter.x - 0.5f * Width(); }
    float                   Right() const                   { return mCenter.x + 0.5f * Width(); }
    float                   Top() const                     { return mCenter.y - 0.5f * Height(); }
    float                   Bottom() const                  { return mCenter.y + 0.5f * Height(); }

    void                    SetLeft(float x)                { mCenter.x = x + 0.5f * Width(); }
    void                    SetRight(float x)               { mCenter.x = x - 0.5f * Width(); }
    void                    SetTop(float y)                 { mCenter.y = y + 0.5f * Height(); }
    void                    SetBottom(float y)              { mCenter.y = y - 0.5f * Height(); }

    void					Update(float dt);

    void					Draw(SDL_Renderer* renderer) const;
};

#endif
