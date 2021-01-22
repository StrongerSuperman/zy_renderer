#pragma once

#include <windows.h>
#include "platform.hpp"


class Win32: public Platform{
public:
    HWND m_Handle;
    HDC m_MemoryDC;
    Image *m_pSurface;
    bool m_ShouldClose;
    char m_Keys[5];
    char m_Buttons[2];
    Callbacks m_Callbacks;
    void *m_pUserdata;

    Win32();
    virtual ~Win32();

    virtual void Create(int width, int height) override;
    virtual void Destroy() override;
    virtual bool ShouldClose() override;
    virtual void SetUserdata(void *userdata) override;
    virtual void* GetUserdata() override;
    virtual void DrawBuffer(FrameBuffer *buffer) override;
    virtual void SetInputCallbacks(Callbacks callbacks) override;
    virtual void PollEvents() override;
    virtual int GetKeyPressed(KeyCode key) override;
    virtual int GetButtonPressed(Button button) override;
    virtual void GetCursor(float *xpos, float *ypos) override;
    virtual float GetTime() override;
};
