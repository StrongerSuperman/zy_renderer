#include <iostream>
#include <direct.h>

#include <glm/glm.hpp>

#include "platform/win32.hpp"
#include "core/framebuffer.hpp"
#include "core/perframe.hpp"
#include "core/camera.hpp"
#include "core/userdata.hpp"
#include "blinn/blinn_scene.hpp"


static void key_callback(Platform *platform, KeyCode key, int pressed){
    auto userdata = static_cast<Userdata*>(platform->GetUserdata());
    userdata->GetCamera()->HandleKey(key, pressed);
    std::cout << "key press" << std::endl;
}

static void button_callback(Platform *platform, Button button, int pressed){
    auto userdata = static_cast<Userdata*>(platform->GetUserdata());
    float posX, posY;
    platform->GetCursor(&posX, &posY);
    userdata->GetCamera()->HandleMouseBtnPress(
        button, static_cast<int>(posX), static_cast<int>(posY), pressed);
    std::cout << "mouse button press" << std::endl;
}

static void scroll_callback(Platform *platform, float offset){
    auto userdata = static_cast<Userdata*>(platform->GetUserdata());
    userdata->GetCamera()->HandleMouseScroll(offset);
    printf("scroll/n");
    std::cout << "mouse scroll" << std::endl;
}

int main() {
    int width = 800;
    int height = 600;

    // window
    Win32 window(width, height);

    // frame buffer
    FrameBuffer framebuffer(width, height);

    // perframe
    Perframe perframe;

    Userdata userdata;
    auto camera = userdata.GetCamera();
    camera->SetAspectRatio((float)width/height);
    camera->SetEyeAndDir(glm::vec3(0,8,15), glm::vec3(0,0,-1));
    camera->SetFov(45.0f);
    window.SetUserdata(static_cast<void*>(&userdata));

    Callbacks callbacks;
    callbacks.key_callback = key_callback;
    callbacks.button_callback = button_callback;
    callbacks.scroll_callback = scroll_callback;
    window.SetInputCallbacks(callbacks);

    // scene
	const char* assert_path = "\\..\\..\\assets\\nanosuit\\nanosuit.obj";
#define MAX_PATH 256
    char buffer[MAX_PATH];
    getcwd(buffer, MAX_PATH);
	std::string filename(buffer);
	filename.append(assert_path);
    Scene* scene = new BlinnScene(filename);
    scene->InitShadow(width, height);

    int num_frames = 0;
    float prev_time = window.GetTime();
    float print_time = window.GetTime();
    while (!window.ShouldClose()) {
        auto view_mat = camera->GetViewMatrix();
        auto Proj_mat = camera->GetProjectionMatrix();

        perframe.light_dir = glm::normalize(glm::vec3(100, 100, 100));
        auto light_pos = glm::vec3(100, 100, 100);
        auto light_dir = -perframe.light_dir;
        auto right = glm::normalize(glm::cross(perframe.light_dir, glm::vec3(0, 1, 0)));
	    auto up = glm::normalize(glm::cross(right, light_dir));
        perframe.light_view_mat = glm::lookAt(light_pos, light_pos + light_dir, up);
        perframe.light_proj_mat = Proj_mat;
        perframe.camera_view_mat = view_mat;
        perframe.camera_proj_mat = Proj_mat;
        perframe.ambient_intensity = 1.0f;
        perframe.punctual_intensity = 1.0f;
        perframe.shadow_map = scene->shadow_map;

        scene->Update(&perframe);
        scene->Render(&framebuffer, &perframe);

        window.DrawBuffer(&framebuffer);
        window.PollEvents();

        float cur_time = window.GetTime();
        float delta_time = cur_time - prev_time;
        num_frames += 1;
        if (cur_time - print_time >= 1) {
            int sum_millis = (int)((cur_time - print_time) * 1000);
            int avg_millis = sum_millis / num_frames;
            printf("fps: %3d, avg: %3d ms\n", num_frames, avg_millis);
            num_frames = 0;
            print_time = cur_time;
        }
        prev_time = cur_time;
    }
}
