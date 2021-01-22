#include "win32.hpp"
#include "graphics.hpp"


int main(int argc, char *argv[]) {
    int width = 800;
    int height = 600;

    Win32 window;
    window.Create(width, height);
    FrameBuffer *framebuffer = framebuffer_create(width, height);

    float prev_time = window.GetTime();
    while (!window.ShouldClose()) {
        float cur_time = window.GetTime();
        float delta_time = cur_time - prev_time;

        window.DrawBuffer(framebuffer);
        window.PollEvents();
    }

    window.Destroy();
    framebuffer_release(framebuffer);
}
