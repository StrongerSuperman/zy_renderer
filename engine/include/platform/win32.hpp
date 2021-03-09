#pragma once

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <windows.h>

#include "core/platform.hpp"
#include "core/image.hpp"


class Win32: public Platform{
public:
    HWND m_Handle;
    HDC m_MemoryDC;

    Win32(int width, int height);
    ~Win32();

    void SwapFrameBuffer() override;
    void PollEvents() override;
    int GetKeyPressed(KeyCode key) override;
    int GetButtonPressed(const Button& button) override;
    void GetCursor(float *xpos, float *ypos) override;
    float GetTime() override;
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
