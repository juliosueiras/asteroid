#ifndef ENTITY_H_
#define ENTITY_H_

#include "Texture.h"
#include "Vec2.h"

class Entity
{
protected:
    const Texture*          mTex;
    Vec2                    mCenter;
    float                   mRadius;
    float                   mAngle;

public:
                            Entity(const Vec2& pos, const Texture* tex);

    virtual                 ~Entity()                       {}

    const Vec2&             Center() const                  { return mCenter; }
    void                    SetCenter(const Vec2& pos)      { mCenter = pos; }
    void                    SetCenter(float x, float y)     { mCenter = Vec2(x, y); }

    float                   Angle() const                   { return mAngle; }
    void                    SetAngle(float angle)           { mAngle = angle; }

    //float                   Width() const                   { return (float)mTex->GetWidth(); }
    //float                   Height() const                  { return (float)mTex->GetHeight(); }

    float                   Radius() const                  { return mRadius; }

    float                   Left() const                    { return mCenter.x - mRadius; }
    float                   Right() const                   { return mCenter.x + mRadius; }
    float                   Top() const                     { return mCenter.y - mRadius; }
    float                   Bottom() const                  { return mCenter.y + mRadius; }

    void                    SetLeft(float x)                { mCenter.x = x + mRadius; }
    void                    SetRight(float x)               { mCenter.x = x - mRadius; }
    void                    SetTop(float y)                 { mCenter.y = y + mRadius; }
    void                    SetBottom(float y)              { mCenter.y = y - mRadius; }

    virtual void		    Update(float dt)                {}

    virtual void			Draw(SDL_Renderer* renderer) const;
};

#endif
