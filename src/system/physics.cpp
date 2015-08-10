#include "physics.hpp"
#include "logging.hpp"
#include "entity.hpp"
#include "update_state.hpp"
#include "../imgui/imgui.h"

using namespace ces;

PhysicsSystem ces::g_PhysicsSystem;

//------------------------------------------------------------------------------
PhysicsSystem::PhysicsSystem()
: SystemBase(CMPosition | CMPhysics)
{

}

//------------------------------------------------------------------------------
void PhysicsSystem::AddEntity(const Entity& entity)
{
  entities.push_back(SystemEntity{
    (PositionComponent*)entity.GetComponent(CMPosition),
    (PhysicsComponent*)entity.GetComponent(CMPhysics)});
}

//------------------------------------------------------------------------------
bool PhysicsSystem::Init()
{
  return true;
}

//------------------------------------------------------------------------------
void PhysicsSystem::Tick(const UpdateState& state)
{
}
