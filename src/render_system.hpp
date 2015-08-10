#pragma once
#include "system_base.hpp"
#include "components.hpp"
#include "object_handle.hpp"
#include "update_state.hpp"

namespace ces
{
  struct RenderSystem : public SystemBase<RenderComponent>
  {
    bool Init();
    void Tick(const UpdateState& state);

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