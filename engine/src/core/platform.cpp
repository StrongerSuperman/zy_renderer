
#include "define.hpp"
#include "framebuffer.hpp"
#include "userdata.hpp"
#include "platform.hpp"


static void key_callback(Platform *platform, KeyCode key, int pressed){
    auto userdata = static_cast<Userdata*>(platform->GetUserdata());
    userdata->GetScene()->GetCamera()->HandleKey(key, pressed);
}

static void mouse_press_callback(Platform *platform, Button button, float x, float y, int pressed){
    auto userdata = static_cast<Userdata*>(platform->GetUserdata());
    userdata->GetScene()->GetCamera()->HandleMouseBtnPress(
		button, static_cast<int>(x), static_cast<int>(y), pressed);
}

static void mouse_move_callback(Platform *platform, float x, float y) {
	auto userdata = static_cast<Userdata*>(platform->GetUserdata());
	userdata->GetScene()->GetCamera()->HandleMouseMove(static_cast<int>(x), static_cast<int>(y));
}

static void scroll_callback(Platform *platform, float offset){
    auto userdata = static_cast<Userdata*>(platform->GetUserdata());
    userdata->GetScene()->GetCamera()->HandleMouseScroll(offset);
}


Platform::Platform(int width, int height) {
    this->m_pSurface=nullptr,
    this->m_ShouldClose=false,
    this->m_pFrameBuffer = new FrameBuffer(width, height);

    Callbacks callbacks;
    callbacks.key_callback = key_callback;
    callbacks.mouse_press_callback = mouse_press_callback;
    callbacks.mouse_move_callback = mouse_move_callback;
    callbacks.scroll_callback = scroll_callback;
    this->SetInputCallbacks(callbacks);
};


