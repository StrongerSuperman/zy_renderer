#ifndef PLATFORM_H
#define PLATFORM_H

#include "enum.hpp"
#include "userdata.hpp"
#include "framebuffer.hpp"


class Platform;

struct Callbacks{
    void (*key_callback)(Platform *platform, KeyCode key, int pressed);
    void (*button_callback)(Platform *platform, Button button, int pressed);
    void (*scroll_callback)(Platform *platform, float offset);
};

class Platform{
public:
    Image *m_pSurface;
    bool m_ShouldClose;
    char m_Keys[5];
    char m_Buttons[2];
    Callbacks m_Callbacks;
    Userdata *m_pUserdata;

    Platform() {this->m_pSurface=nullptr, this->m_ShouldClose=false, this->m_pUserdata=new Userdata();};
    virtual ~Platform() {delete this->m_pSurface; delete this->m_pUserdata;};
    virtual bool ShouldClose() {return this->m_ShouldClose;};
    virtual Userdata* GetUserdata() {return this->m_pUserdata;};

    virtual void DrawBuffer(FrameBuffer *buffer)=0;
    virtual void SetInputCallbacks(Callbacks callbacks)=0;
    virtual void PollEvents()=0;
    virtual int GetKeyPressed(KeyCode key)=0;
    virtual int GetButtonPressed(Button button)=0;
    virtual void GetCursor(float *xpos, float *ypos)=0;
    virtual float GetTime()=0;
};

#endif //PLATFORM_H
