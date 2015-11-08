#pragma once
#include "system_base.hpp"

namespace ces
{
  struct UpdateState;
  struct PositionComponent;
  struct InputComponent;

//  struct InputSystem : public SystemBase
  struct InputSystem
  {
    InputSystem();
    bool Init();
    void Tick(const UpdateState& state);

    virtual void AddEntity(const Entity* entity);

//    struct SystemEntity
//    {
//      PositionComponent* pos;
//      InputComponent* input;
//    };
//
//    vector<SystemEntity> entities;
  };

  extern InputSystem g_InputSystem;
}
