#pragma once

#include <set>

#include "SpatialBase.h"

namespace utec {
namespace spatial {

/**
 * BasicSpatial implementation
 */
template <typename Point>
class BasicSpatial : public SpatialBase<Point> {
 private:
 public:
  struct compare_class {
    bool operator()(const Point& lhs, const Point& rhs) const {
      if (lhs.get(0) == rhs.get(0)) {
        return lhs.get(1) < rhs.get(1);
      }

      return lhs.get(0) < rhs.get(0);
    }
  };

  std::set<Point, compare_class> by_x;

  BasicSpatial(){};

  void insert(const Point& new_point) override { by_x.emplace(new_point); }

  long long get_dis(Point a, Point b) {
    return 1ll * (a.get(0) - b.get(0)) * (a.get(0) - b.get(0)) +
           1ll * (a.get(1) - b.get(1)) * (a.get(1) - b.get(1));
  }

  // El punto de referencia no necesariamente es parte del dataset
  Point nearest_neighbor(const Point& reference) override {
    typename std::set<Point, compare_class>::iterator it =
        by_x.lower_bound(reference);

    if (it != by_x.begin()) {
      it--;
    }

    long long dis = LLONG_MAX;

    Point cur_best_point;

    while (true) {
      if (1ll * (reference.get(0) - (*it).get(0)) *
              (reference.get(0) - (*it).get(0)) >
          dis) {
        break;
      }

      long long cur_dis = get_dis(reference, *it);

      if (dis > cur_dis) {
        dis = cur_dis;
        cur_best_point = *it;
      }

      if (it == by_x.begin()) {
        break;  // No hay nada a la izquierda
      }

      it--;
    }

    it = by_x.lower_bound(reference);

    while (it != by_x.end()) {
      if (1ll * (reference.get(0) - (*it).get(0)) *
              (reference.get(0) - (*it).get(0)) >
          dis) {
        break;  //
      }

      long long cur_dis = get_dis(reference, *it);

      if (dis > cur_dis) {
        dis = cur_dis;
        cur_best_point = *it;
      }

      it++;
    }

    return cur_best_point;
  }
};

}  // namespace spatial
}  // namespace utec
