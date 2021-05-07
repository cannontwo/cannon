#include <cannon/ray/material.hpp>

using namespace cannon::ray;

bool Lambertian::scatter(const Ray& r_in, const hit_record& rec, Vector3d&
    attenuation, Ray& scattered) const {
  Vector3d scatter_direction = rec.normal + random_unit_vec();

  // Catch degenerate scatter direction
  if ((std::fabs(scatter_direction[0]) < 1e-8) &&
      (std::fabs(scatter_direction[1]) < 1e-8) &&
      (std::fabs(scatter_direction[2]) < 1e-8)) {
    scatter_direction = rec.normal;
  }

  scattered = Ray(rec.p, scatter_direction, r_in.time_);
  attenuation = albedo_->value(rec.u, rec.v, rec.p);
  return true;
}

bool Metal::scatter(const Ray& r_in, const hit_record& rec, Vector3d&
    attenuation, Ray& scattered) const {
  Vector3d reflected = reflect(r_in.dir_.normalized(), rec.normal);
  scattered = Ray(rec.p, reflected + fuzz_ * random_in_unit_sphere(), r_in.time_);
  attenuation = albedo_;
  return (scattered.dir_.dot(rec.normal) > 0);
}

bool Dielectric::scatter(const Ray& r_in, const hit_record& rec, Vector3d&
    attenuation, Ray& scattered) const {
  attenuation = Vector3d::Ones();
  double refraction_ratio = rec.front_face ? (1.0 / ir_) : ir_;
  Vector3d unit_direction = r_in.dir_.normalized();

  double cos_theta = std::fmin(-unit_direction.dot(rec.normal.normalized()), 1.0);
  double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

  // If angle is too oblique, cannot refract
  bool cannot_refract = refraction_ratio * sin_theta > 1.0;

  Vector3d direction;

  if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
    direction = reflect(unit_direction, rec.normal);
  else
    direction = refract(unit_direction, rec.normal.normalized(), refraction_ratio);

  scattered = Ray(rec.p, direction, r_in.time_);
  return true;
}

bool Isotropic::scatter(const Ray& r_in, const hit_record& rec,
    Vector3d& attenuation, Ray& scattered) const {
  scattered = Ray(rec.p, random_in_unit_sphere(), r_in.time_);
  attenuation = albedo_->value(rec.u, rec.v, rec.p);

  return true;
}

// Public Functions
Vector3d cannon::ray::reflect(const Vector3d& v, const Vector3d& n) {
  return v - 2*v.dot(n)*n;
}

Vector3d cannon::ray::refract(const Vector3d& uv, const Vector3d& n, double etai_over_etat) {
  // Assuming uv and n are unit vectors
  auto cos_theta = std::fmin(-uv.dot(n), 1.0);

  Vector3d r_out_perp = etai_over_etat * (uv + cos_theta * n);
  Vector3d r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.dot(r_out_perp))) * n;
  return r_out_perp + r_out_parallel;
}

double cannon::ray::reflectance(double cosine, double ref_idx) {
  // Schlick's approximation for reflectance
  auto r0 = (1 - ref_idx) / (1 + ref_idx);
  r0 = r0*r0;
  return r0 + (1-r0) * std::pow((1 - cosine), 5);
}

