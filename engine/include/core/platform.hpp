#ifndef PLATFORM_H
#define PLATFORM_H

#include "define.hpp"
#include "userdata.hpp"
#include "framebuffer.hpp"


class Platform;

struct Callbacks{
    void (*key_callback)(Platform *platform, KeyCode key, int pressed);
	void (*mouse_press_callback)(Platform *platform, const Button& button, float x, float y, int pressed);
	void (*mouse_move_callback)(Platform *platform, float x, float y);
    void (*scroll_callback)(Platform *platform, float offset);
};

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
    void SetInputCallbacks(Callbacks& callbacks){this->m_Callbacks = callbacks;};
    Userdata* GetUserdata() {return this->m_pUserdata;};
    FrameBuffer* GetFrameBuffer() {return this->m_pFrameBuffer;};

    virtual void SwapFrameBuffer()=0;
    virtual void PollEvents()=0;
    virtual int GetKeyPressed(KeyCode key)=0;
    virtual int GetButtonPressed(const Button& button)=0;
    virtual void GetCursor(float *xpos, float *ypos)=0;
    virtual float GetTime()=0;

    virtual ~Platform() {delete this->m_pSurface; delete this->m_pUserdata;};
protected:
    Platform(int width, int height);
};

#endif //PLATFORM_H
