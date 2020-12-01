#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <windows.h>
// #include "../core/graphics.h"
#include "../include/lrImage.h"
#include "../include/lrMacro.h"
#include "../include/lrDisplay.h"
// #include "../core/private.h"

struct window {
    HWND handle;
    HDC memory_dc;
    image_t *surface;
    /* common data */
    int should_close;
    char keys[KEY_NUM];
    char buttons[BUTTON_NUM];
    callbacks_t callbacks;
    void *userdata;
};

/* window related functions */

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
static void handle_key_message(window_t *window, WPARAM virtual_key,
                               char pressed) {
    keycode_t key;
    switch (virtual_key) {
        case 'A':       key = KEY_A;     break;
        case 'D':       key = KEY_D;     break;
        case 'S':       key = KEY_S;     break;
        case 'W':       key = KEY_W;     break;
        case VK_SPACE:  key = KEY_SPACE; break;
        case VK_ESCAPE: key = KEY_ESC;   break;
        default:        key = KEY_NUM;   break;
    }
    if (key < KEY_NUM) {
        window->keys[key] = pressed;
        if (window->callbacks.key_callback) {
            window->callbacks.key_callback(window, key, pressed);
        }
    }
}

static void handle_button_message(window_t *window, button_t button,
                                  char pressed) {
    window->buttons[button] = pressed;
    if (window->callbacks.button_callback) {
        window->callbacks.button_callback(window, button, pressed);
    }
}

static void handle_scroll_message(window_t *window, float offset) {
    if (window->callbacks.scroll_callback) {
        window->callbacks.scroll_callback(window, offset);
    }
}

