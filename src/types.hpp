#pragma once

namespace ces
{
  struct Color
  {
    float r, g, b, a;
  };

  struct Vector2
  {
    float x, y;
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
