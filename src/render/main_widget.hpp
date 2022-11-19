#pragma once

#ifndef GEOMETRY_LAB_RENDER_MAIN_WIDGET_HPP_
#define GEOMETRY_LAB_RENDER_MAIN_WIDGET_HPP_

#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "render_config.hpp"

namespace geometry_lab {

/**
 * @brief The main window widget of the visulizaiton program.
 *  We encapsulate some of the APIs in GL and ImGui for simple
 *  start.
*/
class MainWidget {
 public:
  /**
   * @brief Initialize the GLFW and ImGui context, and make some
   *  default settings.
   * @return Success?
  */
  bool Init();
  /**
   * @brief Call this function to start a new frame in the loop.
  */
  void StartNewFrame();
  /**
   * @brief Call this function to start a drawing process.
  */
  void Render();
  /**
   * @brief Call this function to end a frame.
  */
  void EndFrame();
  /**
   * @brief Close the window and end the context.
  */
  void Close();

  MainWidget() {}
  /**
   * @return Call @c glfwWindowShouldClose().
  */
  bool should_close() const { return glfwWindowShouldClose(main_window_); }
  /// Static instance of single main widget
  static std::shared_ptr<MainWidget> instance() {
    static auto ptr = std::make_shared<MainWidget>();
    return ptr;
  }
  /// main window
  GLFWwindow* main_window_ = nullptr;

 private:
  /// Change the size of viewpot when the window size changes.
  void viewport_callback() {
    static int w, h;
    glfwGetFramebufferSize(main_window_, &w, &h);
    glViewport(0, 0, w, h);
  }
  /// Process IO inputs
  void io_callback() {
    // Esc to exit
    if (glfwGetKey(main_window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(main_window_, true);
  }
};

}  // namespace geometry_lab

#endif  // !GEOMETRY_LAB_RENDER_MAIN_WIDGET_HPP_
