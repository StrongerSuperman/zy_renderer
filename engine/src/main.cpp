#include <iostream>
#include <direct.h>

#include <glm/glm.hpp>

#include "core/define.hpp"
#include "core/platform.hpp"
#include "core/userdata.hpp"
#include "platform/win32.hpp"
#include "app/my_userdata.hpp"


int main() {
    Platform* window = new Win32(WIDTH, HEIGHT);
    Userdata* userdata = new MyUserdata(window);
    window->SetUserdata(userdata);

    int num_frames = 0;
    float prev_time = window->GetTime();
    float print_time = window->GetTime();
    while (!window->ShouldClose()) {
        window->Tick();
        window->SwapFrameBuffer();
        window->PollEvents();

        float cur_time = window->GetTime();
        float delta_time = cur_time - prev_time;
        (void)(delta_time);
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

    delete window;
}
