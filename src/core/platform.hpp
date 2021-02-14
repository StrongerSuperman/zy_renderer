#pragma once

#include "framebuffer.hpp"


enum KeyCode{
    KEY_A,
    KEY_D,
    KEY_S,
    KEY_W,
    KEY_SPACE
};

enum Button{
    BUTTON_L,
    BUTTON_R
};

class Platform;

struct Callbacks{
    void (*key_callback)(Platform *platform, KeyCode key, int pressed);
    void (*button_callback)(Platform *platform, Button button, int pressed);
    void (*scroll_callback)(Platform *platform, float offset);
};

class Platform{
public:
    Platform(int width, int height);
    virtual ~Platform()=0;
    virtual bool ShouldClose()=0;
    virtual void SetUserdata(void *userdata)=0;
    virtual void* GetUserdata()=0;
    virtual void DrawBuffer(FrameBuffer *buffer)=0;
    virtual void SetInputCallbacks(Callbacks callbacks)=0;
    virtual void PollEvents()=0;
    virtual int GetKeyPressed(KeyCode key)=0;
    virtual int GetButtonPressed(Button button)=0;
    virtual void GetCursor(float *xpos, float *ypos)=0;
    virtual float GetTime()=0;
};
