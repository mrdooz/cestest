#pragma once

namespace ces
{
  struct SystemBase;
  struct Entity;
  struct EventManager;

  extern vector<SystemBase*> g_systems;
  extern vector<Entity*> g_entities;

  extern EventManager* g_eventManager;

  void AddEntity(Entity* entity);
}