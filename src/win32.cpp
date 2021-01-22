#include <string.h>
#include <windows.h>
#include "graphics.hpp"
#include "platform.hpp"
#include "win32.hpp"
#include "win32_func.cpp"


Win32::Win32():
    m_pSurface(nullptr),
    m_ShouldClose(false),
    m_pUserdata(nullptr){
}

Win32::~Win32(){
}

void Win32::Create(int width, int height){
    // platform init
    register_class();
    initialize_path();

    HWND handle;
    Image *surface;
    HDC memory_dc;
    handle = create_window("zy_renderer", width, height);
    create_surface(handle, width, height, &surface, &memory_dc);

    this->m_Handle = handle;
    this->m_MemoryDC = memory_dc;
    this->m_pSurface = surface;

    SetProp(handle, WINDOW_ENTRY_NAME, this);
    ShowWindow(handle, SW_SHOW);
}

void Win32::Destroy(){
    ShowWindow(this->m_Handle, SW_HIDE);
    RemoveProp(this->m_Handle, WINDOW_ENTRY_NAME);

    DeleteDC(this->m_MemoryDC);
    DestroyWindow(this->m_Handle);

    this->m_pSurface->ldr_buffer = nullptr;
    image_release(this->m_pSurface);

    // platform deinit
    unregister_class();
}

bool Win32::ShouldClose() {
    return this->m_ShouldClose;
}

void Win32::SetUserdata(void *userdata) {
    this->m_pUserdata = userdata;
}

void* Win32::GetUserdata() {
    return this->m_pUserdata;
}

void Win32::DrawBuffer(FrameBuffer *buffer) {
    blit_bgr(buffer, this->m_pSurface);

    HDC window_dc = GetDC(this->m_Handle);
    HDC memory_dc = this->m_MemoryDC;
    int width = this->m_pSurface->width;
    int height = this->m_pSurface->height;
    BitBlt(window_dc, 0, 0, width, height, memory_dc, 0, 0, SRCCOPY);
    ReleaseDC(this->m_Handle, window_dc);
}

void Win32::SetInputCallbacks(Callbacks callbacks) {
    this->m_Callbacks = callbacks;
}

void Win32::PollEvents(void) {
    MSG message;
    while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

int Win32::GetKeyPressed(KeyCode key){
    return this->m_Keys[key];
}

int Win32::GetButtonPressed(Button button){
    return this->m_Buttons[button];
}

void Win32::GetCursor(float *xpos, float *ypos){
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(this->m_Handle, &point);
    *xpos = (float)point.x;
    *ypos = (float)point.y;
}

float Win32::GetTime(){
    static double initial = -1;
    if (initial < 0) {
        initial = get_native_time();
    }
    return (float)(get_native_time() - initial);
}
