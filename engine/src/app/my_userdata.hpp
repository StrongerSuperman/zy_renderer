#pragma once

#include <direct.h>

#include <glm/glm.hpp>

#include "../core/define.hpp"
#include "../core/scene.hpp"
#include "../core/perframe.hpp"
#include "../core/userdata.hpp"
#include "../shading/blinn/blinn_scene.hpp"


class Platform;

class MyUserdata : public Userdata{
public:
    MyUserdata(Platform* owner): Userdata(owner){
        const char* assert_path = "\\..\\..\\assets\\nanosuit\\nanosuit.obj";
	    const auto max_path = 256;
        char buffer[max_path];
        _getcwd(buffer, max_path);
	    std::string filename(buffer);
	    filename.append(assert_path);

        m_Scene = new BlinnScene(filename);
        m_Scene->InitShadow(WIDTH, HEIGHT);
	    m_Scene->background = glm::vec4(0.4, 0.1, 0.3, 1.0);

        auto camera = m_Scene->GetCamera();
        camera->SetAspectRatio((float)WIDTH/HEIGHT);
        auto eye = glm::vec3(0, 8, 15);
        auto target  = glm::vec3(0, 8, 0);
	    camera->SetEyeAndTarget(eye, target);
        camera->SetFov(45.0f);
	    camera->SetKeyMoveSpeed(0.5f);
	    camera->SetMouseLeftSpeed(0.6f);
	    camera->SetMouseRightSpeed(0.05f);
	    camera->SetMouseScrollSpeed(1.5f);

        m_Perframe = new Perframe();
    }
    ~MyUserdata(){
        delete m_Scene;
        delete m_Perframe;
    }
    virtual void Tick() override{
        auto light_pos = glm::vec3(100, 100, 100);
        auto light_dir = glm::normalize(light_pos);
        auto right = glm::normalize(glm::cross(light_dir, glm::vec3(0, 1, 0)));
	    auto up = glm::normalize(glm::cross(right, -light_dir));
        auto light_view_mat = glm::lookAt(light_pos, light_pos - light_dir, up);

        auto camera = m_Scene->GetCamera();
        auto view_mat = camera->GetViewMatrix();
        auto Proj_mat = camera->GetProjectionMatrix();

        m_Perframe->light_dir = light_dir;
        m_Perframe->light_view_mat = light_view_mat;
        m_Perframe->light_proj_mat = Proj_mat;
        m_Perframe->camera_view_mat = view_mat;
        m_Perframe->camera_proj_mat = Proj_mat;
        m_Perframe->ambient_intensity = 1.0f;
        m_Perframe->punctual_intensity = 1.0f;
        m_Perframe->shadow_map = m_Scene->shadow_map;

        m_Scene->Update(m_Perframe);
        m_Scene->Render(this->GetOwner()->GetFrameBuffer(), m_Perframe);
    };
    virtual Scene* GetScene() const override{return m_Scene;};

private:
    Scene* m_Scene;
    Perframe* m_Perframe;
};
