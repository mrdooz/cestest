#include "utils.hpp"
#include "path_utils.hpp"
#include "input_buffer.hpp"
#include "parse_base.hpp"
#include "parse_sprite.hpp"
#include "types.hpp"
#include "resource_manager.hpp"
#include "logging.hpp"
#include "graphics.hpp"
#include "sprite_manager.hpp"
#include "entity.hpp"
#include "components.hpp"
#include "system/render.hpp"
#include "system/input.hpp"
#include "system/physics.hpp"

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

//------------------------------------------------------------------------------
int main(int, char**)
{
  LogSinkConsole consoleLog;

  ResourceManager::Create();
  Graphics::Create();
  SpriteManager::Create();

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

  g_RenderSystem.Init();
  g_InputSystem.Init();

  vector<Entity> entities;

  // clang-format off
  for (int i = 0; i < 10; ++i)
  {
    entities.push_back(
        Entity()
        .AddComponent(PositionComponent::FACTORY.AllocComponent(Vector2{(float)i * 100, 0}), ComponentMask::CMPosition)
        .AddComponent(RenderComponent::FACTORY.AllocComponent(SPRITE_MANAGER.GetSpriteIndex("enemyBlack1")), ComponentMask::CMRender));
  }

  entities.push_back(
        Entity()
        .AddComponent(PositionComponent::FACTORY.AllocComponent(Vector2{100, 100}),ComponentMask::CMPosition)
        .AddComponent(RenderComponent::FACTORY.AllocComponent(SPRITE_MANAGER.GetSpriteIndex("playerShip1_red")), ComponentMask::CMRender)
        .AddComponent(InputComponent::FACTORY.AllocComponent(), ComponentMask::CMInput)
        .AddComponent(PhysicsComponent::FACTORY.AllocComponent(), ComponentMask::CMPhysics));

  vector<SystemBase*> systems;
  systems.push_back(&g_RenderSystem);
  systems.push_back(&g_InputSystem);
  systems.push_back(&g_PhysicsSystem);

  for (Entity& e : entities)
  {
    for (SystemBase* system : systems)
    {
      if ((e.componentMask & system->componentMask) == system->componentMask)
      {
        // the entity has all the components the system wants, so add it
        system->AddEntity(e);
      }
    }
  }

  bool show_test_window = true;
  bool show_another_window = false;

  //  RenderComponent::Create("enemyBlack1", Vector2{0,0});
  //  RenderComponent::Create("enemyBlack2", Vector2{120,0});

  // Main loop
  while (!glfwWindowShouldClose(window))
  {
    ImGuiIO& io = ImGui::GetIO();
    glfwPollEvents();
    ImGui_ImplGlfwGL3_NewFrame();

    ImVec4 clearColor = ImColor(114, 144, 154);
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);

    {
      static float f = 0.0f;
      ImGui::Text("Hello, world!");
      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
      if (ImGui::Button("Test Window"))
        show_test_window ^= 1;
      if (ImGui::Button("Another Window"))
        show_another_window ^= 1;
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
          1000.0f / ImGui::GetIO().Framerate,
          ImGui::GetIO().Framerate);
    }

    UpdateState state;
    for (SystemBase* s: systems)
    {
      s->Tick(state);
    }

    ImGui::Render();
    glfwSwapBuffers(window);
  }

  // Cleanup
  ImGui_ImplGlfwGL3_Shutdown();
  glfwTerminate();

  return 0;
}
