#version 330 core

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform float screen_width;
uniform float screen_height;

uniform vec4 samples[64];

uniform float radius;
uniform float bias;

uniform mat4 projection;
uniform mat4 view;

const int kernelSize = 64;

void main() {
  vec2 noiseScale = vec2(screen_width / 4.0, screen_height / 4.0);

  vec4 fragPos = view * texture(gPosition, texCoord);
  fragPos.xyz /= fragPos.w;
  // Compute adjustment to normal for view space by premultiplying transpose
  // inverse of view matrix
  vec3 vNormal = transpose(inverse(mat3(view))) * vec3(texture(gNormal, texCoord).xyz * 2.0 - 1.0);
  vec3 normal = normalize(vNormal);
  vec3 randomVec = normalize(vec3(texture(texNoise, texCoord * noiseScale).xyz * 2.0 - 1.0));

  vec3 tangent = normalize(randomVec - (normal * dot(randomVec, normal)));
  vec3 bitangent = normalize(cross(normal, tangent));
  mat3 TBN = mat3(tangent, bitangent, normal);

  float occlusion = 0.0;
  for (int i = 0; i < kernelSize; i++) {
    vec3 samplePos = TBN * samples[i].xyz;

    samplePos = fragPos.xyz + (samplePos * radius);
    
    vec4 offset = vec4(samplePos, 1.0);
    offset = projection * offset;
    offset.xyz /= offset.w;
    offset.xyz = (offset.xyz * 0.5) + 0.5;

    vec4 samplePoint = view * texture(gPosition, offset.xy);
    samplePoint.xyz /= samplePoint.w;
    float sampleDepth = samplePoint.z; 

    float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
    occlusion += ((sampleDepth >= (samplePos.z + bias)) ? 1.0 : 0.0) * rangeCheck; 
  }

  occlusion = 1.0 - (occlusion / kernelSize);
  FragColor = vec4(occlusion, occlusion, occlusion, 1.0);
}
