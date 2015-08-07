#include "utils.hpp"

namespace ces
{
  bool LoadFile(const char* filename, vector<char>* buf)
  {
    FILE* f = fopen(filename, "rb");
    if (!f)
      return false;

    fseek(f, 0, SEEK_END);
    u64 size = ftell(f);
    fseek(f, 0, SEEK_SET);

    buf->resize(size);
    u64 bytesRead = fread(buf->data(), 1, size, f);

    return bytesRead == size;
  }
}
