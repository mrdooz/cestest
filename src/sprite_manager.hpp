#pragma once

#include "types.hpp"
#include "object_handle.hpp"

namespace ces
{
  struct SpriteManager
  {
    static bool Create();
    static void Destroy();

    static SpriteManager& Instance();

    u16 AddSprite(const string& name,
        const string& sub,
        const Vector2& uvTopLeft,
        const Vector2& uvBottomRight,
        const Vector2& size,
        ObjectHandle handle);

    void CopyOut(const string& name, const Vector2& pos, vector<SpriteVtx>* verts, vector<u32>* indices);
    void CopyOut(u16 idx, const Vector2& pos, vector<SpriteVtx>* verts, vector<u32>* indices);
    u16 GetSpriteIndex(const string& name);

    bool Init();

    struct Sprite
    {
      Vector2 uvTopLeft;
      Vector2 uvBottomRight;
      Vector2 size;
      ObjectHandle handle;
      u16 idx;
    };

    static SpriteManager* _instance;

    void CopyOut(const Sprite* sprite, const Vector2& pos, vector<SpriteVtx>* verts, vector<u32>* indices);

    unordered_map<string, Sprite*> _spritesByName;
    vector<Sprite*> _sprites;
  };

#define SPRITE_MANAGER SpriteManager::Instance()
}