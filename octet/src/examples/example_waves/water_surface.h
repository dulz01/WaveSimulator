#pragma once

namespace octet {
  class water_surface {
    std::vector<vec3> points;
  
  public:
    std::vector<vec3> generate_water_surface(int x, int y) {
      float tempX = 0.0f;
      float tempY = 0.0f;
      int maxRangeX = 0;
      int maxRangeY = 0;
      int screenRange = 1 - (-1);

      for (int i = 0; i < (x*y); i++) {
        tempX = i % x;
        tempY = i / x;
        maxRangeX = x - 1;
        maxRangeY = y - 1;

        tempX = (maxRangeX * -1 - 0 * 1) / maxRangeX + tempX * screenRange / maxRangeX;
        tempY = (maxRangeY * -1 - 0 * 1) / maxRangeY + tempY * screenRange / maxRangeY;

        points.push_back(vec3(tempX, tempY, 0));
      }

      return points;
    }
  };
}