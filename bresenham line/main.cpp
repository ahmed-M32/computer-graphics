#include <windows.h>
#include <iostream>

POINT currentPoints[4];
int clickCount = 0;

void DrawBresenhamLine(HWND hwnd, POINT pts[2],COLORREF c1);
void setPixel(HWND hwnd ,int x,int y,COLORREF c);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_LBUTTONDOWN: {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        currentPoints[clickCount].x = x;
        currentPoints[clickCount].y = y;
        std::cout << "Click " << clickCount + 1 << ": (" << x << ", " << y << ")" << std::endl;
        clickCount++;

        if (clickCount == 2) {
            COLORREF c1 = RGB(255, 0, 0);
            DrawBresenhamLine(hwnd, currentPoints ,c1);
            clickCount = 0;
        }
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}





void DrawBresenhamLine(HWND hwnd, POINT pts[2], COLORREF c1) {
    HDC hdc = GetDC(hwnd);

    int x0 = pts[0].x;
    int y0 = pts[0].y;
    int x1 = pts[1].x;
    int y1 = pts[1].y;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    SetPixel(hdc, x0, y0, c1);

    while (true) {
        if (x0 == x1 && y0 == y1) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }

    ReleaseDC(hwnd, hdc);
}





int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    const char CLASS_NAME[] = "BresenhamLineWindow";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Click twice to Draw a line",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
