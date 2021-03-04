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
}

static void mouse_press_callback(Platform *platform, Button button, float x, float y, int pressed){
    auto userdata = static_cast<Userdata*>(platform->GetUserdata());
    userdata->GetCamera()->HandleMouseBtnPress(
		button, static_cast<int>(x), static_cast<int>(y), pressed);
}

static void mouse_move_callback(Platform *platform, float x, float y) {
	auto userdata = static_cast<Userdata*>(platform->GetUserdata());
	userdata->GetCamera()->HandleMouseMove(static_cast<int>(x), static_cast<int>(y));
}

static void scroll_callback(Platform *platform, float offset){
    auto userdata = static_cast<Userdata*>(platform->GetUserdata());
    userdata->GetCamera()->HandleMouseScroll(offset);
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

    auto camera = window.GetUserdata()->GetCamera();
    camera->SetAspectRatio((float)width/height);
	camera->SetEyeAndTarget(glm::vec3(0, 8, 15), glm::vec3(0, 8, 0));
    camera->SetFov(45.0f);
	camera->SetKeyMoveSpeed(0.5f);
	camera->SetMouseLeftSpeed(0.6f);
	camera->SetMouseRightSpeed(0.05f);
	camera->SetMouseScrollSpeed(1.5f);

    Callbacks callbacks;
    callbacks.key_callback = key_callback;
	callbacks.mouse_press_callback = mouse_press_callback;
	callbacks.mouse_move_callback = mouse_move_callback;
    callbacks.scroll_callback = scroll_callback;
    window.SetInputCallbacks(callbacks);

    // scene
	const char* assert_path = "\\..\\..\\assets\\nanosuit\\nanosuit.obj";
	const auto max_path = 256;
    char buffer[max_path];
    getcwd(buffer, max_path);
	std::string filename(buffer);
	filename.append(assert_path);
    Scene* scene = new BlinnScene(filename);
    scene->InitShadow(width, height);
	scene->background = glm::vec4(0.4, 0.1, 0.3, 1.0);

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
