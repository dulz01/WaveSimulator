////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//

#include "water_surface.h"

namespace octet {
  /// Scene containing a box with octet.
  class example_waves : public app {
    // scene for drawing box
    ref<visual_scene> app_scene;
    water_surface waves;
    float time;
  public:
    /// this is called when we construct the class before everything is initialised.
    example_waves(int argc, char **argv) : app(argc, argv) {
    }

    void KeyboardInputControl() {
      if (is_key_down(key::key_esc)) {
        exit(1);
      }

      float speed = 1;
      mat4t &camera = app_scene->get_camera_instance(0)->get_node()->access_nodeToParent();
      
      // Translate camera - X-axis
      if (is_key_down('A')) { camera.translate(-speed, 0, 0); }
      if (is_key_down('D')) { camera.translate(speed, 0, 0); }

      // Translate camera - Y-axis
      if (is_key_down('W')) { camera.translate(0, speed, 0); }
      if (is_key_down('S')) { camera.translate(0, -speed, 0); }

      // Translate camera - Z-axis
      if (is_key_down('E')) { camera.translate(0, 0, -speed); }
      if (is_key_down('Q')) { camera.translate(0, 0, speed); }
      
      // Rotate camera - X-axis
      if (is_key_down('Z')) { camera.rotateX(-speed); }
      if (is_key_down('X')) { camera.rotateX(speed); }

      // Rotate camera - Y axis
      if (is_key_down('C')) { camera.rotateY(speed); }
      if (is_key_down('V')) { camera.rotateY(-speed); }

      // Set wireframe on or off
      static bool wireframe = false;
      if (is_key_going_down(' ')) {
        wireframe = !wireframe;
        if (wireframe) {
          glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
          glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
      }
    }

    /// this is called once OpenGL is initialized
    void app_init() {
      app_scene = new visual_scene();
      app_scene->create_default_camera_and_lights();
      app_scene->get_camera_instance(0)->set_far_plane(1000000.0f);
      
      mat4t &camera = app_scene->get_camera_instance(0)->get_node()->access_nodeToParent();
      camera.translate(50, 30, 100);
      camera.rotateX(-20);

      param_shader *shader = new param_shader("shaders/default.vs", "shaders/simple_color.fs");
      material *colour = new material(vec4(0, 0, 1, 1), shader);
      scene_node *node = new scene_node();
      app_scene->add_child(node);

      time = 0.0f;
      waves.init();
      mesh *water = waves.Get_Mesh();
      app_scene->add_mesh_instance(new mesh_instance(node, water, colour));
    }

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      KeyboardInputControl();
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);

      // update matrices. assume 30 fps.
      app_scene->update(1.0f/30);

      waves.AnimateWaves(time += 1.0f/30);


      // draw the scene
      app_scene->render((float)vx / vy);

      // tumble the box  (there is only one mesh instance)
      scene_node *node = app_scene->get_mesh_instance(0)->get_node();
    }
  };
}
