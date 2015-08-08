#pragma once

namespace ces
{
  bool LoadFile(const char* filename, bool asText, vector<char>* buf);

  void CheckOpenGLError(const char* stmt, const char* fname, int line);

#define GL_CHECK(stmt) do { \
  stmt; \
  CheckOpenGLError(#stmt, __FILE__, __LINE__); \
  } while (0)
}

