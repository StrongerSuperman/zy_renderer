#pragma once

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <windows.h>

#include "../core/platform.hpp"
#include "../core/image.hpp"


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

    Win32(int width, int height);
    virtual ~Win32();

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


#ifdef UNICODE
    static const wchar_t *WINDOW_CLASS_NAME = L"Class";
    static const wchar_t *WINDOW_ENTRY_NAME = L"Entry";
#else
    static const char *WINDOW_CLASS_NAME = "Class";
    static const char *WINDOW_ENTRY_NAME = "Entry";
#endif

/*
 * for virtual-key codes, see
 * https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes
 */
void handle_key_message(Win32 *window, WPARAM virtual_key, char pressed);
void handle_button_message(Win32 *window, Button button, char pressed);
void handle_scroll_message(Win32 *window, float offset);
LRESULT CALLBACK process_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

double get_native_time(void);

/* platform initialization */
void register_class(void);
void unregister_class(void);
void initialize_path(void);

/* window related functions */
HWND create_window(const char *title_, int width, int height);

/*
 * for memory device context, see
 * https://docs.microsoft.com/en-us/windows/desktop/gdi/memory-device-contexts
 */
void create_surface(HWND handle, int width, int height, Image **out_surface, HDC *out_memory_dc);
