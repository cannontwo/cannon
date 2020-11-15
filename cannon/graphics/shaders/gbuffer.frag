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

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material;

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

void main() {
  gPosition = vec4(FragPos, 1.0);
  gNormal = vec4(normalize(Normal), 1.0);

  vec3 material_diffuse = material.diffuse.xyz + vec3(texture(material.diffuse_tex[0], TexCoords));
  float material_specular = length(material.specular.xyz / 3.0) + length(vec3(texture(material.specular_tex[0], TexCoords)) / 3.0);

  gAlbedoSpec.rgb = material_diffuse;
  gAlbedoSpec.a = material_specular;
}
