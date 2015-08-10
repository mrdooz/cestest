#include "sprite_manager.hpp"

using namespace ces;

//------------------------------------------------------------------------------
SpriteManager* SpriteManager::_instance;

//------------------------------------------------------------------------------
bool SpriteManager::Create()
{
  assert(!_instance);

  _instance = new SpriteManager();
  return _instance->Init();
}

//------------------------------------------------------------------------------
SpriteManager& SpriteManager::Instance()
{
  return *_instance;
}

//------------------------------------------------------------------------------
bool SpriteManager::Init()
{
  return true;
}

//------------------------------------------------------------------------------
u16 SpriteManager::AddSprite(const string& name,
    const string& sub,
    const Vector2& uvTopLeft,
    const Vector2& uvBottomRight,
    const Vector2& size,
    ObjectHandle handle)
{
  u16 id = (u16)_sprites.size();
  Sprite* sprite = new Sprite{uvTopLeft, uvBottomRight, size, handle, id};
  _spritesByName[name] = sprite;
  _sprites.push_back(sprite);
  return id;
}

//------------------------------------------------------------------------------
void SpriteManager::CopyOut(const Sprite* sprite, const Vector2& pos, vector<SpriteVtx>* verts, vector<u32>* indices)
{
  // 1--2
  // |  |
  // 0--3

  // CCW:
  // 0, 3, 1
  // 3, 2, 1

  int vertOfs = (int)verts->size();

  Vector2 v0 = Vector2{pos.x, pos.y + sprite->size.y};
  Vector2 v1 = Vector2{pos.x, pos.y};
  Vector2 v2 = Vector2{pos.x + sprite->size.x, pos.y};
  Vector2 v3 = Vector2{pos.x + sprite->size.x, pos.y + sprite->size.y};

  Vector2 t0{sprite->uvTopLeft.x, sprite->uvBottomRight.y};
  Vector2 t1{sprite->uvTopLeft.x, sprite->uvTopLeft.y};
  Vector2 t2{sprite->uvBottomRight.x, sprite->uvTopLeft.y};
  Vector2 t3{sprite->uvBottomRight.x, sprite->uvBottomRight.y};

  u32 c = 0xffffffff;

  verts->push_back(SpriteVtx{v0, t0, c});
  verts->push_back(SpriteVtx{v1, t1, c});
  verts->push_back(SpriteVtx{v2, t2, c});
  verts->push_back(SpriteVtx{v3, t3, c});

  indices->push_back(vertOfs + 0);
  indices->push_back(vertOfs + 3);
  indices->push_back(vertOfs + 1);

  indices->push_back(vertOfs + 3);
  indices->push_back(vertOfs + 2);
  indices->push_back(vertOfs + 1);
}

//------------------------------------------------------------------------------
void SpriteManager::CopyOut(const string& name, const Vector2& pos, vector<SpriteVtx>* verts, vector<u32>* indices)
{
  auto it = _spritesByName.find(name);
  if (it == _spritesByName.end())
    return;

  CopyOut(it->second, pos, verts, indices);
}

//------------------------------------------------------------------------------
void SpriteManager::CopyOut(u16 idx, const Vector2& pos, vector<SpriteVtx>* verts, vector<u32>* indices)
{
  assert(idx < _sprites.size());
  assert(_sprites[idx]);

  CopyOut(_sprites[idx], pos, verts, indices);
}

//------------------------------------------------------------------------------
u16 SpriteManager::GetSpriteIndex(const string& name)
{
  auto it = _spritesByName.find(name);
  if (it == _spritesByName.end())
    return INVALID_SPRITE;

  return it->second->idx;
}

