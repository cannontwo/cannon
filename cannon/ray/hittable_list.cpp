#include <cannon/ray/hittable_list.hpp>

using namespace cannon::ray;

void HittableList::clear() {
  objects_.clear();
}

void HittableList::add(std::shared_ptr<Hittable> obj) { 
  objects_.push_back(obj);
}

bool HittableList::object_space_hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
  hit_record temp_rec;
  bool hit_anything = false;
  auto closest = t_max;

  for (const auto& obj : objects_) {
    if (obj->hit(r, t_min, closest, temp_rec)) {
      hit_anything = true;
      closest = temp_rec.t;
      rec = temp_rec;
    }
  }

  return hit_anything;
}

bool HittableList::object_space_bounding_box(double time_0, double time_1, Aabb& output_box) const {
  if (objects_.empty())
    return false;

  Aabb temp_box;
  bool first_box = true;

  for (const auto& object : objects_) {
    if (!object->bounding_box(time_0, time_1, temp_box))
      return false;

    output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
    first_box = false;
  }

  return true;
}
