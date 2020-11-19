// Helper Functions

float ndot(vec2 a, vec2 b) {
  return a.x*b.x - a.y*b.y;
}

// Primitives (mostly from
// https://iquilezles.org/www/articles/distfunctions/distfunctions.htm)

float sphereSDF(vec3 p, float radius) {
  return length(p) - radius;
}

float boxSDF(vec3 p, vec3 extents) {
  vec3 q = abs(p) - extents;

  return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float boundingBoxSDF(vec3 p, vec3 extents, vec3 border) {
  p = abs(p) - border;
  vec3 q = abs(p + extents) - extents;

  return min(min(
    length(max(vec3(p.x, q.y, q.z), 0.0)) + min(max(p.x, max(q.y, q.z)), 0.0),
    length(max(vec3(q.x, p.y, q.z), 0.0)) + min(max(q.x, max(p.y, q.z)), 0.0)),
    length(max(vec3(q.x, q.y, p.z), 0.0)) + min(max(q.x, max(q.y, p.z)), 0.0));
}

float torusSDF(vec3 p, vec2 radii) {
  vec2 q = vec2(length(p.xz) - radii.x, p.y);
  return length(q) - radii.y;
}

float solidAngleSDF(vec3 p, vec2 sincos, float radius) {
  vec2 q = vec2(length(p.xz), p.y);
  float l = length(q) - radius;
  float m = length(q - sincos * clamp(dot(q, sincos), 0.0, radius));
  return max(l, m*sign(sincos.y*q.x - sincos.x*q.y));
}

// SDF Unary Operations

// Should be applied before primitive
vec3 opElongate(vec3 p, vec3 h) {
  return p - clamp(p, -h, h); 
}

vec3 opSymX(vec3 p) {
  return vec3(abs(p.x), p.y, p.z);
}

vec3 opSymXZ(vec3 p) {
  return vec3(abs(p.x), p.y, abs(p.z));
}

vec3 opRep(vec3 p, vec3 offset) {
  return mod(p + 0.5*offset, offset) - 0.5 * offset;
}

// Should be applied after primitive
float opRound(float d, float h) {
  return d - h;
}

float opOnion(float d, float thickness) {
  return abs(d) - thickness;
}

// SDF Binary Operations

float opUnion(float d1, float d2) {
  return min(d1, d2);
}

float opSmoothUnion(float d1, float d2, float k) {
  float h = clamp(0.5 + 0.5 * (d2 - d1)/k, 0.0, 1.0);
  return mix(d2, d1, h) - k * h * (1.0 - h);
}

float opSubtraction(float d1, float d2) {
  return max(-d1, d2);
}

float opSmoothSubtraction(float d1, float d2, float k) {
  float h = clamp(0.5 - 0.5 * (d2 + d1)/k, 0.0, 1.0);
  return mix(d2, -d1, h) + k * h * (1.0 - h);
}

float opIntersection(float d1, float d2) {
  return max(d1, d2);
}

float opSmoothIntersection(float d1, float d2, float k) {
  float h = clamp(0.5 - 0.5 * (d2 - d1)/k, 0.0, 1.0);
  return mix(d2, d1, h) + k * h * (1.0 - h);
}
