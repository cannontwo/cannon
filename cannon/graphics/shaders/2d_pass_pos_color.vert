#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;

out vec2 vertPos;
out vec3 ourColor;

uniform float pointSize; 
uniform mat4 matrix;

void main() {
  gl_Position = matrix * vec4(aPos.x, aPos.y, 0.0, 1.0);
  gl_PointSize = pointSize;

  ourColor = aColor.xyz;
}
