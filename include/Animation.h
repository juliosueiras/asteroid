#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <SDL.h>

#include "Texture.h"

/* The Class That control animation
 * is to be used with explosion
 */
class Animation
{
    const Texture*              mTex;

    int                         mNumCells;
    int                         mCellWidth;
    int                         mCellHeight;

    float                       mDuration;          // total duration in seconds
    float                       mTime;              // current time position (in seconds)

    bool                        mIsLoopable;

public:
								/* The main constructor  of Animation
								 * @tex the Texture
								 * @numCells Number of Cells
								 * @duration Duration of the animation
								 * @loopable is the animation loopable
								 */
                                Animation(const Texture* tex, int numCells, float duration, bool loopable);

								/* The main destructor
								 *
								 */
                                ~Animation();

								/* Cell Width of the animation
								 *
								 * @return the cell width
								 */
    int                         CellWidth() const           { return mCellWidth; }

								/* Cell Height of the animation
								 *
								 * @return the cell height
								 */
    int                         CellHeight() const          { return mCellHeight; }

								/* Add time to the animation
								 * @dt duration of time
								 */
    void                        AddTime(float dt);

								/* Is the animation finished playing
								 *
								 * @return the boolean of animation finishedPlaying state
								 */
    bool                        FinishedPlaying() const;

								/* Draw Current Cell of the animation
								 * @renderer SDL renderer
								 * @dstRect SDL rect
								 */
    void                        DrawCurrentCell(SDL_Renderer* renderer, const SDL_Rect* dstRect);
};

#endif
