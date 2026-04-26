#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;

#pragma comment (lib,"gdiplus.lib")

ULONG_PTR gdiplusToken;
HWND g_hWnd = NULL;
int g_percent = 0;

extern "C" __declspec(dllexport)
void InitProgress(HWND hWnd)
{
    g_hWnd = hWnd;
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

extern "C" __declspec(dllexport)
void SetProgress(int percent)
{
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;
    g_percent = percent;
    InvalidateRect(g_hWnd, NULL, TRUE);
}

extern "C" __declspec(dllexport)
void DrawProgress()
{
    if (!g_hWnd) return;

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(g_hWnd, &ps);

    Graphics g(hdc);
    g.SetSmoothingMode(SmoothingModeAntiAlias);

    Rect rect(10, 10, 100, 100);

    Pen penBg(Color(60, 200, 200, 200), 10);
    g.DrawArc(&penBg, rect, 0, 360);

    Pen penFg(Color(255, 0, 120, 215), 10);
    g.DrawArc(&penFg, rect, -90, (360 * g_percent) / 100);

    EndPaint(g_hWnd, &ps);
}

extern "C" __declspec(dllexport)
void FreeProgress()
{
    GdiplusShutdown(gdiplusToken);
}