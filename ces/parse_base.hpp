#pragma once

namespace ces
{
  struct InputBuffer;
  struct Color;
  struct Vector2;
  struct Vector3;
  struct Vector4;

  bool ParseBool(InputBuffer& buf, bool* res);
  bool ParseFloat(InputBuffer& buf, float* res);
  bool ParseColor(InputBuffer& buf, Color* res);
  bool ParseVec2(InputBuffer& buf, Vector2* res);
  bool ParseVec3(InputBuffer& buf, Vector3* res);
  bool ParseVec4(InputBuffer& buf, Vector4* res);
  bool ParseInt(InputBuffer& buf, int* res);
  bool ParseString(InputBuffer& buf, string* res);
  bool ParseIdentifier(InputBuffer& buf, string* res, bool colon = true);
}
