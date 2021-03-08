#include <direct.h>

#include <glm/glm.hpp>

#include "../core/define.hpp"
#include "../shading/blinn/blinn_scene.hpp"
#include "my_userdata.hpp"


MyUserdata::MyUserdata(){
    char buffer[MAX_PATH_LEN];
    _getcwd(buffer, MAX_PATH_LEN);
	std::string filename(buffer);
	filename.append(ASSETS_PATH);

    this->m_Scene = new BlinnScene(filename);
    this->m_Scene->InitShadow(WIDTH, HEIGHT);
	this->m_Scene->background = glm::vec4(0.4, 0.1, 0.3, 1.0);

    auto camera = this->m_Scene->GetCamera();
    camera->SetAspectRatio((float)WIDTH/HEIGHT);
    auto eye = glm::vec3(0, 8, 15);
    auto target  = glm::vec3(0, 8, 0);
	camera->SetEyeAndTarget(eye, target);
    camera->SetFov(45.0f);
	camera->SetKeyMoveSpeed(0.5f);
	camera->SetMouseLeftSpeed(0.6f);
	camera->SetMouseRightSpeed(0.05f);
	camera->SetMouseScrollSpeed(1.5f);
};

MyUserdata::~MyUserdata(){
    delete this->m_Scene;
};

void MyUserdata::Tick() {
    auto light_pos = glm::vec3(100, 100, 100);
    auto light_dir = glm::normalize(light_pos);
    auto right = glm::normalize(glm::cross(light_dir, glm::vec3(0, 1, 0)));
    auto up = glm::normalize(glm::cross(right, -light_dir));
    auto light_view_mat = glm::lookAt(light_pos, light_pos - light_dir, up);

    auto camera = this->m_Scene->GetCamera();
    auto view_mat = camera->GetViewMatrix();
    auto Proj_mat = camera->GetProjectionMatrix();

    auto perframe = this->m_Scene->GetPerframe();
    perframe->light_dir = light_dir;
    perframe->light_view_mat = light_view_mat;
    perframe->light_proj_mat = Proj_mat;
    perframe->camera_view_mat = view_mat;
    perframe->camera_proj_mat = Proj_mat;
    perframe->ambient_intensity = 1.0f;
    perframe->punctual_intensity = 1.0f;
    perframe->shadow_map = this->m_Scene->shadow_map;

    this->m_Scene->Update();
    this->m_Scene->Render();
};