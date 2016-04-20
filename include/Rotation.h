#ifndef ROTATION_H_
#define ROTATION_H_

#include "Vec2.h"
#include <cmath>

const float PI = 3.14159265359f;

inline float Deg2Rad(float deg)
{
    return (PI / 180.f) * deg;
}

inline float Rad2Deg(float rad)
{
    return (180.f / PI) * rad;
}

inline float StandardizeAngle(float angle)
{
    while (angle <= -180.0f) {
        angle += 360.0f;
    }
    while (angle > 180.0f) {
        angle -= 360.0f;
    }
    return angle;
}

inline float SignedAngleDiff(float srcAngle, float dstAngle)
{
    float diff = dstAngle - srcAngle;
    return StandardizeAngle(diff);      // return result in range (-180, 180]
}

// get direction vector corresponding to an angle expressed in radians
inline Vec2 GetDirectionR(float radAngle)
{
    return Vec2(std::cos(radAngle), std::sin(radAngle));
}

// get direction vector corresponding to an angle expressed in degrees
inline Vec2 GetDirectionD(float degAngle)
{
    float radAngle = Deg2Rad(degAngle);
    return Vec2(std::cos(radAngle), std::sin(radAngle));
}

// return the angle of a vector (in radians)
inline float GetAngleR(const Vec2& v)
{
    if (v.LengthSquared() > 0.0f) {
        return std::atan2(v.y, v.x);
    }
    else {
        return 0.0f;
    }
}

// return the angle of a vector (in degrees)
inline float GetAngleD(const Vec2& v)
{
    if (v.LengthSquared() > 0.0f) {
        return Rad2Deg(std::atan2(v.y, v.x));
    }
    else {
        return 0.0f;
    }
}

#endif
