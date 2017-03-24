#pragma once

namespace octet {
  class water_surface {
    struct my_vertex {
      vec3p pos;
      uint32_t color;
    };

    ref<mesh> water_mesh;
    std::vector<vec3> water_plane;


    void init() {
      water_mesh = new mesh();

      water_plane.resize(10000);

      // allocate vertices and indices into OpenGL buffers
      size_t num_steps = 320;
      size_t num_indices = num_steps * 6;
      water_mesh->allocate(sizeof(my_vertex) * water_plane.size(), sizeof(uint32_t) * num_indices);
      water_mesh->set_params(sizeof(my_vertex), num_indices, water_plane.size(), GL_TRIANGLES, GL_UNSIGNED_INT);
      
      // describe the structure of my_vertex to OpenGL
      water_mesh->add_attribute(attribute_pos, 3, GL_FLOAT, 0);
      water_mesh->add_attribute(attribute_color, 4, GL_UNSIGNED_BYTE, 12, GL_TRUE);

      {
        // these write-only locks give access to the vertices and indices.
        // they will be released at the next } (the end of the scope)
        gl_resource::wolock vl(water_mesh->get_vertices());
        my_vertex *vtx = (my_vertex *)vl.u8();
        gl_resource::wolock il(water_mesh->get_indices());
        uint32_t *idx = il.u32();

        // make the vertices
        float radius1 = 1.0f;
        float radius2 = 7.0f;
        float height = 24.0f;
        float num_twists = 4.0f;
        for (size_t i = 0; i != num_steps + 1; ++i) {
          float y = i * (height / num_steps) - height * 0.5f;
          float angle = i * (num_twists * 2.0f * 3.14159265f / num_steps);
          vtx->pos = vec3p(cosf(angle) * radius1, y, sinf(angle) * radius1);
          vtx++;
          vtx->pos = vec3p(cosf(angle) * radius2, y, sinf(angle) * radius2);
          vtx++;
        }

        // make the triangles
        uint32_t vn = 0;
        for (size_t i = 0; i != num_steps; ++i) {
          // 0--2
          // | \|
          // 1--3
          idx[0] = vn + 0;
          idx[1] = vn + 3;
          idx[2] = vn + 1;
          idx += 3;

          idx[0] = vn + 0;
          idx[1] = vn + 2;
          idx[2] = vn + 3;
          idx += 3;

          vn += 2;
        }
      }
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