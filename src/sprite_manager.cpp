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
void SpriteManager::AddSprite(const string& name, const string& sub, const Vector2& uvTopLeft, const Vector2& uvBottomRight, const Vector2& size)
{
  _sprites[name] = Sprite{ uvTopLeft, uvBottomRight, size };
}

//------------------------------------------------------------------------------
void SpriteManager::CopyOut(const string& name, const Vector2& pos, vector<SpriteVtx>* verts, vector<u32>* indices)
{
  auto it = _sprites.find(name);
  if (it == _sprites.end())
    return;

  // 1--2
  // |  |
  // 0--3

  // CCW:
  // 0, 3, 1
  // 3, 2, 1

  const Sprite& s = it->second;
  float w = s.size.x / 2;
  float h = s.size.y / 2;

  Vector2 v0{pos.x - w, pos.y - h};
  Vector2 v1{pos.x - w, pos.y + h};
  Vector2 v2{pos.x + w, pos.y + h};
  Vector2 v3{pos.x + w, pos.y - h};

  v0 = Vector2{0, 200};
  v1 = Vector2{0, 0};
  v2 = Vector2{200, 0};
  v3 = Vector2{200, 200};

  Vector2 t0{s.uvTopLeft.x, s.uvBottomRight.y};
  Vector2 t1{s.uvTopLeft.x, s.uvTopLeft.y};
  Vector2 t2{s.uvBottomRight.x, s.uvTopLeft.y};
  Vector2 t3{s.uvBottomRight.x, s.uvBottomRight.y};

  u32 c = 0xffffffff;

  verts->push_back(SpriteVtx{v0, t0, c});
  verts->push_back(SpriteVtx{v1, t1, c});
  verts->push_back(SpriteVtx{v2, t2, c});
  verts->push_back(SpriteVtx{v3, t3, c});

  indices->push_back(0);
  indices->push_back(3);
  indices->push_back(1);

  indices->push_back(3);
  indices->push_back(2);
  indices->push_back(1);
}
