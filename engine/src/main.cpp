#include <glm/glm.hpp>

#include "platform/win32.hpp"
#include "core/framebuffer.hpp"
#include "core/perframe.hpp"
#include "core/camera.hpp"
#include "blinn/blinn_scene.hpp"


int main() {
    int width = 800;
    int height = 600;

    // window
    Win32 window(width, height);

    // frame buffer
    FrameBuffer framebuffer(width, height);

    // perframe
    Perframe perframe;

    // camera
    Camera camera;
    auto eye = glm::vec3(0,0,10);
    auto dir = glm::vec3(0,0,-1);
    camera.SetEyeAndDir(eye,dir);
    camera.SetFov(45.0f);
    camera.SetAspectRatio((float)width/height);

    // scene
    std::string filename = "D://G94//github//zy_renderer//assert//nanosuit//nanosuit.obj";
    Scene* scene = new BlinnScene(filename);
    scene->InitShadow(width, height);

    float prev_time = window.GetTime();
    while (!window.ShouldClose()) {
        float cur_time = window.GetTime();
        float delta_time = cur_time - prev_time;

        auto view_mat = camera.GetViewMatrix();
        auto Proj_mat = camera.GetProjectionMatrix();

        scene->Update(&perframe);
        scene->Render(&framebuffer, &perframe);

        window.DrawBuffer(&framebuffer);
        window.PollEvents();
    }
}
