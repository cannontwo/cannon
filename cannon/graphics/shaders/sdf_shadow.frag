#version 330 core

#define EPSILON 0.0001

struct Material {
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};

in vec3 sdfPos;
in vec3 FragPos;

out vec4 FragColor;

uniform Material material;

uniform vec4 viewPos;

uniform mat4 perspective;
uniform mat4 model;
uniform mat4 view;
uniform mat4 normalmat;

uniform float scale;
uniform float time;

uniform int max_march_steps;

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

float scene_sdf(vec3 p) {
  return solidAngleSDF(p, vec2(sin(time), cos(time)), 0.4);
}

vec3 estimate_normal(vec3 p) {
  return normalize(vec3(
    scene_sdf(vec3(p.x + EPSILON, p.y, p.z)) - scene_sdf(vec3(p.x - EPSILON, p.y, p.z)),
    scene_sdf(vec3(p.x, p.y + EPSILON, p.z)) - scene_sdf(vec3(p.x, p.y - EPSILON, p.z)),
    scene_sdf(vec3(p.x, p.y, p.z + EPSILON)) - scene_sdf(vec3(p.x, p.y, p.z - EPSILON))
  ));
}

// viewPos should be sdfPos, viewDir normalized camera view dir
float sphere_march(vec3 viewPos, vec3 viewDir, float max_depth) {
  float depth = 0.0;

  for (int i = 0; i < max_march_steps; i++) {
    float dist = scene_sdf(viewPos + depth * viewDir);

    if (dist < EPSILON) {
      return depth;
    }

    depth += dist;

    if (depth >= max_depth) {
      return max_depth;
    }
  }

  return max_depth;
}

void main() {
  float max_depth = 5.0;
  vec3 worldspace_viewDir = normalize(FragPos - viewPos.xyz);
  vec3 sdfspace_viewDir = normalize((transpose(model) * vec4(worldspace_viewDir, 0.0)).xyz);
  float sdf_depth = sphere_march(sdfPos, sdfspace_viewDir, max_depth);

  if (sdf_depth == max_depth) {
    discard;
  }

  FragColor = vec4(gl_FragCoord.z, 0.0, 0.0, 1.0);
}
