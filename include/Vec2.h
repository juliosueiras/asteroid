#ifndef VEC2_H_
#define VEC2_H_

#include <cmath>

struct Vec2
{
    float x, y;

    Vec2()
        : x(0), y(0)
    { }

    Vec2(float x, float y)
        : x(x), y(y)
    { }

    Vec2 operator+ (const Vec2& other) const
    {
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 operator- (const Vec2& other) const
    {
        return Vec2(x - other.x, y - other.y);
    }

    Vec2 operator- () const
    {
        return Vec2(-x, -y);
    }

    Vec2 operator* (float s) const
    {
        return Vec2(x*s, y*s);
    }

    Vec2 operator/ (float s) const
    {
        return Vec2(x / s, y / s);
    }

    Vec2& operator+= (const Vec2& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vec2& operator-= (const Vec2& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vec2& operator*= (float s)
    {
        x *= s;
        y *= s;
        return *this;
    }

    Vec2& operator/= (float s)
    {
        x /= s;
        y /= s;
        return *this;
    }

    float Length() const
    {
        return std::sqrt(x*x + y*y);
    }

    float LengthSquared() const
    {
        return x*x + y*y;
    }

    void Normalize()
    {
        float len = Length();
        if (len > 0) {
            x /= len;
            y /= len;
        }
    }

    // return a normalized copy of this vector
    Vec2 Direction() const
    {
        Vec2 result(x, y);      // create copy
        result.Normalize();     // normalize copy
        return result;
    }
};

// global overloated operator* for float multiplied by Vec2
inline Vec2 operator* (float s, const Vec2& v)
{
    return Vec2(s * v.x, s * v.y);
}


inline float Distance(const Vec2& a, const Vec2& b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx*dx + dy*dy);
}

inline float DistanceSquared(const Vec2& a, const Vec2& b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return dx*dx + dy*dy;
}

#endif
