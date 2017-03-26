#include <windows.h> 

LRESULT CALLBACK WinFun(HWND, UINT, WPARAM, LPARAM);

char WinName[] = "Window";

int WINAPI WinMain(HINSTANCE hIns, HINSTANCE hPrevIns, LPSTR arg, int WinMode)

{

	HWND hwnd;

	MSG msg;

	WNDCLASSEX wcl;

	wcl.hInstance = hIns;

	wcl.lpszClassName = WinName;

	wcl.lpfnWndProc = WinFun;

	wcl.style = 0;

	wcl.cbSize = sizeof(WNDCLASSEX);

	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	wcl.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);

	wcl.lpszMenuName = NULL;

	wcl.cbClsExtra = 0;

	wcl.cbWndExtra = 0;

	wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	if (!RegisterClassEx(&wcl)) return 0;

	hwnd = CreateWindow(

		WinName,

		"Ellipse",

		WS_OVERLAPPEDWINDOW,

		CW_USEDEFAULT,

		CW_USEDEFAULT,

		CW_USEDEFAULT,

		CW_USEDEFAULT,

		HWND_DESKTOP,

		NULL,

		hIns,

		NULL);

	ShowWindow(hwnd, WinMode);

	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))

	{

		TranslateMessage(&msg);

		DispatchMessage(&msg);

	}

	return msg.wParam;

}

struct Ellip

{

	int x1, x2, y1, y2;

};

struct ellipList

{

	Ellip E;

	ellipList *pNext;

};

ellipList *pFirst = 0, *p;

void add(ellipList *&pF, ellipList *p)

{

	p->pNext = pF;

	pF = p;

}

int x1, x2, y1, y2;

HPEN penB, penW;

HBRUSH brW, brB;



LRESULT CALLBACK WinFun(HWND hwnd, UINT message,

	WPARAM wParam, LPARAM lParam)

{

	HDC hdc;

	PAINTSTRUCT ps;

	switch (message)

	{

	case WM_CREATE:

		penW = CreatePen(PS_DASH, 1, RGB(255, 255, 255));

		penB = CreatePen(PS_DASH, 1, RGB(0, 0, 255));

		brW = CreateHatchBrush(HS_VERTICAL, RGB(255, 255, 255));

		brB = CreateHatchBrush(HS_VERTICAL, RGB(255, 0, 0));

		break;

	case WM_RBUTTONDOWN:

		x2 = x1 = LOWORD(lParam);

		y2 = y1 = HIWORD(lParam);

		break;

	case WM_MOUSEMOVE:

		if (wParam & MK_RBUTTON)

		{

			hdc = GetDC(hwnd);

			SelectObject(hdc, penW);

			SelectObject(hdc, brW);

			Ellipse(hdc, x1, y1, x2, y2);



			SelectObject(hdc, penB);

			SelectObject(hdc, brB);

			x2 = LOWORD(lParam);

			y2 = HIWORD(lParam);

			Ellipse(hdc, x1, y1, x2, y2);

			ReleaseDC(hwnd, hdc);

		}

		EndPaint(hwnd, &ps);

		break;

	case WM_RBUTTONUP:

		p = new ellipList;

		p->E.x1 = x1; p->E.x2 = x2;

		p->E.y1 = y1; p->E.y2 = y2;

		add(pFirst, p);

		break;



	case WM_PAINT:

		hdc = BeginPaint(hwnd, &ps);

		p = pFirst;

		while (p)

		{

			SelectObject(hdc, penB);

			SelectObject(hdc, brB);

			Ellipse(hdc, p->E.x1, p->E.y1, p->E.x2, p->E.y2);

			p = p->pNext;

		}

		EndPaint(hwnd, &ps);

		break;

	case WM_DESTROY:

		PostQuitMessage(0);

		break;

	default:

		return DefWindowProc(hwnd, message, wParam, lParam);

	}

	return 0;

}