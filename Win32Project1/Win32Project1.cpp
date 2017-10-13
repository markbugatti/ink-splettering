// Win32Project1.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "Win32Project1.h"
#include "Functions.h"
#include <ctime>
#include <d2d1.h>

#define MAX_LOADSTRING 100
#define MAX_SITE 307200 // 640x480
#define COUNT_PART 4

#ifndef RGBA
#define RGBA(r,g,b,a)        ((COLORREF)( (((DWORD)(BYTE)(a))<<24) | RGB(r,g,b) ))
#endif


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
WCHAR szWindowClassChild[MAX_LOADSTRING];       // the child window class name


Site cSite[MAX_SITE];
//Site **cSite = new Site *[WINDOWHIGHTCHILD / UNIT];

int top_site = 0;
int cur_site = 0;
int delta_t = 10; // delta t
SIZE ñlientSize;
int timeCounter = 0;
static time_t t1, t2;
bool bCaptured = false;
BITMAP bm;
HBITMAP hBm;
HDC hMemDc;
HDC hdc;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
ATOM				MyRegisterClassChild(HINSTANCE hInstance);
HWND                InitInstance(HINSTANCE, int);
HWND				InitInstanceChild(HINSTANCE, int, HWND*);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	WndProcChild(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WINCHILD, szWindowClassChild, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
	MyRegisterClassChild(hInstance);

//	for (size_t i = 0; i < WINDOWHIGHTCHILD / UNIT; i++)
//		cSite[i] = new Site[WINDOWWIDTHCHILD / UNIT];


    // Perform application initialization:
	HWND *hWndMain = new HWND;
	if (!(*hWndMain = InitInstance (hInstance, nCmdShow)))
    {
		return FALSE;
    }
	
	HWND *hWndChild = new HWND(InitInstanceChild(hInst, 0, hWndMain));
	

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32PROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

ATOM MyRegisterClassChild(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex2;

	wcex2.cbSize = sizeof(WNDCLASSEX);

	wcex2.style = CS_HREDRAW | CS_VREDRAW;
	wcex2.lpfnWndProc = WndProcChild;
	wcex2.cbClsExtra = 0;
	wcex2.cbWndExtra = 0;
	wcex2.hInstance = hInstance;
	wcex2.hIcon = NULL;
	wcex2.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex2.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex2.lpszMenuName = NULL;
	wcex2.lpszClassName = szWindowClassChild;
	wcex2.hIconSm = NULL;

	return RegisterClassExW(&wcex2);
}

HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ^ WS_MINIMIZEBOX ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME,
      CW_USEDEFAULT, 0, WINDOWWIDTH, WINDOWHIGHT, nullptr, nullptr, hInstance, nullptr);
   
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return hWnd;
}

HWND InitInstanceChild(HINSTANCE hInstance, int nCmdShow, HWND *in_hWnd)
{
	int cx = 640, cy = 480;
	HWND hWnd = CreateWindowW(szWindowClassChild, nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER,
		(ñlientSize.cx-cx)/2, (ñlientSize.cy - cy)/2, cx, cy, *in_hWnd, nullptr, hInstance, nullptr);
	if (!hWnd) {
		return FALSE;
	}
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	return hWnd;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_SIZE:
		ñlientSize.cx = LOWORD(lParam);
		ñlientSize.cy = HIWORD(lParam);
	break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code that uses hdc here...
			EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

HBRUSH hBrush;
HPEN hPen;
RECT rc;

LRESULT CALLBACK WndProcChild(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		CreateLattice(cSite, UNIT, { WINDOWWIDTHCHILD, WINDOWHIGHTCHILD }, top_site);

		//static ID2D1Factory* pD2DFactory = NULL;

		//static HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
		//
		//static ID2D1HwndRenderTarget* pRT = NULL;
		//static HRESULT hr1 = pD2DFactory->CreateHwndRenderTarget(
		//	D2D1::RenderTargetProperties(),
		//	D2D1::HwndRenderTargetProperties(
		//		hWnd,
		//		D2D1::SizeU(
		//			WINDOWWIDTHCHILD,
		//			WINDOWHIGHTCHILD
		//			)
		//		),
		//	&pRT
		//	);
		hdc = GetDC(hWnd);
		hMemDc = CreateCompatibleDC(hdc);
		hBm = CreateCompatibleBitmap(hdc, WINDOWWIDTHCHILD, WINDOWHIGHTCHILD);
		SelectObject(hMemDc, hBm);

		PatBlt(hMemDc, 0, 0, WINDOWWIDTHCHILD, WINDOWHIGHTCHILD, WHITENESS);
		ReleaseDC(hWnd, hdc);
	}
		break;
	case WM_LBUTTONDOWN: {
		//SetCapture(hWnd);
		if (bCaptured) {
			KillTimer(hWnd, IDT_TIMER1);
			bCaptured = false;
		}
		else {
			bCaptured = true;
			SetTimer(hWnd, IDT_TIMER1, delta_t, NULL);
			fellOnSite(cSite, { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }, top_site);
		}
	}
		break;
	case WM_TIMER:
//		t1 = time(nullptr);

		perform(cSite, top_site, delta_t);
		
		for (int i = 0; i < top_site; i++)
		{
			hBrush = CreateSolidBrush( RGB( 0.05 * cSite[i].count_part <= 255 ? 0.05 * cSite[i].count_part : 0,  0 , 0));
			SelectObject(hMemDc, hBrush);
			rc = { cSite[i].pos.x, cSite[i].pos.y, cSite[i].pos.x + UNIT, cSite[i].pos.y + UNIT };
			FillRect(hMemDc, &rc, hBrush);
			DeleteObject(hBrush);
		}
		//MoveToEx(hMemDc, 10, 10, NULL);
		//LineTo(hMemDc, 40, 40);

		InvalidateRect(hWnd, NULL, FALSE);
//		t2 = time(nullptr);
//		delta_t = t2 - t1;
		break;
	case WM_PERFORM: { // user message
		//t1 = time(nullptr);

		perform(cSite, top_site, delta_t);

		//t2 = time(nullptr);
		//delta_t = t2 - t1;

		//InvalidateRect(hWnd, NULL, TRUE);
		//if(bCaptured)
			//SendMessage(hWnd, WM_PERFORM, NULL, NULL);
	}
		break;
	case WM_LBUTTONUP:
		/*ReleaseCapture();
		bCaptured = false;
		break;*/
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		
		hdc = BeginPaint(hWnd, &ps);
		
		BitBlt(hdc, 0, 0, WINDOWWIDTHCHILD, WINDOWHIGHTCHILD, hMemDc, 0, 0, SRCCOPY);
		//for (size_t i = 0; i < top_site; i++)
		//{
		//	Rectangle(ps.hdc, cSite[i].pos.x, cSite[i].pos.y, cSite[i].pos.x + cSite[i].size.cx, cSite[i].pos.y + cSite[i].size.cy);
		//}
		 
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		DeleteObject(hMemDc);
		delete[] cSite;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}