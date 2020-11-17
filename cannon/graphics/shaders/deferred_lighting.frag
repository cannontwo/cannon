#version 330 core

struct DirectionalLight {
  vec4 direction;

  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};

struct PointLight {
  vec4 position;

  vec4 ambient;
  vec4 diffuse;
  vec4 specular;

  float constant;
  float linear;
  float quadratic;
};
#define NR_POINT_LIGHTS 16

struct Spotlight {
  vec4 position;
  vec4 direction;
  float cutoff;
  float outer_cutoff;

  vec4 ambient;
  vec4 diffuse;
  vec4 specular;

  float constant;
  float linear;
  float quadratic;
};
#define NR_SPOTLIGHTS 16

in vec2 texCoord;

out vec4 FragColor;

uniform DirectionalLight directional_light;
uniform PointLight point_lights[NR_POINT_LIGHTS];
uniform Spotlight spotlights[NR_SPOTLIGHTS];

uniform int num_point_lights;
uniform int num_spotlights;

uniform vec4 viewPos;

uniform float shininess;

uniform bool enable_ssao;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D ssao;

vec3 calculate_dir_light(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 material_ambient, vec3 material_diffuse, vec3 material_specular) {
  vec3 lightDir = normalize(-light.direction.xyz);
  float diff = max(dot(normal, lightDir), 0.0);

  vec3 reflectDir = reflect(-lightDir, normal);
  vec3 halfwayDir = normalize(lightDir + viewDir);
  // Phong
  //float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
  // Blinn-Phong
  float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

  vec3 ambient = light.ambient.xyz * material_ambient;
  vec3 diffuse = light.diffuse.xyz * diff * material_diffuse;
  vec3 specular = light.specular.xyz * spec * material_specular;

  return (ambient + diffuse + specular);
}

vec3 calculate_point_light(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 material_ambient, vec3 material_diffuse, vec3 material_specular) {
  float distance = length(light.position.xyz - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

  // ambient
  vec3 ambient = light.ambient.xyz * material_ambient;

  // diffuse
  vec3 lightDir = normalize(light.position.xyz - fragPos);
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = light.diffuse.xyz * (diff * material_diffuse);

  // specular
  vec3 reflectDir = reflect(-lightDir, normal);
  vec3 halfwayDir = normalize(lightDir + viewDir);
  // Phong
  //float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
  // Blinn-Phong
  float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
  vec3 specular = light.specular.xyz * (spec * material_specular);

  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;

  return (ambient + diffuse + specular);
}

vec3 calculate_spotlight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 material_ambient, vec3 material_diffuse, vec3 material_specular) {
  vec3 lightDir = normalize(light.position.xyz - fragPos);
  float theta = dot(lightDir, normalize(-light.direction.xyz));
  float epsilon = light.cutoff - light.outer_cutoff;
  float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);

  // ambient
  vec3 ambient = light.ambient.xyz * material_ambient;

  // diffuse
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = light.diffuse.xyz * (diff * material_diffuse);

  // specular
  vec3 reflectDir = reflect(-lightDir, normal);
  vec3 halfwayDir = normalize(lightDir + viewDir);
  // Phong
  //float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

  // Blinn-Phong
  float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
  vec3 specular = light.specular.xyz * spec * material_specular;

  // attenuation
  float distance = length(light.position.xyz - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

  ambient *= intensity * attenuation;
  diffuse *= intensity * attenuation;
  specular *= intensity * attenuation;

  return (ambient + diffuse + specular);
}

void main() {
  vec3 output = vec3(0.0);

  vec3 FragPos = texture(gPosition, texCoord).rgb;
  vec3 Normal = texture(gNormal, texCoord).rgb;
  vec3 Albedo = texture(gAlbedoSpec, texCoord).rgb;
  float Specular = texture(gAlbedoSpec, texCoord).a;

  float occlusion = texture(ssao, texCoord).r;
  if (!enable_ssao)
    occlusion = 1.0;

  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos.xyz - FragPos);

  // Only rendering first diffuse and specular tex right now
  vec3 material_ambient = Albedo * occlusion;
  vec3 material_diffuse = Albedo;
  vec3 material_specular = vec3(Specular);

  output += calculate_dir_light(directional_light, norm, viewDir, material_ambient, material_diffuse, material_specular);

  for (int i = 0; i < num_point_lights; i++) {
    output += calculate_point_light(point_lights[i], norm, FragPos, viewDir, material_ambient, material_diffuse, material_specular);
  }

  for (int i = 0; i < num_spotlights; i++) {
    output += calculate_spotlight(spotlights[i], norm, FragPos, viewDir, material_ambient, material_diffuse, material_specular);
  }

  FragColor = vec4(output, 1.0);
}
