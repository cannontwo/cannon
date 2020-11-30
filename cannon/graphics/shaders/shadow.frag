#version 330 core

struct DirectionalLight {
  vec4 direction;

  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};

in vec2 texCoord;

out vec4 FragColor;

uniform DirectionalLight directional_light;

uniform mat4 lightSpaceMatrix;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D shadowMap;

float calculate_shadow(vec4 fragPosLightSpace, vec3 lightDir, vec3 normal) {
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

  if (projCoords.z > 1.0)
    return 0.0;

  projCoords = projCoords * 0.5 + 0.5;
  float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
  float currentDepth = projCoords.z;

  float shadow = 0.0;
  vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
  for (int x = -1; x <= 1; x++) {
    for (int y = -1; y <= 1; y++) {
      float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
      shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
    }
  }

  shadow /= 9.0;

  //float pcfDepth = texture(shadowMap, projCoords.xy).r;
  //float shadow = currentDepth - bias > pcfDepth ? 1.0 : 0.0;
  return shadow;
}

void main() {
  vec3 FragPos = texture(gPosition, texCoord).rgb;
  vec4 FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
  vec3 Normal = texture(gNormal, texCoord).rgb;

  float shadow = calculate_shadow(FragPosLightSpace, normalize(-directional_light.direction.xyz), Normal);

  FragColor = vec4(shadow, shadow, shadow, 1.0);
}
