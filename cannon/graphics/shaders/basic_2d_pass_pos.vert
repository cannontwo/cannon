#version 330 core

layout (location = 0) in vec2 aPos;

out vec2 vertPos;

uniform float pointSize; 
uniform mat4 matrix;

void main() {
  gl_Position = matrix * vec4(aPos.x, aPos.y, 0.0, 1.0);
  gl_PointSize = pointSize;
}
