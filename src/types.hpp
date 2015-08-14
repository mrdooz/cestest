#pragma once

namespace ces
{
  const static u16 INVALID_SPRITE = 0xffff;

  struct Color
  {
    float r, g, b, a;
  };

  struct Vector2
  {
    float x, y;

    friend Vector2 operator*(float s, const Vector2& v)
    {
      return Vector2{s * v.x, s * v.y};
    }

    Vector2 operator+=(const Vector2& rhs)
    {
      x += rhs.x;
      y += rhs.y;
      return *this;
    }
  };

  struct Vector3
  {
    float x, y, z;
  };

  struct Vector4
  {
    float x, y, z, w;
  };

  struct SpriteVtx
  {
    Vector2 pos;
    Vector2 uv;
    u32 col;
  };
}
