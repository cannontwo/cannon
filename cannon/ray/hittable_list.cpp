#include <cannon/ray/hittable_list.hpp>

using namespace cannon::ray;

void HittableList::clear() {
  objects_.clear();
}

void HittableList::add(std::shared_ptr<Hittable> obj) { 
  objects_.push_back(obj);
}

bool HittableList::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
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
