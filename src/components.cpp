#include "components.hpp"
#include "sprite_manager.hpp"
#include "types.hpp"
#include "render_system.hpp"

using namespace ces;

RenderComponent* RenderComponent::Create(const string& spriteName, const Vector2& pos)
{
  u16 idx = SPRITE_MANAGER.GetSpriteIndex(spriteName);
  if (idx == INVALID_SPRITE)
    return nullptr;

  return g_RenderSystem.AllocComponent(idx, pos);
}
