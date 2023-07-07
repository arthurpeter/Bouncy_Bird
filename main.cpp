#include <windows.h>

//#include <random>
bool running = true;
int buffer_width;
int buffer_heigth;
void *buffer_memory;
BITMAPINFO buffer_bitmap_info;

#include "renderer.cpp"
#include "platform_common.cpp"
#include "game.cpp"

unsigned int direction = right;

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    switch (uMsg) {
        case WM_CLOSE:
        case WM_DESTROY: {running = false;} break;

        case WM_SIZE: {
            RECT rect;
            GetClientRect(hwnd, &rect);
            buffer_width = rect.right - rect.left;
            buffer_heigth = rect.bottom - rect.top;

            int buffer_size = buffer_width * buffer_heigth * sizeof(unsigned int);

            if (buffer_memory) VirtualFree(buffer_memory, 0, MEM_RELEASE);
            buffer_memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            buffer_bitmap_info.bmiHeader.biSize = sizeof(buffer_bitmap_info.bmiHeader);
            buffer_bitmap_info.bmiHeader.biWidth = buffer_width;
            buffer_bitmap_info.bmiHeader.biHeight = buffer_heigth;
            buffer_bitmap_info.bmiHeader.biPlanes = 1;
            buffer_bitmap_info.bmiHeader.biBitCount = 32;
            buffer_bitmap_info.bmiHeader.biCompression = BI_RGB;

            } break;

        default: {result = DefWindowProc(hwnd, uMsg, wParam, lParam);}
    }
    return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    ShowCursor(false);
    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = "Bouncy Bird";
    window_class.lpfnWndProc = window_callback;

    RegisterClass(&window_class);

    HWND window = CreateWindow(window_class.lpszClassName, "Bouncy Bird", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1080, 640, 0, 0, hInstance, 0);
    {
        // fullscreen
        /*SetWindowLong(window, GWL_STYLE, GetWindowLong(window, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);
		MONITORINFO mi = { sizeof(mi) };
		GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &mi);
		SetWindowPos(window, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);*/
        
    }
    HDC hdc = GetDC(window);

    Input input = {};

    float delta_time = 0.016666f;
    LARGE_INTEGER frame_begin_time;
    QueryPerformanceCounter(&frame_begin_time);

    float performance_frequency;
    {
        LARGE_INTEGER perf;
        QueryPerformanceFrequency(&perf);
        performance_frequency = (float)perf.QuadPart;
    }
    
    while (running) {

        MSG message;

        for (int i = 0 ; i < BUTTON_COUNT ; i++) {
            input.buttons[i].changed = false;
        }

        while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
            switch(message.message) {
                case WM_KEYUP:
                case WM_KEYDOWN: {
                    unsigned int vk_code = (unsigned int)message.wParam;
                    bool is_down = ((message.lParam & (1 << 31)) == 0);

#define process_button(b, vk)\
case vk: {\
input.buttons[b].changed = is_down != input.buttons[b].is_down;\
input.buttons[b].is_down = is_down;\
}break;

                    switch(vk_code) {
                        process_button(BUTTON_UP, VK_UP);
                        process_button(BUTTON_DOWN, VK_DOWN);
                        process_button(BUTTON_LEFT, VK_LEFT);
                        process_button(BUTTON_RIGHT, VK_RIGHT);
                        process_button(BUTTON_UP, 0x57);
                        process_button(BUTTON_DOWN, 0x53);
                        process_button(BUTTON_LEFT, 0x41);
                        process_button(BUTTON_RIGHT, 0x44);
                        process_button(BUTTON_JUMP, VK_SPACE);
                        process_button(BUTTON_JUMP, VK_LBUTTON);
                        process_button(BUTTON_ENTER, VK_RETURN);
                    }
                }break;
                default: {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                }
            }
            
        }

        // Simulate
        simulate_game(&input, delta_time, &direction);

        StretchDIBits(hdc, 0, 0, buffer_width, buffer_heigth, 0, 0, buffer_width, buffer_heigth, buffer_memory, &buffer_bitmap_info, DIB_RGB_COLORS, SRCCOPY);

        LARGE_INTEGER frame_end_time;
        QueryPerformanceCounter(&frame_end_time);
        delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
        frame_begin_time = frame_end_time;

    }
}
