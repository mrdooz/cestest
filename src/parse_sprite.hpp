#pragma once

#include "types.hpp"

namespace ces
{
  struct SpriteSettings
  {
    string name;
    string sub;
    Vector2 pos;
    Vector2 size;
  };

  struct SpriteSheetSettings
  {
    string filename;
    Vector2 size;

    vector<SpriteSettings> sprites;
  };

  bool ParseSpriteSheet(const char* filename, SpriteSheetSettings* sheet);
}