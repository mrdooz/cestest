#pragma once

namespace ces
{
  struct ObjectHandle
  {
    enum Type
    {
      Invalid = 0,
      Texture,

      ArrayBuffer,
      ElementArrayBuffer,

      ShaderProgram,
      VertexShader,
      FragmentShader,
    };

    ObjectHandle() : raw(0) {}
    ObjectHandle(Type type, u32 id, u32 data = 0) : type(type), id(id), data(data) {}

    union {
      u32 raw = 0;
      struct {
        u32 type : 8;
        u32 id : 16;
        u32 data : 8;
      };
    };

    bool IsValid() const { return raw != 0; }
  };

  static_assert(sizeof(ObjectHandle) == sizeof(u32), "ObjectHandle too large");
}