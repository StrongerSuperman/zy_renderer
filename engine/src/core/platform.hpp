#ifndef PLATFORM_H
#define PLATFORM_H

#include "define.hpp"
#include "userdata.hpp"
#include "framebuffer.hpp"


class Platform;


struct Callbacks{
    void (*key_callback)(Platform *platform, KeyCode key, int pressed);
	void (*mouse_press_callback)(Platform *platform, Button button, float x, float y, int pressed);
	void (*mouse_move_callback)(Platform *platform, float x, float y);
    void (*scroll_callback)(Platform *platform, float offset);
};

static void key_callback(Platform *platform, KeyCode key, int pressed);
static void mouse_press_callback(Platform *platform, Button button, float x, float y, int pressed);
static void mouse_move_callback(Platform *platform, float x, float y);
static void scroll_callback(Platform *platform, float offset);


class Platform{
public:
    Image *m_pSurface;
    FrameBuffer *m_pFrameBuffer;
    bool m_ShouldClose;
    char m_Keys[5];
    char m_Buttons[2];
    Callbacks m_Callbacks;
    Userdata *m_pUserdata;

    void Tick(){m_pUserdata->Tick();};
    bool ShouldClose(){return this->m_ShouldClose;};
    void SetUserdata(Userdata* userdata){this->m_pUserdata=userdata;};
    void SetInputCallbacks(Callbacks callbacks){this->m_Callbacks = callbacks;};
    Userdata* GetUserdata() {return this->m_pUserdata;};
    FrameBuffer* GetFrameBuffer() {return this->m_pFrameBuffer;};

    virtual void SwapFrameBuffer()=0;
    virtual void PollEvents()=0;
    virtual int GetKeyPressed(KeyCode key)=0;
    virtual int GetButtonPressed(Button button)=0;
    virtual void GetCursor(float *xpos, float *ypos)=0;
    virtual float GetTime()=0;

    virtual ~Platform() {delete this->m_pSurface; delete this->m_pUserdata;};
protected:
    Platform(int width, int height) {
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
};


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

#endif //PLATFORM_H
