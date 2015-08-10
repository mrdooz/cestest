#include "input.hpp"
#include "logging.hpp"
#include "entity.hpp"
#include "update_state.hpp"
#include "../imgui/imgui.h"

using namespace ces;

InputSystem ces::g_InputSystem;

//------------------------------------------------------------------------------
InputSystem::InputSystem()
: SystemBase(CMPosition | CMInput)
{

}

//------------------------------------------------------------------------------
void InputSystem::AddEntity(const Entity& entity)
{
  entities.push_back(SystemEntity{
    (PositionComponent*)entity.GetComponent(CMPosition),
    (InputComponent*)entity.GetComponent(CMInput)});
}

//------------------------------------------------------------------------------
bool InputSystem::Init()
{
  return true;
}

//------------------------------------------------------------------------------
void InputSystem::Tick(const UpdateState& state)
{
  ImGuiIO& io = ImGui::GetIO();
  for (SystemEntity& e : entities)
  {
    if (io.KeysDown['A'])
    {
      e.pos->pos.x -= 1;
    }
    else if (io.KeysDown['D'])
    {
      e.pos->pos.x += 1;
    }
  }
}
