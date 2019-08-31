#version 130

in vec4 vPosition;
in vec4 vColor;
in vec2 vTexCoord;
in vec3 vNormal;
in vec2 vType;

out vec3 normal;
out vec4 eye;
out vec4 Color;
out vec2 tex;
out vec4 ecPos;
out float Type;

uniform float on1;
uniform float on2;

out float s1;
out float s2;

uniform mat4 uModelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 viewMatrix;

void main (void) 
{
  gl_Position = uModelViewMatrix * vPosition;
  ecPos = uModelViewMatrix * vPosition;
  Type = vType[0];
  normal = (normalMatrix * normalize(vNormal));
  eye = - (uModelViewMatrix * gl_Position);
  Color = vColor;
  tex = vTexCoord;
  s1 = on1;
  s2 = on2;
}
