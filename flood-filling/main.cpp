#include <bits/stdc++.h>
#include <windows.h>
#include <cmath>

int xc = 0, yc = 0, r;

void DrawPoints(HDC hdc, int xc, int yc, int x, int y, COLORREF c);

void DrawCircleBres(HDC hdc, int xc, int yc, int i, DWORD dword);

void floodFill(HDC hdc, int xc, int yc, COLORREF c);

// Define Window Procedure
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_LBUTTONDOWN:
            xc = LOWORD(lParam);
            yc = HIWORD(lParam);
            break;

        case WM_LBUTTONUP: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            r = sqrt((x - xc) * (x - xc) + (y - yc) * (y - yc));
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
        case WM_RBUTTONDOWN: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            HDC hdc = GetDC(hWnd);
            COLORREF fillColor = RGB(123, 4, 45); // Purple fill color
            floodFill(hdc, x, y, fillColor);
            ReleaseDC(hWnd, hdc);
            break;
        }
        break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            DrawCircleBres(hdc, xc, yc, r, RGB(255, 0, 0));
            EndPaint(hWnd, &ps);
        }
        break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

// Main Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "MyWindow";
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, "MyWindow", "Circle Draw", WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
                               NULL, NULL, hInstance, NULL);

    if (!hwnd) return -1;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG message;
    while (GetMessage(&message, NULL, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return 0;
}

// Bresenham Circle Drawing
void DrawCircleBres(HDC hdc, int xc, int yc, int r, COLORREF c) {
    int x = 0, y = r, d = 1 - r;
    int d1 = 3, d2 = 5 - 2 * r;
    DrawPoints(hdc, xc, yc, x, y, c);
    while (x < y) {
        if (d < 0) {
            d += d1;
            d1 += 2;
            d2 += 2;
            x++;
        } else {
            d += d2;
            d1 += 2;
            d2 += 4;
            x++;
            y--;
        }
        DrawPoints(hdc, xc, yc, x, y, c);
    }
}

// Plotting Circle Symmetry Points
void DrawPoints(HDC hdc, int xc, int yc, int x, int y, COLORREF c) {
    SetPixel(hdc, xc + x, yc + y, c);
    SetPixel(hdc, xc - x, yc + y, c);
    SetPixel(hdc, xc + x, yc - y, c);
    SetPixel(hdc, xc - x, yc - y, c);
    SetPixel(hdc, xc + y, yc + x, c);
    SetPixel(hdc, xc + y, yc - x, c);
    SetPixel(hdc, xc - y, yc + x, c);
    SetPixel(hdc, xc - y, yc - x, c);
}


void floodFill(HDC hdc, int x, int y, COLORREF fillColor) {
    COLORREF targetColor = GetPixel(hdc, x, y);
    if (targetColor == fillColor) return;

    std::stack<std::pair<int, int>> s;
    s.push({x, y});

    while (!s.empty()) {
        auto [cx, cy] = s.top();
        s.pop();

        COLORREF current = GetPixel(hdc, cx, cy);
        if (current != targetColor) continue;

        SetPixel(hdc, cx, cy, fillColor);

        s.push({cx + 1, cy});
        s.push({cx - 1, cy});
        s.push({cx, cy + 1});
        s.push({cx, cy - 1});
    }
}
