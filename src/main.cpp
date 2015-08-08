#include "utils.hpp"
#include "path_utils.hpp"
#include "input_buffer.hpp"
#include "parse_base.hpp"
#include "types.hpp"
#include "resource_manager.hpp"
#include "logging.hpp"
#include "graphics.hpp"
#include "sprite_manager.hpp"

using namespace ces;

//------------------------------------------------------------------------------
static void error_callback(int error, const char* description)
{
  LOG_ERROR("Error: ", error, " ", description);
}

struct Game
{
  static bool Create();

  static Game& Instance();

  int time = 0;
};

struct Sprite
{
  int x, y;
  int width, height;
};

struct SpriteInfo
{
  int speed = 1;
  int numAnimations = 1;
  vector<int> sprites;
};


struct RenderComponent
{

  u32 spriteIdx;
};

struct MovementComponent
{

};

struct AlientComponent
{

};

struct PositionComponent
{
  float x, y;
};

struct Entity
{

};

struct AlienEntity
{

};

struct MovementSystem
{
};

//------------------------------------------------------------------------------
bool SkipToken(InputBuffer& buf, char token)
{
  buf.SkipWhitespace();
  if (!buf.Expect(token))
    return false;
  buf.SkipWhitespace();
  return true;
}

//------------------------------------------------------------------------------
bool ParseSpriteSheetSettings(InputBuffer& buf, SpriteSheetSettings* settings)
{
  buf.SkipWhitespace();
  while (!buf.Eof())
  {
    string keyword;
    CHECKED_OP(ParseIdentifier(buf, &keyword, true));
    buf.SkipWhitespace();

    if (keyword == "filename")
    {
      CHECKED_OP(ParseIdentifier(buf, &settings->filename, false));
    }
    else if (keyword == "size")
    {
      CHECKED_OP(ParseVec2(buf, &settings->size));
    }
    else
    {
      LOG_ERROR("Unknown token while parsing sprite sheet: ", keyword);
      return false;
    }
    CHECKED_OP(SkipToken(buf, ';'));
  }

  return true;
}

//------------------------------------------------------------------------------
bool ParseSingleSprite(InputBuffer& buf, vector<SpriteSettings>* sprites)
{
  string name;
  string sub;
  Vector2 offset = Vector2{0, 0};
  Vector2 size = Vector2{0, 0};
  int repeat = 1;
  int spacing = 0;

  buf.SkipWhitespace();
  while (!buf.Eof())
  {
    string keyword;
    CHECKED_OP(ParseIdentifier(buf, &keyword, true));
    buf.SkipWhitespace();

    if (keyword == "name")
    {
      CHECKED_OP(ParseIdentifier(buf, &name, false));
    }
    else if (keyword == "sub")
    {
      CHECKED_OP(ParseIdentifier(buf, &sub, false));
    }
    else if (keyword == "offset")
    {
      CHECKED_OP(ParseVec2(buf, &offset));
    }
    else if (keyword == "size")
    {
      CHECKED_OP(ParseVec2(buf, &size));
    }
    else if (keyword == "repeat")
    {
      CHECKED_OP(ParseInt(buf, &repeat));
    }
    else if (keyword == "spacing")
    {
      CHECKED_OP(ParseInt(buf, &spacing));
    }
    else
    {
      LOG_ERROR("Unknown token while parsing sprite: ", keyword);
      return false;
    }
    CHECKED_OP(SkipToken(buf, ';'));

  }

  // create the sprite settings from the input
  Vector2 pos = offset;
  for (int i = 0; i < repeat; ++i)
  {
    sprites->push_back(SpriteSettings{name, sub, pos, size});
    pos.x += size.x + spacing;
    pos.y += size.y + spacing;
  }

  return true;
}

//------------------------------------------------------------------------------
bool ParseSpriteSheet(const char* filename, SpriteSheetSettings* sheet)
{
  vector<char> buf;
  if (!ResourceManager::Instance().LoadFile(filename, &buf))
    return false;

  InputBuffer input(buf);

  while (!input.Eof())
  {
    input.SkipWhitespace();
    string keyword;
    CHECKED_OP(ParseIdentifier(input, &keyword, true));

    InputBuffer inner;
    if (keyword == "sprite")
    {
      input.InnerScope("{}", false, &inner);
      CHECKED_OP(ParseSingleSprite(inner, &sheet->sprites));
    }
    else if (keyword == "sheet")
    {
      input.InnerScope("{}", false, &inner);
      CHECKED_OP(ParseSpriteSheetSettings(inner, sheet));
    }
    else
    {
      LOG_ERROR("Unknown token while parsing sprite sheet: ", keyword);
    }
    CHECKED_OP(SkipToken(input, ';'));
  }

  return true;
}

