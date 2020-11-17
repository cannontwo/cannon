#version 330 core

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D screenTexture;
uniform float exposure;

vec3 reinhard_map(vec3 hdr) {
  return hdr / (hdr + vec3(1.0));
}

vec3 basic_exposure_map(vec3 hdr) {
  return vec3(1.0) - exp(-hdr * exposure);
}


vec3 filmic_map(vec3 x) {
  // Magic numbers from https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
  float a = 2.51;
  float b = 0.03;
  float c = 2.43;
  float d = 0.59;
  float e = 0.14;

  return clamp((x * (a*x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

void main() {
  vec3 hdrColor = texture(screenTexture, texCoord).rgb;

  //vec3 mapped = reinhard_map(hdrColor);
  //vec3 mapped = basic_exposure_map(hdrColor, exposure);
  //vec3 mapped = hdrColor;
  vec3 mapped = filmic_map(hdrColor * pow(2.0, exposure));

  FragColor = vec4(mapped, 1.0);
}
