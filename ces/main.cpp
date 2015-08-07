#include "utils.hpp"
#include "path_utils.hpp"
#include "input_buffer.hpp"
#include "parse_base.hpp"
#include "types.hpp"

using namespace ces;

//------------------------------------------------------------------------------
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

struct Game
{
  static bool Create();

  static Game& Instance();

  int time = 0;
};

struct SpriteManager
{
  static bool Create();
  static SpriteManager& Instance();
  bool LoadSprites(const char* spriteSheet, const char* filename);

  static SpriteManager* _instance;
};

SpriteManager* SpriteManager::_instance;

bool SpriteManager::Create()
{
  assert(!_instance);
  _instance = new SpriteManager();
  return true;
}

SpriteManager& SpriteManager::Instance()
{
  return *_instance;
}

//------------------------------------------------------------------------------
bool SpriteManager::LoadSprites(const char *spriteSheet, const char* filename)
{
  FILE* f = fopen(filename, "rb");
  if (!f)
    return false;

  return true;
}

struct Sprite
{
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

struct ResourceManager
{
  static bool Create();
  static void Destroy();
  static ResourceManager& Instance();

  bool Init();
  bool LoadFile(const char* filename, vector<char>* buf);

  static ResourceManager* _instance;

  string _appRoot;
};

ResourceManager* ResourceManager::_instance;

//------------------------------------------------------------------------------
bool ResourceManager::Create()
{
  assert(!_instance);
  _instance = new ResourceManager();
  return _instance->Init();
}

bool ResourceManager::Init()
{
  _appRoot = "/Users/dooz/projects/cestest";

  return true;
}

//------------------------------------------------------------------------------
bool ResourceManager::LoadFile(const char* filename, vector<char>* buf)
{
  string fullPath = Path::Join(_appRoot, filename);
  return ces::LoadFile(fullPath.c_str(), buf);
}

//------------------------------------------------------------------------------
ResourceManager& ResourceManager::Instance()
{
  return *_instance;
}

bool SkipChar(InputBuffer& buf, char ch)
{
  buf.SkipWhitespace();
  if (!buf.Expect(ch))
    return false;
  buf.SkipWhitespace();
  return true;
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

//------------------------------------------------------------------------------
bool ParseSpriteSheet(InputBuffer& buf, SpriteSheetSettings* settings)
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
      // unknown keyword
    }
    CHECKED_OP(SkipChar(buf, ';'));
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
      // unknown keyword
      return false;
    }
    CHECKED_OP(SkipChar(buf, ';'));

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
bool ParseSpriteSheet(const char* filename)
{
  vector<char> buf;
  if (!ResourceManager::Instance().LoadFile(filename, &buf))
    return false;

  InputBuffer input(buf);

  SpriteSheetSettings sheet;

  while (!input.Eof())
  {
    input.SkipWhitespace();
    string keyword;
    CHECKED_OP(ParseIdentifier(input, &keyword, true));

    InputBuffer inner;
    if (keyword == "sprite")
    {
      input.InnerScope("{}", false, &inner);
      CHECKED_OP(ParseSingleSprite(inner, &sheet.sprites));
    }
    else if (keyword == "sheet")
    {
      input.InnerScope("{}", false, &inner);
      CHECKED_OP(ParseSpriteSheet(inner, &sheet));
    }
    else
    {
      // log
    }
    CHECKED_OP(SkipChar(input, ';'));
  }

  return true;
}

//------------------------------------------------------------------------------
int main(int, char**)
{

  ResourceManager::Create();

  ParseSpriteSheet("gfx/space_sheet1.txt");

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

    // Load Fonts
    // (see extra_fonts/README.txt for more details)
    //ImGuiIO& io = ImGui::GetIO();
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

    // Merge glyphs from multiple fonts into one (e.g. combine default font with another with Chinese glyphs, or add icons)
    //ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
    //ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/fontawesome-webfont.ttf", 18.0f, &icons_config, icons_ranges);

    bool show_test_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImColor(114, 144, 154);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        ImGuiIO& io = ImGui::GetIO();
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        // 1. Show a simple window
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
        {
            static float f = 0.0f;
            ImGui::Text("Hello, world!");
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float*)&clear_color);
            if (ImGui::Button("Test Window")) show_test_window ^= 1;
            if (ImGui::Button("Another Window")) show_another_window ^= 1;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        // 2. Show another simple window, this time using an explicit Begin/End pair
        if (show_another_window)
        {
            ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello");
            ImGui::End();
        }

        // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
        if (show_test_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
            ImGui::ShowTestWindow(&show_test_window);
        }

        // Rendering
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();

    return 0;
}
