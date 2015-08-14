#pragma once

namespace ces
{
  struct SystemBase;
  struct Entity;

  extern vector<SystemBase*> g_systems;
  extern vector<Entity*> g_entities;

  void AddEntity(Entity* entity);
}