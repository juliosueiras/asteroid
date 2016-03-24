#ifndef ENEMY_H_
#define ENEMY_H_

#include "Texture.h"
#include "Vec2.h"

class Enemy
{
    const Texture*          mTex;
    Vec2                    mCenter;
    Vec2                    mVelocity;

public:
	/* Main constructor
	 * @pos position of the enemy
	 * @tex texture of the enemy
	 */
                            Enemy(const Vec2& pos, const Texture* tex);
                            ~Enemy();

    /* Center of enemy
     *
     * @return Vec2 of center of the enemy
     */
    const Vec2&             Center() const                  { return mCenter; }

	/* Setter for center of the enemy
	 * @pos Vec2 position of the enemy
	 */
    void                    SetCenter(const Vec2& pos)      { mCenter = pos; }

	/* Setter for center of the enemy
	 * @x x position for setting the center of the enemy
	 * @y y position for setting the center of the enemy
	 */
    void                    SetCenter(float x, float y)     { mCenter = Vec2(x, y); }

	/* Velocity of the enemy
	 *
	 * @return the Velocity
	 */
    const Vec2&             Velocity() const                { return mVelocity; }

	/* Setter for velocity of the enemy
	 * @vel Vec2 velocity of the enemy
	 */
    void                    SetVelocity(const Vec2& vel)    { mVelocity = vel; }

	/* Setter for velocity of the enemy
	 * @x x value of velocity of the enemy
	 * @y y value of velocity of the enemy
	 */
    void                    SetVelocity(float x, float y)   { mVelocity = Vec2(x, y); }

	/* Width of the enemy
	 *
	 * @return the width of the enemy
	 */
    float                   Width() const                   { return (float)mTex->GetWidth(); }

	/* Height of the enemy
	 *
	 * @return the height of the enemy
	 */
    float                   Height() const                  { return (float)mTex->GetHeight(); }

	/* Left of the enemy
	 *
	 * @return the left of the enemy
	 */
    float                   Left() const                    { return mCenter.x - 0.5f * Width(); }

	/* right of the enemy
	 *
	 * @return the right of the enemy
	 */
    float                   Right() const                   { return mCenter.x + 0.5f * Width(); }

	/* top of the enemy
	 *
	 * @return the top of the enemy
	 */
    float                   Top() const                     { return mCenter.y - 0.5f * Height(); }

	/* bottom of the enemy
	 *
	 * @return the bottom of the enemy
	 */
    float                   Bottom() const                  { return mCenter.y + 0.5f * Height(); }

    void                    SetLeft(float x)                { mCenter.x = x + 0.5f * Width(); }
    void                    SetRight(float x)               { mCenter.x = x - 0.5f * Width(); }
    void                    SetTop(float y)                 { mCenter.y = y + 0.5f * Height(); }
    void                    SetBottom(float y)              { mCenter.y = y - 0.5f * Height(); }

    void					Update(float dt);

    void					Draw(SDL_Renderer* renderer) const;
};

#endif
