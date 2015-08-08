#include <vector>
#include <string>
#include <functional>
#include <stdint.h>
#include <unordered_map>

#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include <stdio.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

namespace ces
{
  typedef uint8_t u8;
  typedef uint16_t u16;
  typedef uint32_t u32;
  typedef uint64_t u64;

  typedef char s8;
  typedef short s16;
  typedef int s32;

  using std::vector;
  using std::string;
  using std::function;
  using std::unordered_map;
}
