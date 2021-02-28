#include <iostream>

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
    window.SetUserdata(static_cast<void*>(&userdata));

    Callbacks callbacks;
    callbacks.key_callback = key_callback;
    callbacks.button_callback = button_callback;
    callbacks.scroll_callback = scroll_callback;
    window.SetInputCallbacks(callbacks);

    // scene
    std::string filename = "C://Users//bestr//Documents//Github//zy_renderer//assert//nanosuit//nanosuit.obj";
    Scene* scene = new BlinnScene(filename);
    scene->InitShadow(width, height);

    float prev_time = window.GetTime();
    while (!window.ShouldClose()) {
        float cur_time = window.GetTime();
        float delta_time = cur_time - prev_time;

        auto view_mat = camera->GetViewMatrix();
        auto Proj_mat = camera->GetProjectionMatrix();

        scene->Update(&perframe);
        scene->Render(&framebuffer, &perframe);

        window.DrawBuffer(&framebuffer);
        window.PollEvents();
    }
}
