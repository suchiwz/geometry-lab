#include "main_widget.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace geometry_lab {

bool MainWidget::Init() {
  // glfw initialization
  if (!glfwInit()) {
    printf("ERROR::MainWidget::Failed to initialize glfw.\n\n");
    return false;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,
                 GEOMETRY_LAB_GLFW_CONTEXT_VERSION_MAJOR);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,
                 GEOMETRY_LAB_GLFW_CONTEXT_VERSION_MINOR);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Create the main window
  main_window_ =
      glfwCreateWindow(GEOMETRY_LAB_DEFAULT_WINDOW_WIDTH,
                       GEOMETRY_LAB_DEFAULT_WINDOW_HEIGHT, "", NULL, NULL);
  if (!main_window_) {
    printf("ERROR::MainWidget::Failed to creat main window.\n\n");
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(main_window_);
  glfwSwapInterval(1);
  glfwSetWindowPos(main_window_, GEOMETRY_LAB_DEFAULT_WINDOW_POS_X,
                   GEOMETRY_LAB_DEFAULT_WINDOW_POS_Y);
  // ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  // ImGui settings
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  // - fontsize
  float xscale, yscale;
  glfwGetWindowContentScale(main_window_, &xscale, &yscale);
  io.FontGlobalScale = xscale;
  // - style
  ImGui::StyleColorsDark();
  // - backends
  ImGui_ImplGlfw_InitForOpenGL(main_window_, true);
  ImGui_ImplOpenGL3_Init(GEOMETRY_LAB_GLSL_VERSION);
  // glad initialization
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("ERROR::MainWidget::Failed to initialize GLAD.");
    exit(-1);
  }
  // gl status
  glViewport(0, 0, GEOMETRY_LAB_DEFAULT_WINDOW_WIDTH,
             GEOMETRY_LAB_DEFAULT_WINDOW_HEIGHT);
  // - Default clear color is white
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LINE_SMOOTH);
  glLineWidth(2.0f);

  return true;
}

void MainWidget::StartNewFrame() {
  glfwPollEvents();
  viewport_callback();
  io_callback();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void MainWidget::Render() {
  ImGui::Render();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MainWidget::EndFrame() {
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  glfwSwapBuffers(main_window_);
}

void MainWidget::Close() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(main_window_);
  glfwTerminate();
}

}  // namespace geometry_lab