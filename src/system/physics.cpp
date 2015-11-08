#include "physics.hpp"
#include "logging.hpp"
#include "entity.hpp"
#include "update_state.hpp"
#include "../imgui/imgui.h"

using namespace ces;

PhysicsSystem ces::g_PhysicsSystem;

//------------------------------------------------------------------------------
PhysicsSystem::PhysicsSystem()
//: SystemBase(CMPosition | CMPhysics)
{

}

//------------------------------------------------------------------------------
void PhysicsSystem::AddEntity(const Entity* entity, const Vector2& pos)
{
  _entityPos[_numEntities] = pos;
  _entityVel[_numEntities] = Vector2{0, 0};
//  _entitySpriteIdx[_numEntities] = spriteIdx;
  _entityIdToIdx[entity->id] = _numEntities;
  _numEntities += 1;

//  entities.push_back(SystemEntity{
//    (PositionComponent*)entity->GetComponent(CMPosition),
//    (PhysicsComponent*)entity->GetComponent(CMPhysics)});
}

//------------------------------------------------------------------------------
bool PhysicsSystem::Init()
{
  return true;
}

//------------------------------------------------------------------------------
void PhysicsSystem::Tick(const UpdateState& state)
{
//  for (SystemEntity& e : entities)
//  {
//    e.pos->pos += state.delta * e.physics->vel;
//  }
}

