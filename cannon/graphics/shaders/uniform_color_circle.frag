#version 330 core

in vec2 vertPos;

out vec4 FragColor;

uniform vec4 uColor;
uniform float pointSize;

void main() {
  vec2 centered = 2.0 * gl_PointCoord - 1.0;
  float r = dot(centered, centered);

  if (r > 1.0) {
    discard;
  }

  FragColor = uColor;
}
