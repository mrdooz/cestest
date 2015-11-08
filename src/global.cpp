#include "global.hpp"
#include "entity.hpp"
#include "system_base.hpp"

namespace ces
{
  vector<SystemBase*> g_systems;
  vector<Entity*> g_entities;

  EventManager* g_eventManager;

  void AddEntity(Entity* e)
  {
//    for (SystemBase* system : g_systems)
//    {
//      if ((e->componentMask & system->componentMask) == system->componentMask)
//      {
//        // the entity has all the components the system wants, so add it
//        system->AddEntity(e);
//      }
//    }

    g_entities.push_back(e);
  }
}