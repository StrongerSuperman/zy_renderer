#include "win32.hpp"
#include "graphics.hpp"
#include "camera.hpp"

#include <glm/glm.hpp>


int main() {
    int width = 800;
    int height = 600;

    Win32 window;
    window.Create(width, height);

    FrameBuffer framebuffer;
    framebuffer.Create(width, height);

    Camera camera;
    auto eye = glm::vec3(0,0,10);
    auto dir = glm::vec3(0,0,-1);
    camera.SetEyeAndDir(eye,dir);

    float prev_time = window.GetTime();
    while (!window.ShouldClose()) {
        float cur_time = window.GetTime();
        float delta_time = cur_time - prev_time;

        auto view_mat = camera.GetViewMatrix();
        auto Proj_mat = camera.GetProjectionMatrix();

        window.DrawBuffer(&framebuffer);
        window.PollEvents();
    }

    window.Destroy();
    framebuffer.Release();
}
