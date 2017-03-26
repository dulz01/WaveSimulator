#pragma once

namespace octet {
  class water_surface {
    struct my_vertex {
      vec3p pos;
      uint32_t color;
    };

    // this function converts three floats into a RGBA 8 bit color
    static uint32_t make_color(float r, float g, float b) {
      return 0xff000000 + ((int)(r*255.0f) << 0) + ((int)(g*255.0f) << 8) + ((int)(b*255.0f) << 16);
    }

    ref<mesh> water_mesh;
    std::vector<vec3p> water_plane;
    std::vector<uint32_t> indices;

    void init(int xSize = 100, int zSize = 100) {
      water_mesh = new mesh();

      water_plane.resize(xSize*zSize);
      indices.resize((xSize - 1) * (zSize - 1) * 6);

      // allocate vertices and indices into OpenGL buffers
      water_mesh->allocate(sizeof(my_vertex) * water_plane.size(), sizeof(uint32_t) * indices.size());
      water_mesh->set_params(sizeof(my_vertex), indices.size(), water_plane.size(), GL_TRIANGLES, GL_UNSIGNED_INT);

      // describe the structure of my_vertex to OpenGL
      water_mesh->add_attribute(attribute_pos, 3, GL_FLOAT, 0);
      water_mesh->add_attribute(attribute_color, 4, GL_UNSIGNED_BYTE, 12, GL_TRUE);

      // these write-only locks give access to the vertices and indices.
      // they will be released at the next } (the end of the scope)
      gl_resource::wolock vl(water_mesh->get_vertices());
      my_vertex *vtx = (my_vertex *)vl.u8();

      // generating a flat plane
      for (int i = 0; i < water_plane.size(); ++i) {
        float r = 0.0f, g = 1.0f * i / water_plane.size(), b = 1.0f;

        water_plane[i] = vec3p(i % xSize, -1, i / xSize);
        vtx->pos = water_plane[i];
        vtx->color = make_color(r, g, b);
        vtx++;
      }

      int i = 0;
      for (int z = 0, t = 0; z < zSize - 1; ++z) {
        for (int x = 0; x < xSize - 1; ++x) {
          indices[i] = t;
          indices[i + 1] = t + xSize;
          indices[i + 2] = t + xSize + 1;

          indices[i + 3] = t;
          indices[i + 4] = t + xSize + 1;
          indices[i + 5] = t + 1;
          i += 6;
          t++;
        }
        t++;
      }
      gl_resource::wolock il(water_mesh->get_indices());
      uint32_t *indx = il.u32();
      memcpy(indx, indices.data(), sizeof(uint32_t)*indices.size());
    }

  public:
    water_surface() {
      init();
    }

    ref<mesh> Get_Mesh() {
      return water_mesh;
    }

  };
}