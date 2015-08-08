#pragma once

#include "types.hpp"

namespace ces
{
  struct SpriteManager
  {
    static bool Create();
    static void Destroy();

    static SpriteManager& Instance();

    void AddSprite(const string& name, const string& sub, const Vector2& uvTopLeft, const Vector2& uvBottomRight, const Vector2& size);
    void CopyOut(const string& name, const Vector2& pos, vector<SpriteVtx>* verts, vector<u32>* indices);

    bool Init();

    struct Sprite
    {
      Vector2 uvTopLeft;
      Vector2 uvBottomRight;
      Vector2 size;
    };

    static SpriteManager* _instance;

    unordered_map<string, Sprite> _sprites;
  };

#define SPRITE_MANAGER SpriteManager::Instance()
}