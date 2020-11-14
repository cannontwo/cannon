#version 330 core

#define NR_DIFFUSE_TEX 8
#define NR_SPECULAR_TEX 8

struct Material {
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  sampler2D diffuse_tex[NR_DIFFUSE_TEX];
  sampler2D specular_tex[NR_SPECULAR_TEX];
  float shininess;
};

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

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform Material material;

uniform DirectionalLight directional_light;
uniform PointLight point_lights[NR_POINT_LIGHTS];
uniform Spotlight spotlights[NR_SPOTLIGHTS];

uniform int num_point_lights;
uniform int num_spotlights;

uniform vec4 viewPos;

vec3 calculate_dir_light(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 material_ambient, vec3 material_diffuse, vec3 material_specular) {
  vec3 lightDir = normalize(-light.direction.xyz);
  float diff = max(dot(normal, lightDir), 0.0);

  vec3 reflectDir = reflect(-lightDir, normal);
  vec3 halfwayDir = normalize(lightDir + viewDir);
  // Phong
  //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  // Blinn-Phong
  float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

  vec3 ambient = light.ambient.xyz * material_ambient;
  vec3 diffuse = light.diffuse.xyz * diff * material_diffuse;
  vec3 specular = light.specular.xyz * spec * material_specular;

  return (ambient + diffuse + specular);
}

vec3 calculate_point_light(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 material_ambient, vec3 material_diffuse, vec3 material_specular) {
  float distance = length(light.position.xyz - FragPos);
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
  //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  // Blinn-Phong
  float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
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
  vec3 diffuse = light.diffuse.xyz * (diff * material.diffuse.xyz);

  // specular
  vec3 reflectDir = reflect(-lightDir, normal);
  vec3 halfwayDir = normalize(lightDir + viewDir);
  // Phong
  //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  // Blinn-Phong
  float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
  vec3 specular = light.specular.xyz * spec * material.specular.xyz;

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

  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos.xyz - FragPos);

  // Only rendering first diffuse and specular tex right now
  vec3 material_ambient = material.ambient.xyz + vec3(texture(material.diffuse_tex[0], TexCoords));
  vec3 material_diffuse = material.diffuse.xyz + vec3(texture(material.diffuse_tex[0], TexCoords));
  vec3 material_specular = material.specular.xyz + vec3(texture(material.specular_tex[0], TexCoords));

  output += calculate_dir_light(directional_light, norm, viewDir, material_ambient, material_diffuse, material_specular);

  for (int i = 0; i < num_point_lights; i++) {
    output += calculate_point_light(point_lights[i], norm, FragPos, viewDir, material_ambient, material_diffuse, material_specular);
  }

  for (int i = 0; i < num_spotlights; i++) {
    output += calculate_spotlight(spotlights[i], norm, FragPos, viewDir, material_ambient, material_diffuse, material_specular);
  }

  FragColor = vec4(output, 1.0);
}
