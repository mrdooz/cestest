#pragma once

namespace ces
{
  struct PositionComponent;
  struct RenderComponent;
  
  struct Entity
  {

  };

  struct AlienEntity
  {
    PositionComponent* pos;
    RenderComponent* render;
  };

}