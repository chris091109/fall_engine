#pragma once

#include <cmath>
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <cstdint>
#include <glm/vec2.hpp>
#include "common.h"
#include <print>
#include <string>

namespace Engine {

  enum WindowMode {
    WindowModeFullscreen,
    WindowModeBorderless,
    WindowModeWindowed
  };

  class Platform 
  {
    public:
      Platform(void);
      ~Platform(void);

      f64 delta = 0;
      bool framebuffer_resized = false;

      void set_window_mode(WindowMode new_window_mode);
      bool should_close(void);
      void pollevents(void);

      GLFWwindow *get_handle(void);

      bool input_pressed(int key);
      bool input_released(int key);
      f32 input_vector2(int key_a, int key_b, f64 dt);
      vec2 input_mouse_dt();
      vec2 input_mouse_location();
      f32 get_aspect_ratio() { return (f32)m_current.width / (f32)m_current.height; }


    private:
      struct WindowState {
        int width  = 600;
        int height = 450;
        int x      = 100;
        int y      = 100;
      };

      WindowState m_windowed   = { 500, 500, 100, 100 };
      WindowState m_current    = { 600, 450, 0, 0 };

      std::string m_title = "Helix";
      WindowMode m_window_mode = WindowModeWindowed;


      GLFWwindow *m_handle     = nullptr;
      GLFWmonitor *m_monitor   = nullptr;

      f64 m_last_time = 0;
      f32 m_axis_value = 0.0f;
      f64 m_mouse_last_x = 0.0;
      f64 m_mouse_last_y = 0.0;
      bool  m_mouse_first  = true;
  };
}
