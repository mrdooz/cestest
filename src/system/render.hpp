#pragma once
#include "system_base.hpp"
#include "components.hpp"
#include "object_handle.hpp"
#include "update_state.hpp"

namespace ces
{
  struct Entity2;
//  struct RenderSystem : public SystemBase
  struct RenderSystem
  {
    RenderSystem();
    bool Init();
    void Tick(const UpdateState& state);

    virtual void AddEntity(const Entity2* entity, const Vector2& pos, u32 spriteIdx);

    int _numEntities = 0;
    unordered_map<u32, u32> _entityIdToIdx;
    vector<Vector2> _entityPos;
    vector<u32> _entitySpriteIdx;

//    struct SystemEntity
//    {
//      PositionComponent* pos;
//      RenderComponent* render;
//    };

//    vector<SystemEntity> entities;

    GLuint vaoHandle;
    GLuint vboHandle;
    GLuint elementsHandle;
    ObjectHandle textureHandle;
    
    int glShaderProgram = 0;
    int attribLocationTex = 0;
    int attribLocationProjMtx = 0;
    int attribLocationPosition = 0;
    int attribLocationUV = 0;
    int attribLocationColor = 0;
  };
  
  extern RenderSystem g_RenderSystem;
}