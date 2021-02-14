#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <windows.h>

#include "../core/framebuffer.hpp"
#include "../core/platform.hpp"
#include "../core/image.hpp"
#include "win32.hpp"


const int LINE_SIZE = 256;


/*
 * for virtual-key codes, see
 * https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes
 */
void handle_key_message(
        Win32 *window, WPARAM virtual_key, char pressed) {
    KeyCode key;
    switch (virtual_key) {
        case 'A':      key = KEY_A;     break;
        case 'D':      key = KEY_D;     break;
        case 'S':      key = KEY_S;     break;
        case 'W':      key = KEY_W;     break;
        case VK_SPACE: key = KEY_SPACE; break;
        default: return;
    }
    window->m_Keys[key] = pressed;
    if (window->m_Callbacks.key_callback) {
        window->m_Callbacks.key_callback(window, key, pressed);
    }
}

void handle_button_message(Win32 *window, Button button,
                                  char pressed) {
    window->m_Buttons[button] = pressed;
    if (window->m_Callbacks.button_callback) {
        window->m_Callbacks.button_callback(window, button, pressed);
    }
}

void handle_scroll_message(Win32 *window, float offset) {
    if (window->m_Callbacks.scroll_callback) {
        window->m_Callbacks.scroll_callback(window, offset);
    }
}

LRESULT CALLBACK process_message(HWND hWnd, UINT uMsg,
                                        WPARAM wParam, LPARAM lParam) {
    Win32 *window = (Win32*)GetProp(hWnd, WINDOW_ENTRY_NAME);
    if (window == nullptr) {
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    } else if (uMsg == WM_CLOSE) {
        window->m_ShouldClose = true;
        return 0;
    } else if (uMsg == WM_KEYDOWN) {
        handle_key_message(window, wParam, 1);
        return 0;
    } else if (uMsg == WM_KEYUP) {
        handle_key_message(window, wParam, 0);
        return 0;
    } else if (uMsg == WM_LBUTTONDOWN) {
        handle_button_message(window, BUTTON_L, 1);
        return 0;
    } else if (uMsg == WM_RBUTTONDOWN) {
        handle_button_message(window, BUTTON_R, 1);
        return 0;
    } else if (uMsg == WM_LBUTTONUP) {
        handle_button_message(window, BUTTON_L, 0);
        return 0;
    } else if (uMsg == WM_RBUTTONUP) {
        handle_button_message(window, BUTTON_R, 0);
        return 0;
    } else if (uMsg == WM_MOUSEWHEEL) {
        float offset = GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
        handle_scroll_message(window, offset);
        return 0;
    } else {
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

double get_native_time(void) {
    static double period = -1;
    LARGE_INTEGER counter;
    if (period < 0) {
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        period = 1 / (double)frequency.QuadPart;
    }
    QueryPerformanceCounter(&counter);
    return counter.QuadPart * period;
}

/* platform initialization */
void register_class(void) {
    ATOM class_atom;
    WNDCLASS window_class;
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpfnWndProc = process_message;
    window_class.cbClsExtra = 0;
    window_class.cbWndExtra = 0;
    window_class.hInstance = GetModuleHandle(nullptr);
    window_class.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
    window_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    window_class.lpszMenuName = nullptr;
    window_class.lpszClassName = WINDOW_CLASS_NAME;
    class_atom = RegisterClass(&window_class);
    assert(class_atom != 0);
    (void)class_atom;
}

void unregister_class(void) {
    UnregisterClass(WINDOW_CLASS_NAME, GetModuleHandle(nullptr));
}

void initialize_path(void) {
#ifdef UNICODE
    wchar_t path[MAX_PATH];
    GetModuleFileName(nullptr, path, MAX_PATH);
    *wcsrchr(path, L'\\') = L'\0';
    _wchdir(path);
    _wchdir(L"assets");
#else
    char path[MAX_PATH];
    GetModuleFileName(nullptr, path, MAX_PATH);
    *strrchr(path, '\\') = '\0';
    _chdir(path);
    _chdir("assets");
#endif
}

/* window related functions */
HWND create_window(const char *title_, int width, int height) {
    DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    RECT rect;
    HWND handle;

    #ifdef UNICODE
        wchar_t title[LINE_SIZE];
        mbstowcs(title, title_, LINE_SIZE);
    #else
        const char *title = title_;
    #endif

    rect.left = 0;
    rect.top = 0;
    rect.right = width;
    rect.bottom = height;
    AdjustWindowRect(&rect, style, 0);
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;

    handle = CreateWindow(WINDOW_CLASS_NAME, title, style,
                          CW_USEDEFAULT, CW_USEDEFAULT, width, height,
                          nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
    assert(handle != nullptr);
    return handle;
}

/*
 * for memory device context, see
 * https://docs.microsoft.com/en-us/windows/desktop/gdi/memory-device-contexts
 */
void create_surface(HWND handle, int width, int height,
                           Image **out_surface, HDC *out_memory_dc) {
    BITMAPINFOHEADER bi_header;
    HBITMAP dib_bitmap;
    HBITMAP old_bitmap;
    HDC window_dc;
    HDC memory_dc;

    Image *surface = new Image(width, height, 4, FORMAT_LDR);
    free(surface->m_LDRBuffer);
    surface->m_LDRBuffer = nullptr;

    window_dc = GetDC(handle);
    memory_dc = CreateCompatibleDC(window_dc);
    ReleaseDC(handle, window_dc);

    memset(&bi_header, 0, sizeof(BITMAPINFOHEADER));
    bi_header.biSize = sizeof(BITMAPINFOHEADER);
    bi_header.biWidth = width;
    bi_header.biHeight = -height;  /* top-down */
    bi_header.biPlanes = 1;
    bi_header.biBitCount = 32;
    bi_header.biCompression = BI_RGB;
    dib_bitmap = CreateDIBSection(memory_dc, (BITMAPINFO*)&bi_header,
                                    DIB_RGB_COLORS, (void**)&surface->m_LDRBuffer,
                                    nullptr, 0);
    assert(dib_bitmap != nullptr);
    old_bitmap = (HBITMAP)SelectObject(memory_dc, dib_bitmap);
    DeleteObject(old_bitmap);

    *out_surface = surface;
    *out_memory_dc = memory_dc;
}


Win32::Win32(int width, int height):
        m_pSurface(nullptr),
        m_ShouldClose(false),
        m_pUserdata(nullptr){
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

Win32::~Win32(){
    ShowWindow(this->m_Handle, SW_HIDE);
    RemoveProp(this->m_Handle, WINDOW_ENTRY_NAME);

    DeleteDC(this->m_MemoryDC);
    DestroyWindow(this->m_Handle);

    delete this->m_pSurface;

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
    buffer->BlitBGR(this->m_pSurface);

    HDC window_dc = GetDC(this->m_Handle);
    HDC memory_dc = this->m_MemoryDC;
    int width = this->m_pSurface->m_Width;
    int height = this->m_pSurface->m_Height;
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
