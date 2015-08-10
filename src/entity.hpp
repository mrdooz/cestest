#pragma once
#include "components.hpp"

namespace ces
{
  struct PositionComponent;
  struct RenderComponent;

  struct Entity
  {
    enum { MAX_COMPONENTS = 16 };

    Entity& AddComponent(void* ptr, ComponentMask type)
    {
      components[numComponents++] = {ptr, type};
      componentMask |= (u64)type;
      return *this;
    }

    void* GetComponent(ComponentMask mask) const
    {
      for (int i = 0; i < numComponents; ++i)
      {
        if (components[i].type == mask)
          return components[i].ptr;
      }
      return nullptr;
    }

    struct
    {
      void* ptr;
      ComponentMask type;
    }
    components[MAX_COMPONENTS];
    int numComponents = 0;
    u64 componentMask = 0;
  };

}