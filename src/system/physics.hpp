#pragma once
#include "system_base.hpp"

namespace ces
{
  struct UpdateState;
  struct PositionComponent;
  struct PhysicsComponent;

//  struct PhysicsSystem : public SystemBase
  struct PhysicsSystem
  {
    PhysicsSystem();
    bool Init();
    void Tick(const UpdateState& state);

    virtual void AddEntity(const Entity* entity, const Vector2& pos);

    int _numEntities = 0;
    unordered_map<u32, u32> _entityIdToIdx;
    vector<Vector2> _entityPos;
    vector<Vector2> _entityVel;

    struct SystemEntity
    {
      PositionComponent* pos;
      PhysicsComponent* physics;
    };

    vector<SystemEntity> entities;
  };

  extern PhysicsSystem g_PhysicsSystem;
}
