#pragma once
#include "types.hpp"

namespace ces
{
#if 0
  enum ComponentMask
  {
    CMPosition = 1 << 0,
    CMRender = 1 << 1,
    CMInput = 1 << 2,
    CMPhysics = 1 << 3,
  };

  void CreateFreeList(int stride, int numElems, char* buf);

  template <typename T, int MAX_NUM_COMPONENTS = 1024>
  struct ComponentBase
  {
    ComponentBase()
    {
      // create the free list between the components
      CreateFreeList(sizeof(T), MAX_NUM_COMPONENTS, (char*)components);
      activeComponents[0] = INVALID_SPRITE;
    }

    template <typename... Args>
    T* AllocComponent(Args... args)
    {
      // grab the next free component, and use his "next free" for the new head
      char* ptr = &components[freeListHead * sizeof(T)];
      activeComponents[numActiveComponents] = freeListHead;
      activeComponents[++numActiveComponents] = -1;
      freeListHead = *(u16*)ptr;
      T* obj = new(ptr)T{args...};
      return obj;
    }

    void FreeComponent(T* ptr)
    {
      u16 idx = ptr - (T*)components;
      *(u16*)ptr = freeListHead;
      freeListHead = idx;

      // compact the active component list
      int* obj = &activeComponents[idx];
      memmove(obj, obj + 1, (MAX_NUM_COMPONENTS-idx-1)*sizeof(int));
      numActiveComponents--;
      activeComponents[numActiveComponents] = INVALID_SPRITE;
    }

    const T& Get(int idx)
    {
      return *(T*)&components[idx*sizeof(T)];
    }

    char components[sizeof(T) * MAX_NUM_COMPONENTS];
    int activeComponents[MAX_NUM_COMPONENTS+1];
    int numActiveComponents = 0;
    u16 freeListHead = 0;
  };

  struct PositionComponent
  {
    enum { type = ComponentMask::CMPosition };
    typedef ComponentBase<PositionComponent> Factory;
    static Factory FACTORY;
    Vector2 pos;
  };

  struct RenderComponent
  {
    enum { type = ComponentMask::CMRender };
    typedef ComponentBase<RenderComponent> Factory;
    static Factory FACTORY;
    u32 spriteIdx;
  };

  struct InputComponent
  {
    enum { type = ComponentMask::CMInput };
    typedef ComponentBase<InputComponent> Factory;
    static Factory FACTORY;
  };

  struct PhysicsComponent
  {
    enum { type = ComponentMask::CMPhysics };
    typedef ComponentBase<PhysicsComponent> Factory;
    static Factory FACTORY;
    Vector2 vel;
    Vector2 acc;
  };
#endif
}