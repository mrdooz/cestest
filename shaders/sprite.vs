#version 330

uniform mat4 ProjMtx;
in vec2 Position;
in vec2 UV;
in vec4 Color;
out vec2 Frag_UV;

void main()
{
  Frag_UV = UV;
  gl_Position = ProjMtx * vec4(Position.xy,0,1);
}
