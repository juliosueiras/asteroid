#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include "Animation.h"
#include "Vec2.h"

/* The Explosion for Missle , Enemy , and Player
 */
class Explosion
{
    Animation*              mAnim;
    Vec2                    mCenter;
    float                   mScale;

public:
							/* Main Constructor, accept for position and texture
							 * @pos position of the explosion
							 * @tex the texture of the explosion
							 */
                            Explosion(const Vec2& pos, const Texture* tex);
                            Explosion(const Vec2& pos, const Texture* tex, float scale, float duration);
                            ~Explosion();

    void					Update(float dt);

    bool                    IsFinished() const;

    void					Draw(SDL_Renderer* renderer) const;
};

#endif
