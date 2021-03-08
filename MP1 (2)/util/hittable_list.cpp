#include "hittable_list.h"

/* Determines if the ray hits any objects in the list
*	@r: ray to cast
*	@t_min: min value of t
*	@t_max: max value of t
*	@rec: hit record to store the info
*/
bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}
