#include "utils.hpp"

namespace ces
{
  bool LoadFile(const char* filename, bool asText, vector<char>* buf)
  {
    FILE* f = fopen(filename, "rb");
    if (!f)
      return false;

    fseek(f, 0, SEEK_END);
    u64 size = ftell(f);
    fseek(f, 0, SEEK_SET);

    buf->resize(size + (asText ? 1 : 0));
    u64 bytesRead = fread(buf->data(), 1, size, f);
    if (asText)
      (*buf)[size] = 0;

    return bytesRead == size;
  }

  void CheckOpenGLError(const char* stmt, const char* fname, int line)
  {
//    return;
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
      printf("OpenGL error %08x, at %s:%i - for %s\n", err, fname, line, stmt);
      abort();
    }
  }

}
