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
    }

    /// this is called once OpenGL is initialized
    void app_init() {
      app_scene =  new visual_scene();
      app_scene->create_default_camera_and_lights();

      material *red = new material(vec4(1, 0, 0, 1));
      mesh_box *box = new mesh_box(vec3(4));
      scene_node *node = new scene_node();
      app_scene->add_child(node);
      app_scene->add_mesh_instance(new mesh_instance(node, box, red));
    }

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      KeyboardInputControl();
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);

      // update matrices. assume 30 fps.
      app_scene->update(1.0f/30);

      // draw the scene
      app_scene->render((float)vx / vy);

      // tumble the box  (there is only one mesh instance)
      scene_node *node = app_scene->get_mesh_instance(0)->get_node();
      node->rotate(1, vec3(1, 0, 0));
      node->rotate(1, vec3(0, 1, 0));
    }
  };
}
