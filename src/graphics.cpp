#include "graphics.hpp"
#include "resource_manager.hpp"
#include "logging.hpp"
#include "sprite_manager.hpp"

using namespace ces;

//------------------------------------------------------------------------------
Graphics* Graphics::_instance;

//------------------------------------------------------------------------------
bool Graphics::Create()
{
  assert(!_instance);
  _instance = new Graphics();
  return _instance->Init();
}

//------------------------------------------------------------------------------
Graphics& Graphics::Instance()
{
  return *_instance;
}

//------------------------------------------------------------------------------
bool Graphics::Init()
{
  return true;
}

//------------------------------------------------------------------------------
ObjectHandle Graphics::LoadTexture(const char* filename)
{
  vector<char> buf;
  if (!RESOURCE_MANAGER.LoadFile(filename, &buf))
  {
    LOG_WARN("Unable to load file: ", filename);
    return ObjectHandle();
  }

  int x, y, c;
  const u8* image = stbi_load_from_memory((const u8*)buf.data(), (int)buf.size(), &x, &y, &c, 4);
  if (!image)
    return ObjectHandle();

  GLuint texture = 0;
  GL_CHECK(glGenTextures(1, &texture));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image));

  return _resTextures.Add(graphics::Texture{texture});
}

//------------------------------------------------------------------------------
void CompileAndAttachShader(const char* buf, GLuint shader, GLuint program)
{
  GL_CHECK(glShaderSource(shader, 1, &buf, 0));
  GL_CHECK(glCompileShader(shader));
  GL_CHECK(glAttachShader(program, shader));
}

//------------------------------------------------------------------------------
ObjectHandle Graphics::LoadShaderProgram(const char* vs, const char* fs)
{
  vector<char> vsBuf;
  vector<char> fsBuf;

  if (!RESOURCE_MANAGER.LoadFileAsText(vs, &vsBuf))
  {
    LOG_WARN("Unable to load vertex shader: ", vs);
    return ObjectHandle();
  }

  if (!RESOURCE_MANAGER.LoadFileAsText(fs, &fsBuf))
  {
    LOG_WARN("Unable to load fragment shader: ", fs);
    return ObjectHandle();
  }

  graphics::VertexShader vsObj;
  graphics::FragmentShader fsObj;
  graphics::ShaderProgram program;

  const GLchar *vertex_shader =
  "#version 330\n"
  "uniform mat4 ProjMtx;\n"
  "in vec2 Position;\n"
  "in vec2 UV;\n"
  "in vec4 Color;\n"
  "out vec2 Frag_UV;\n"
  "out vec4 Frag_Color;\n"
  "void main()\n"
  "{\n"
  "	Frag_UV = UV;\n"
  "	Frag_Color = Color;\n"
  "	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
  "}\n";

  const GLchar* fragment_shader =
  "#version 330\n"
  "uniform sampler2D Texture;\n"
  "in vec2 Frag_UV;\n"
  "in vec4 Frag_Color;\n"
  "out vec4 Out_Color;\n"
  "void main()\n"
  "{\n"
  "	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
  "}\n";

  GL_CHECK(program.glHandle = glCreateProgram());
  GL_CHECK(vsObj.glHandle = glCreateShader(GL_VERTEX_SHADER));
  GL_CHECK(fsObj.glHandle = glCreateShader(GL_FRAGMENT_SHADER));
  CompileAndAttachShader(vertex_shader, vsObj.glHandle, program.glHandle);
  CompileAndAttachShader(fragment_shader, fsObj.glHandle, program.glHandle);
  GL_CHECK(glLinkProgram(program.glHandle));

  program.vs = _resVertexShaders.Add(vsObj);
  program.fs = _resFragmentShaders.Add(fsObj);

  return _resShaderPrograms.Add(program);
}

//------------------------------------------------------------------------------
ObjectHandle Graphics::CreateSpriteSheet(const SpriteSheetSettings& sheet)
{
  ObjectHandle handle = LoadTexture(sheet.filename.c_str());
  if (!handle.IsValid())
    return handle;

  float w = sheet.size.x;
  float h = sheet.size.y;

  for (const SpriteSettings& s : sheet.sprites)
  {
    // Note, pos and size are in texture coords
    SPRITE_MANAGER.AddSprite(s.name, s.sub,
                             Vector2{s.pos.x / w, s.pos.y / h},
                             Vector2{(s.pos.x + s.size.x) / w, (s.pos.y + s.size.y) / h},
                             s.pos);
  }

  return handle;
}
