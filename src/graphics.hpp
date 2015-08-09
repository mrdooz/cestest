#pragma once
#include "object_handle.hpp"
#include "types.hpp"

namespace ces
{
  namespace graphics
  {
    enum { MAX_RESOURCES = 4096 };

    struct Texture
    {
      enum { type = ObjectHandle::Texture };
      GLuint glHandle;
    };

    struct ArrayBuffer
    {
      enum { type = ObjectHandle::ArrayBuffer };
    };

    struct ElementArrayBuffer
    {
      enum { type = ObjectHandle::ElementArrayBuffer };
    };

    struct ShaderProgram
    {
      enum { type = ObjectHandle::ShaderProgram };

      GLuint glHandle = 0;
    };

    template <typename T>
    struct Resource
    {
      ObjectHandle Add(const T& t)
      {
        assert(count < MAX_RESOURCES);
        ObjectHandle h{(ObjectHandle::Type)T::type, count};
        data[count++] = t;
        return h;
      }

      T Get(ObjectHandle h)
      {
        return data[h.id];
      }
      
      T data[MAX_RESOURCES];
      u32 count = 0;
    };
  }

  struct SpriteSettings
  {
    string name;
    string sub;
    Vector2 pos;
    Vector2 size;
  };

  struct SpriteSheetSettings
  {
    string filename;
    Vector2 size;

    vector<SpriteSettings> sprites;
  };

  struct Graphics
  {
    static bool Create();
    static void Destroy();
    static Graphics& Instance();

    bool Init();
    ObjectHandle LoadTexture(const char* filename);
    ObjectHandle LoadShaderProgram(const char* vs, const char* fs);

    ObjectHandle CreateSpriteSheet(const SpriteSheetSettings& sheet);

    static Graphics* _instance;

    graphics::Resource<graphics::Texture> _resTextures;
    graphics::Resource<graphics::ShaderProgram> _resShaderPrograms;
  };
  
#define GRAPHICS Graphics::Instance()
}