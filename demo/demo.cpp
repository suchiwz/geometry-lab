#include <memory>
#include <string>
#include <vector>

#include <ImGuiFileDialog.h>
#include <imgui.h>
#include <core/trimesh.hpp>
#include <render/loader.hpp>
#include <render/main_widget.hpp>

using geometry_lab::TriMeshLoader;
using pTriMeshLoader = std::shared_ptr<TriMeshLoader>;
using geometry_lab::TriMesh;
// ========== Flags ==========
bool show_main_manu_bar = true;
bool show_mesh_info_menu = true;
// ========== Data  ==========
std::vector<pTriMeshLoader> meshes;
pTriMeshLoader current_mesh = nullptr;
// ========== Menus ==========
// ========== 1.MainMenuBar ==========
void NewMeshFileDialog() {
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImVec2 max_size = ImGui::GetMainViewport()->Size;
  ImVec2 min_size = {max_size.x * 0.5f, max_size.y * 0.5f};
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  if (ImGuiFileDialog::Instance()->Display(
          "Open..", ImGuiWindowFlags_NoCollapse, min_size, max_size)) {
    if (ImGuiFileDialog::Instance()->IsOk()) {
      std::string file = ImGuiFileDialog::Instance()->GetFilePathName();
      auto mesh = std::make_shared<TriMesh>();
      if (mesh->LoadFromFile(file)) {
        auto& new_loader =
            meshes.emplace_back(std::make_shared<TriMeshLoader>(file, mesh));
        new_loader->GeneratePainter();
        new_loader->InitBuffers();
        new_loader->LoadBuffers();
      }
    }
    ImGuiFileDialog::Instance()->Close();
  }
}
void DataTabBar() {
  if (ImGui::BeginTabBar("##DataTabBar")) {
    // 1. All the meshes have a tab item
    for (auto& obj : meshes) {
      std::string label = obj->label_ + "##DataTabBar";
      if (ImGui::BeginTabItem(label.c_str())) {
        // If the tab is focused, change the current_mesh
        current_mesh = obj;
        ImGui::EndTabItem();
      }
    }
    // 2. "+" item for load new mesh
    ImGuiTabItemFlags add_tabitem_flag =
        ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip;
    if (ImGui::TabItemButton("+##DataTabs", add_tabitem_flag)) {
      ImGuiFileDialog::Instance()->OpenDialog("Open..", "Choose File", ".obj",
                                              ".");
    }
    NewMeshFileDialog();
    ImGui::EndTabBar();
  }
}
void ShowMainMenuBar() {
  if (ImGui::BeginMainMenuBar()) {
    DataTabBar();
    ImGui::EndMainMenuBar();
  }
}
// ========== 2.MeshInfoMenu ==========
void MeshInfo() {
  if (current_mesh) {
    ImGui::PushID(current_mesh->label_.c_str());
    if (ImGui::CollapsingHeader("Mesh Info", NULL,
                                ImGuiTreeNodeFlags_DefaultOpen)) {
      ImGui::TextWrapped("The basic settings of Mesh\n%s",
                         current_mesh->label_.c_str());
      ImGui::Separator();
      ImGui::Text("Vertices : %lld", current_mesh->mesh_->n_vertices());
      ImGui::Text("Edges : %lld", current_mesh->mesh_->n_edges());
      ImGui::Text("Faces : %lld", current_mesh->mesh_->n_faces());
    }
    if (ImGui::CollapsingHeader("Render", NULL,
                                ImGuiTreeNodeFlags_DefaultOpen)) {
      ImGui::TextWrapped("The rendering settings of Mesh\n%s",
                         current_mesh->label_.c_str());
      ImGui::Separator();
      ImGui::InputFloat("Sensitivity",
                        &current_mesh->painter_->move_sensitivity_);
      ImGui::Separator();
      ImGui::DragFloat3("Position",
                        &current_mesh->painter_->model_.position_[0], 0.1f);
      ImGui::Separator();
      ImGui::Text("Line Mode");
      ImGui::RadioButton("Fill", (int*)&current_mesh->painter_->fill_mode_, 2);
      ImGui::SameLine();
      ImGui::RadioButton("Line", (int*)&current_mesh->painter_->fill_mode_, 1);
      ImGui::SameLine();
      ImGui::RadioButton("Both", (int*)&current_mesh->painter_->fill_mode_, 0);
    }
    ImGui::PopID();
  }
}
void ShowMeshInfoMenu() {
  const auto& viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y));
  ImGui::SetNextWindowSize(ImVec2(300, viewport->WorkSize.y));
  ImGui::SetNextWindowBgAlpha(0.5);
  ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
  ImGuiWindowFlags window_flags = 0;
  window_flags |= ImGuiWindowFlags_NoTitleBar;
  window_flags |= ImGuiWindowFlags_NoMove;
  window_flags |= ImGuiWindowFlags_NoResize;
  if (ImGui::Begin("LeftMenu", NULL, window_flags)) {
    MeshInfo();
    ImGui::End();
  }
}
// ========== Main  ==========
int main() {
  auto main_widget = geometry_lab::MainWidget::instance();
  if (!main_widget->Init()) {
    return -1;
  }
  glfwSetWindowTitle(main_widget->main_window_, "Demo window");
  while (!main_widget->should_close()) {
    main_widget->StartNewFrame();
    if (current_mesh)
      current_mesh->painter_->CallBack();
    if (show_main_manu_bar)
      ShowMainMenuBar();
    if (show_mesh_info_menu)
      ShowMeshInfoMenu();
    main_widget->Render();
    if (current_mesh)
      current_mesh->painter_->Draw();
    main_widget->EndFrame();
  }
  main_widget->Close();
  return 0;
}