//------------------------------------------------------------------------------
int main(int, char**)
{
  LogSinkConsole consoleLog;

  ResourceManager::Create();
  Graphics::Create();
  SpriteManager::Create();

  //  int NUM_ALIENS = 10;

  //  AlienEntity* aliens[NUM_ALIENS];
  for (int i = 0; i < 10; ++i)
  {
    //    aliens[i] = new AlienEntity(
  }
  // Setup window
  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
    exit(1);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui OpenGL3 example", NULL, NULL);
  glfwMakeContextCurrent(window);
  gl3wInit();

  // Setup ImGui binding
  ImGui_ImplGlfwGL3_Init(window, true);
  ImGui_ImplGlfwGL3_CreateDeviceObjects();

  ImVec4 clear_color = ImColor(114, 144, 154);

  bool show_test_window = true;
  bool show_another_window = false;

  ObjectHandle textureHandle, shaderProgram;

  GLuint vboHandle = 0;
  GLuint elementsHandle = 0;
  GLuint vaoHandle = 0;

  int glShaderProgram = 0;

  SpriteSheetSettings sheet;
  ParseSpriteSheet("gfx/space_sheet1.txt", &sheet);
  textureHandle = GRAPHICS.CreateSpriteSheet(sheet);
  shaderProgram = GRAPHICS.LoadShaderProgram("shaders/sprite.vs", "shaders/sprite.fs");
  glShaderProgram = GRAPHICS._resShaderPrograms.Get(shaderProgram).glHandle;

  int attribLocationTex; GL_CHECK(attribLocationTex = glGetUniformLocation(glShaderProgram, "Texture"));
  int attribLocationProjMtx; GL_CHECK(attribLocationProjMtx = glGetUniformLocation(glShaderProgram, "ProjMtx"));
  int attribLocationPosition; GL_CHECK(attribLocationPosition = glGetAttribLocation(glShaderProgram, "Position"));
  int attribLocationUV; GL_CHECK(attribLocationUV = glGetAttribLocation(glShaderProgram, "UV"));
  int attribLocationColor; GL_CHECK(attribLocationColor = glGetAttribLocation(glShaderProgram, "Color"));

  glGenBuffers(1, &vboHandle);
  glGenBuffers(1, &elementsHandle);

  glGenVertexArrays(1, &vaoHandle);
  glBindVertexArray(vaoHandle);
  glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
  glEnableVertexAttribArray(attribLocationPosition);
  glEnableVertexAttribArray(attribLocationUV);
  glEnableVertexAttribArray(attribLocationColor);

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
  glVertexAttribPointer(attribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVtx), (GLvoid*)OFFSETOF(SpriteVtx, pos));
  glVertexAttribPointer(attribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVtx), (GLvoid*)OFFSETOF(SpriteVtx, uv));
  glVertexAttribPointer(attribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(SpriteVtx), (GLvoid*)OFFSETOF(SpriteVtx, col));
#undef OFFSETOF

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Main loop
  while (!glfwWindowShouldClose(window))
  {
    ImGuiIO& io = ImGui::GetIO();
    glfwPollEvents();
    ImGui_ImplGlfwGL3_NewFrame();

    {
      static float f = 0.0f;
      ImGui::Text("Hello, world!");
      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
      ImGui::ColorEdit3("clear color", (float*)&clear_color);
      if (ImGui::Button("Test Window")) show_test_window ^= 1;
      if (ImGui::Button("Another Window")) show_another_window ^= 1;
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    const float width = ImGui::GetIO().DisplaySize.x;
    const float height = ImGui::GetIO().DisplaySize.y;
    const float ortho_projection[4][4] =
    {
      { 2.0f/width,	0.0f,			0.0f,		0.0f },
      { 0.0f,			2.0f/-height,	0.0f,		0.0f },
      { 0.0f,			0.0f,			-1.0f,		0.0f },
      { -1.0f,		1.0f,			0.0f,		1.0f },
    };

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);

    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(glShaderProgram);
    glUniform1i(attribLocationTex, 0);
    glUniformMatrix4fv(attribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);

    glBindVertexArray(vaoHandle);

    vector<SpriteVtx> verts;
    vector<u32> indices;
    SPRITE_MANAGER.CopyOut("wingYellow_7", Vector2{0,0}, &verts, &indices);

    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vboHandle));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)verts.size() * sizeof(SpriteVtx), (GLvoid*)verts.data(), GL_STREAM_DRAW));

    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsHandle));
    GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)indices.size() * sizeof(u32), (GLvoid*)indices.data(), GL_STREAM_DRAW));

    GL_CHECK(glBindTexture(GL_TEXTURE_2D, GRAPHICS._resTextures.Get(textureHandle).glHandle));
    GL_CHECK(glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0));

    ImGui::Render();
    glfwSwapBuffers(window);
  }

  // Cleanup
  ImGui_ImplGlfwGL3_Shutdown();
  glfwTerminate();

  return 0;
}