static LRESULT CALLBACK process_message(HWND hWnd, UINT uMsg,
                                        WPARAM wParam, LPARAM lParam) {
    window_t *window = (window_t*)GetProp(hWnd, WINDOW_ENTRY_NAME);
    if (window == NULL) {
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    } else if (uMsg == WM_CLOSE) {
        window->should_close = 1;
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

static void register_class(void) {
    static int initialized = 0;
    if (initialized == 0) {
        ATOM class_atom;
        WNDCLASS window_class;
        window_class.style = CS_HREDRAW | CS_VREDRAW;
        window_class.lpfnWndProc = process_message;
        window_class.cbClsExtra = 0;
        window_class.cbWndExtra = 0;
        window_class.hInstance = GetModuleHandle(NULL);
        window_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
        window_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        window_class.lpszMenuName = NULL;
        window_class.lpszClassName = WINDOW_CLASS_NAME;
        class_atom = RegisterClass(&window_class);
        assert(class_atom != 0);
        initialized = 1;
    }
}

static HICON make_icon(){
    HICON hIcon;      // icon handle 
    // Yang icon AND bitmask 
    BYTE ANDmaskIcon[] = {0xFF, 0xFF, 0xFF, 0xFF,   // line 1 
                      0xFF, 0xFF, 0xC3, 0xFF,   // line 2 
                      0xFF, 0xFF, 0x00, 0xFF,   // line 3 
                      0xFF, 0xFE, 0x00, 0x7F,   // line 4 
 
                      0xFF, 0xFC, 0x00, 0x1F,   // line 5 
                      0xFF, 0xF8, 0x00, 0x0F,   // line 6 
                      0xFF, 0xF8, 0x00, 0x0F,   // line 7 
                      0xFF, 0xF0, 0x00, 0x07,   // line 8 
 
                      0xFF, 0xF0, 0x00, 0x03,   // line 9 
                      0xFF, 0xE0, 0x00, 0x03,   // line 10 
                      0xFF, 0xE0, 0x00, 0x01,   // line 11 
                      0xFF, 0xE0, 0x00, 0x01,   // line 12 
 
                      0xFF, 0xF0, 0x00, 0x01,   // line 13 
                      0xFF, 0xF0, 0x00, 0x00,   // line 14 
                      0xFF, 0xF8, 0x00, 0x00,   // line 15 
                      0xFF, 0xFC, 0x00, 0x00,   // line 16 
 
                      0xFF, 0xFF, 0x00, 0x00,   // line 17 
                      0xFF, 0xFF, 0x80, 0x00,   // line 18 
                      0xFF, 0xFF, 0xE0, 0x00,   // line 19 
                      0xFF, 0xFF, 0xE0, 0x01,   // line 20 
 
                      0xFF, 0xFF, 0xF0, 0x01,   // line 21 
                      0xFF, 0xFF, 0xF0, 0x01,   // line 22 
                      0xFF, 0xFF, 0xF0, 0x03,   // line 23 
                      0xFF, 0xFF, 0xE0, 0x03,   // line 24 
 
                      0xFF, 0xFF, 0xE0, 0x07,   // line 25 
                      0xFF, 0xFF, 0xC0, 0x0F,   // line 26 
                      0xFF, 0xFF, 0xC0, 0x0F,   // line 27 
                      0xFF, 0xFF, 0x80, 0x1F,   // line 28 
 
                      0xFF, 0xFF, 0x00, 0x7F,   // line 29 
                      0xFF, 0xFC, 0x00, 0xFF,   // line 30 
                      0xFF, 0xF8, 0x03, 0xFF,   // line 31 
                      0xFF, 0xFC, 0x3F, 0xFF};  // line 32 
 
    // Yang icon XOR bitmask 
    BYTE XORmaskIcon[] = {0x00, 0x00, 0x00, 0x00,   // line 1 
                      0x00, 0x00, 0x00, 0x00,   // line 2 
                      0x00, 0x00, 0x00, 0x00,   // line 3 
                      0x00, 0x00, 0x00, 0x00,   // line 4 
 
                      0x00, 0x00, 0x00, 0x00,   // line 5 
                      0x00, 0x00, 0x00, 0x00,   // line 6 
                      0x00, 0x00, 0x00, 0x00,   // line 7 
                      0x00, 0x00, 0x38, 0x00,   // line 8 
 
                      0x00, 0x00, 0x7C, 0x00,   // line 9 
                      0x00, 0x00, 0x7C, 0x00,   // line 10 
                      0x00, 0x00, 0x7C, 0x00,   // line 11 
                      0x00, 0x00, 0x38, 0x00,   // line 12 
 
                      0x00, 0x00, 0x00, 0x00,   // line 13 
                      0x00, 0x00, 0x00, 0x00,   // line 14 
                      0x00, 0x00, 0x00, 0x00,   // line 15 
                      0x00, 0x00, 0x00, 0x00,   // line 16 
 
                      0x00, 0x00, 0x00, 0x00,   // line 17 
                      0x00, 0x00, 0x00, 0x00,   // line 18 
                      0x00, 0x00, 0x00, 0x00,   // line 19 
                      0x00, 0x00, 0x00, 0x00,   // line 20 
 
                      0x00, 0x00, 0x00, 0x00,   // line 21 
                      0x00, 0x00, 0x00, 0x00,   // line 22 
                      0x00, 0x00, 0x00, 0x00,   // line 23 
                      0x00, 0x00, 0x00, 0x00,   // line 24 
 
                      0x00, 0x00, 0x00, 0x00,   // line 25 
                      0x00, 0x00, 0x00, 0x00,   // line 26 
                      0x00, 0x00, 0x00, 0x00,   // line 27 
                      0x00, 0x00, 0x00, 0x00,   // line 28 
 
                      0x00, 0x00, 0x00, 0x00,   // line 29 
                      0x00, 0x00, 0x00, 0x00,   // line 30 
                      0x00, 0x00, 0x00, 0x00,   // line 31 
                      0x00, 0x00, 0x00, 0x00};  // line 32 
 
    hIcon = CreateIcon(GetModuleHandle(NULL),    // application instance  
        32,              // icon width 
        32,              // icon height 
        1,               // number of XOR planes 
        1,               // number of bits per pixel 
        ANDmaskIcon,     // AND bitmask  
        XORmaskIcon);    // XOR bitmask 
    return hIcon;
}

static HWND create_window(const char *title_, int width, int height) {
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
                          NULL, NULL, GetModuleHandle(NULL), NULL);
    assert(handle != NULL);


    //set icon for window
    HICON hIcon = make_icon();
    SendMessage(handle, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    SendMessage(handle, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

    return handle;
}

/*
 * for memory device context, see
 * https://docs.microsoft.com/en-us/windows/desktop/gdi/memory-device-contexts
 */
static void create_surface(HWND handle, int width, int height,
                           image_t **out_surface, HDC *out_memory_dc) {
    BITMAPINFOHEADER bi_header;
    HDC window_dc;
    HDC memory_dc;
    HBITMAP dib_bitmap;
    HBITMAP old_bitmap;
    unsigned char *buffer;
    image_t *surface;

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
                                  DIB_RGB_COLORS, (void**)&buffer, NULL, 0);
    assert(dib_bitmap != NULL);
    old_bitmap = (HBITMAP)SelectObject(memory_dc, dib_bitmap);
    DeleteObject(old_bitmap);

    surface = (image_t*)malloc(sizeof(image_t));
    surface->width = width;
    surface->height = height;
    surface->channels = 4;
    surface->buffer = buffer;

    *out_surface = surface;
    *out_memory_dc = memory_dc;
}

window_t *window_create(const char *title, int width, int height) {
    window_t *window;
    HWND handle;
    image_t *surface;
    HDC memory_dc;

    assert(width > 0 && height > 0);

    register_class();
    handle = create_window(title, width, height);
    create_surface(handle, width, height, &surface, &memory_dc);

    window = (window_t*)malloc(sizeof(window_t));
    memset(window, 0, sizeof(window_t));
    window->handle = handle;
    window->memory_dc = memory_dc;
    window->surface = surface;

    SetProp(handle, WINDOW_ENTRY_NAME, window);
    ShowWindow(handle, SW_SHOW);
    return window;
}

void window_destroy(window_t *window) {
    ShowWindow(window->handle, SW_HIDE);
    RemoveProp(window->handle, WINDOW_ENTRY_NAME);

    DeleteDC(window->memory_dc);
    DestroyWindow(window->handle);

    free(window->surface);
    free(window);
}


int window_should_close(window_t *window) {
    return window->should_close;
}

void window_set_userdata(window_t *window, void *userdata) {
    window->userdata = userdata;
}

void *window_get_userdata(window_t *window) {
    return window->userdata;
}

static void present_surface(window_t *window) {
    HDC window_dc = GetDC(window->handle);
    HDC memory_dc = window->memory_dc;
    image_t *surface = window->surface;
    int width = surface->width;
    int height = surface->height;
    BitBlt(window_dc, 0, 0, width, height, memory_dc, 0, 0, SRCCOPY);
    ReleaseDC(window->handle, window_dc);
}


void window_draw_image(window_t *window, image_t *image) {
    blit_image_rgb(image, window->surface);
    present_surface(window);
}


void window_draw_buffer(window_t *window, framebuffer_t *buffer) {
    blit_buffer_rgb(buffer, window->surface);
    present_surface(window);
}


/* input related functions */

void input_poll_events(void) {
    MSG message;
    while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

int input_key_pressed(window_t *window, keycode_t key) {
    assert(key >= 0 && key < KEY_NUM);
    return window->keys[key];
}

int input_button_pressed(window_t *window, button_t button) {
    assert(button >= 0 && button < BUTTON_NUM);
    return window->buttons[button];
}

void input_query_cursor(window_t *window, float *xpos, float *ypos) {
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(window->handle, &point);
    *xpos = (float)point.x;
    *ypos = (float)point.y;
}

void input_set_callbacks(window_t *window, callbacks_t callbacks) {
    window->callbacks = callbacks;
}

/* misc platform functions */

static double get_native_time(void) {
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

float platform_get_time(void) {
    static double initial = -1;
    if (initial < 0) {
        initial = get_native_time();
    }
    return (float)(get_native_time() - initial);
}

void platform_init_path(void) {
#ifdef UNICODE
    wchar_t path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    *wcsrchr(path, L'\\') = L'\0';
    _wchdir(path);
    _wchdir(L"assets");
#else
    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    *strrchr(path, '\\') = '\0';
    _chdir(path);
    _chdir("assets");
#endif
}