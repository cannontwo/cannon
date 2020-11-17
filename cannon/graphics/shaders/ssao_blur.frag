#version 330 core

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D screenTexture;

void main() {
  vec2 texelSize = 1.0 / vec2(textureSize(screenTexture, 0));

  float result = 0.0;
  for (int x = -2; x < 2; ++x) {
    for (int y = -2; y < 2; ++y) {
      vec2 offset = vec2(float(x), float(y)) * texelSize;
      result += texture(screenTexture, texCoord + offset).r;
    }
  }

  result /= 16.0;
  FragColor = vec4(result, result, result, 1.0);
}
