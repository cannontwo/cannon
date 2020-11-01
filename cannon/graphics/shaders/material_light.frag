#version 330 core
struct Material {
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};

struct Light {
  vec4 position;

  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform Material material;
uniform Light light;

uniform vec4 viewPos;

void main() {
  // ambient
  vec3 ambient = light.ambient.xyz * material.ambient.xyz;

  // diffuse
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(light.position.xyz - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse.xyz * (diff * material.diffuse.xyz);

  // specular
  vec3 viewDir = normalize(viewPos.xyz - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular.xyz * (spec * material.specular.xyz);

  vec3 result = ambient + diffuse + specular;
  FragColor = vec4(result, 1.0);
}
