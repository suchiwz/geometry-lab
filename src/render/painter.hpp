#pragma once

#ifndef GEOMETRY_LAB_RENDER_PAINTER_HPP_
#define GEOMETRY_LAB_RENDER_PAINTER_HPP_

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

namespace geometry_lab {

/**
 * @brief Basic properties of a renderer, including infomation like
 *  global poss and view parameters and light.
*/
class Painter {
 public:
  /**
   * @brief Global poss of the model, the scale parameter is 
   *  eliminated.
  */
  class Model {
   public:
    /**
     * @return The madel matrix.
    */
    glm::mat4 GetModel() const {
      return glm::translate(glm::identity<glm::mat4>(), position_) * rotation_;
    }
    /// Position of the model in the scene, usually (0,0,0)
    glm::vec3 position_ = {0.0f, 0.0f, 0.0f};
    /// Rotation matrix of the model, initially the identity
    glm::mat4 rotation_ = glm::identity<glm::mat4>();
  };
  /**
   * @brief Simple implementation of of a camera.
  */
  class Camera {
   public:
    /**
     * @brief Type of projection when computing projection matrix.
    */
    enum class ProjectionType {
      kPerspective = 0,
      kOrthogonal,
    };
    /**
     * @return  The View matrix computed by lookAt() function.
    */
    glm::mat4 GetView() const {
      return glm::lookAt(position_, position_ - back_z_, up_y_);
    }
    /**
     * @return The projection matrix.
    */
    glm::mat4 GetProjection() const {
      static int w, h;
      const auto& current_window = glfwGetCurrentContext();
      glfwGetFramebufferSize(current_window, &w, &h);
      float aspect = (h == 0) ? 1.0f : (float)w / (float)h;
      switch (type_) {
        case ProjectionType::kPerspective:
          return glm::perspective(glm::radians(zoom_), aspect, z_near_, z_far_);
        case ProjectionType::kOrthogonal:
          return glm::ortho(0.0f, (float)w, 0.0f, (float)h, z_near_, z_far_);
        default:
          assert(false);
          return glm::identity<glm::mat4>();
      }
    }
    /// Type of projection, defaultly being perspective
    ProjectionType type_ = ProjectionType::kPerspective;
    /// Zoom parameter for perspective matrix
    float zoom_ = 45.0f;
    /// Cut plane near and far
    float z_far_ = 100.0f, z_near_ = 0.1f;
    /// Position of the camera, defaultly at (0,0,3)
    glm::vec3 position_ = {0.0f, 0.0f, 5.0f};
    /// Y axi of the local coordinate (FIX)
    glm::vec3 up_y_ = {0.0f, 1.0f, 0.0f};
    /// Z axi of the local coordinate (FIX)
    glm::vec3 back_z_ = {0.0f, 0.0f, 1.0f};
    /// X axi of the local coordinate (FIX)
    glm::vec3 right_x_ = {1.0f, 0.0f, 0.0f};
  };
  /**
   * @brief Simple implementation of a point light.
  */
  class PointLight {
   public:
    /// Position of the point
    glm::vec3 position_ = {0.0f, 0.0f, 3.0f};
    /// Color of the illumination.
    glm::vec3 color_ = {1.0f, 1.0f, 1.0f};
  };
  /**
   * @brief Call this function to render!
  */
  virtual void Draw() const = 0;
  /**
   * @brief Capture the change of mouse to set model_ and zoom_
  */
  void CallBack() {
    auto& io = ImGui::GetIO();
    // Left Mouse
    if (ImGui::IsMouseDown(0) && !io.WantCaptureMouse) {
      auto& d = io.MouseDelta;
      auto direction = glm::normalize(
          glm::cross(glm::vec3{0.f, 0.f, 1.f}, glm::vec3{d.x, -d.y, 0.f}));
      auto angle = sqrtf(d.x * d.x + d.y * d.y);
      if (angle > 0) {
        model_.rotation_ = (glm::rotate(glm::identity<glm::mat4>(),
                                        glm::radians(angle), direction)) *
                           model_.rotation_;
      }
    }
    // Right Mouse
    if (ImGui::IsMouseDown(1) && !io.WantCaptureMouse) {
      auto& d = io.MouseDelta;
      glm::vec3 diff =
          (d.x * camera_.right_x_ - d.y * camera_.up_y_) * move_sensitivity_;
      model_.position_ += diff;
    }
    // Wheel
    if (!io.WantCaptureMouse) {
      camera_.zoom_ -= io.MouseWheel;
      if (camera_.zoom_ > 45.f) {
        camera_.zoom_ = 45.f;
      }
      if (camera_.zoom_ < 1.f) {
        camera_.zoom_ = 1.f;
      }
    }
  }
  Painter() {}
  /// Parameter for converting mouse movement to position.
  float move_sensitivity_ = 0.001f;
  Model model_;
  Camera camera_;
  PointLight point_light_;
};

}  // namespace geometry_lab

#endif  // !GEOMETRY_LAB_RENDER_PAINTER_HPP_
