#version 330 core

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D screenTexture;

uniform vec4 direction;

void main() {
  vec2 texelSize = 1.0 / vec2(textureSize(screenTexture, 0));
  vec2 texDirection = direction.xy;

  vec2 off1 = vec2(1.333333333) * texDirection * texelSize;

  float result = 0.0;
  result += texture2D(screenTexture, texCoord).r * 0.29411764705882354;
  result += texture2D(screenTexture, texCoord + off1).r * 0.35294117647058826;
  result += texture2D(screenTexture, texCoord - off1).r * 0.35294117647058826;

  FragColor = vec4(result, 0.0, 0.0, 1.0);
}
