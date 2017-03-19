#pragma once

namespace octet {
  class water_surface {
    std::vector<vec3> points;
  
  public:
    std::vector<vec3> generate_water_surface(int x, int y) {
      for (int i = 0; i < (x*y); i++) {
        points.push_back(vec3(i % x, i / x, 0));
      }

      return points;
    }
  };
}