#include <windows.h>
#include <iostream>

POINT currentPoints[4];
int clickCount = 0;

void DrawBezierCurve(HWND hwnd, POINT pts[4],COLORREF c1,COLORREF c2, COLORREF c3, COLORREF c4);
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

        if (clickCount == 4) {
            COLORREF c1 = RGB(255, 0, 0);
            COLORREF c2 = RGB(0, 255, 0);
            COLORREF c3 = RGB(0, 0, 255);
            COLORREF c4 = RGB(100, 100, 100);
            DrawBezierCurve(hwnd, currentPoints ,c1,c2,c3,c4);
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



//the eqution  = u*u*u*p1 + 3*u*u*t*p1 + 3*u*t*t*p3 + t*t*t*p4;

void DrawBezierCurve(HWND hwnd, POINT pts[4],COLORREF c1,COLORREF c2 , COLORREF c3, COLORREF c4) {
    HDC hdc = GetDC(hwnd);
    for (double i = 0; i < 1; i+=0.001) {
        double u = 1-i;

        double x= u*u*u*pts[0].x + 3*u*u*i*pts[1].x + 3*u*i*i*pts[2].x + i*i*i*pts[3].x;
        double y = u*u*u*pts[0].y + 3*u*u*i*pts[1].y + 3*u*i*i*pts[2].y + i*i*i*pts[3].y;

        double r =  u*u*u* GetRValue(c1) + 3*u*u*i*GetRValue(c2) + 3*u*i*i*GetRValue(c3) +i*i*i*GetRValue(c4);
        double g =  u*u*u* GetGValue(c1) + 3*u*u*i*GetGValue(c2) + 3*u*i*i*GetGValue(c3) +i*i*i*GetGValue(c4);
        double b =  u*u*u* GetBValue(c1) + 3*u*u*i*GetBValue(c2) + 3*u*i*i*GetBValue(c3) +i*i*i*GetBValue(c4);


        SetPixel(hdc,x,y,RGB(r,g,b));
    }

    ReleaseDC(hwnd, hdc);
}


/*void setPixel(HWND hwnd, int x, int y, COLORREF c) {

}*/


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    const char CLASS_NAME[] = "BezierClickWindow";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Click to Draw Bézier Curves",
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
