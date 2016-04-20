#include <iostream>

#include "Entity.h"
#include "System.h"

Entity::Entity(const Vec2& pos, const Texture* tex)
    : mTex(tex)
    , mCenter(pos)
    , mRadius(0.0f)
    , mAngle(0.0f)
{
    if (tex) {
        float halfWidth = 0.5f * tex->GetWidth();
        float halfHeight = 0.5f * tex->GetHeight();
        mRadius = 0.5f * (halfWidth + halfHeight);      // average
    }
}

void Entity::Draw(SDL_Renderer* renderer) const
{
    // check if we have a valid texture
    if (mTex) {

        // compute rectangle on screen
        SDL_Rect screenRect;
        screenRect.w = mTex->GetWidth();
        screenRect.h = mTex->GetHeight();
        screenRect.x = (int)(mCenter.x - mTex->GetWidth() / 2);
        screenRect.y = (int)(mCenter.y - mTex->GetHeight() / 2);

        // compute center of rotation
        SDL_Point rotCenter = { mTex->GetWidth() / 2, mTex->GetHeight() / 2 };

        // draw textured rectangle with rotation
        SDL_RenderCopyEx(renderer, mTex->GetSDLTexture(), NULL, &screenRect,
                         mAngle, &rotCenter, SDL_FLIP_NONE);
    } else {
        // missing texture, so draw a placeholder
        SDL_Rect screenRect;
        screenRect.w = 64;
        screenRect.h = 64;
        screenRect.x = (int)(mCenter.x - screenRect.w / 2);
        screenRect.y = (int)(mCenter.y - screenRect.h / 2);

        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderFillRect(renderer, &screenRect);
    }
}
