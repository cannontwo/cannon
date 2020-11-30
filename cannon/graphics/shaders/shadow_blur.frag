#version 330 core

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D screenTexture;

uniform vec4 direction;

void main() {
  vec2 texelSize = 1.0 / vec2(textureSize(screenTexture, 0));
  vec2 texDirection = direction.xy;

  vec2 off1 = vec2(1.3846153846) * texDirection * texelSize;
  vec2 off2 = vec2(3.2307692308) * texDirection * texelSize;

  float result = 0.0;
  result += texture2D(screenTexture, texCoord).r * 0.2270270270;
  result += texture2D(screenTexture, texCoord + off1).r * 0.3162162162;
  result += texture2D(screenTexture, texCoord - off1).r * 0.3162162162;
  result += texture2D(screenTexture, texCoord + off2).r * 0.0702702703;
  result += texture2D(screenTexture, texCoord - off2).r * 0.0702702703;

  FragColor = vec4(result, 0.0, 0.0, 1.0);
}
