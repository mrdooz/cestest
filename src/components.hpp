#pragma once
#include "types.hpp"

namespace ces
{
  struct MovementComponent
  {

  };

  struct AlientComponent
  {

  };

  struct PositionComponent
  {
    Vector2 pos;
  };

  struct RenderComponent
  {
    static RenderComponent* Create(const string& spriteName, const Vector2& pos);
    u32 spriteIdx;
    Vector2 pos;
  };
}