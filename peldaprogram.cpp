#include "pch.h"
#include <windows.h>
#include <commdlg.h>
#include <commctrl.h>
#include <richedit.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <winspool.h>

#define HIBA_00 TEXT("Error:Program initialisation process.")
#define IDC_STATIC -1
#define LVM_SELECTED 2
#define WINSTYLE_NORMAL (WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX)
#define WINSTYLE_DIALOG (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU)
#define WINSTYLE_NONESIZEABLE (WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX)
#define WINSTYLE_CAPTIONONLY (WS_DLGFRAME)
#define WINSTYLE_NOCAPTON (WS_POPUP)

FILE *file1;
HINSTANCE hInstGlob;
int SajatiCmdShow;
POINT MousePos;

void ShowMessage(LPCTSTR uzenet, LPCTSTR cim, HWND kuldo);
int QuestionBox(LPCTSTR uzenet, LPCTSTR cim, HWND kuldo);

#define OBJ_ID100 100
#define OBJ_ID101 101
#define OBJ_ID102 102
HWND Form1;
HWND gomb1;
HWND gomb2;
HWND gomb3;
LRESULT CALLBACK WndProc0(HWND, UINT, WPARAM, LPARAM);
char szClassName[] = "WindowsApp";

void szoveg_nyomtat();
void grafika_nyomtat();
void bitkep_nyomtat();
HDC GetPrinterDC(HWND Hwnd);
void kepmeret_beallitas(HDC akt_hdc);

struct kepinfo {
	BITMAPFILEHEADER *pbmfh;
	BITMAPINFO *pbmi;
	BYTE *pBits;
	int Width, Height;
};
kepinfo Bitmap2;

BITMAPFILEHEADER *DibLoadImage(PTSTR pstrFileName);
void ScaleBMP2(HDC hdc, BITMAPINFO *pbmi, BYTE *pBits, int xkoo, int ykoo, int width, int height, int kep_width, int kep_height);
void BMP_betoltes(kepinfo *bitkep, const char kepfajl[]);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("StdWinClassName");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass0;
	LoadLibrary(TEXT("RICHED32.DLL"));
	LoadLibrary(TEXT("COMCTL32.DLL"));
	SajatiCmdShow = iCmdShow;
	hInstGlob = hInstance;

	wndclass0.style = CS_HREDRAW | CS_VREDRAW;
	wndclass0.lpfnWndProc = WndProc0;
	wndclass0.cbClsExtra = 0;
	wndclass0.cbWndExtra = 0;
	wndclass0.hInstance = hInstance;
	wndclass0.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass0.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass0.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wndclass0.lpszMenuName = NULL;
	wndclass0.lpszClassName = TEXT("WIN0");

	if (!RegisterClass(&wndclass0))
	{
		MessageBox(NULL, HIBA_00, TEXT("Program Start"), MB_ICONERROR); return 0;
	}

	Form1 = CreateWindow(TEXT("WIN0"),
		TEXT("Nyomtatás"),
		WINSTYLE_NORMAL,
		0,
		0,
		280,
		270,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(Form1, SajatiCmdShow);
	UpdateWindow(Form1);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc0(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int s1;
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;

	switch (message)
	{
	case WM_CREATE:
		gomb1 = CreateWindow(TEXT("button"), TEXT("Szöveg nyomtatása")
			, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE, 50, 50, 170, 30
			, hwnd, (HMENU)(OBJ_ID100), ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		gomb2 = CreateWindow(TEXT("button"), TEXT("Grafika nyomtatása")
			, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE, 50, 100, 170, 30
			, hwnd, (HMENU)(OBJ_ID101), ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		gomb3 = CreateWindow(TEXT("button"), TEXT("Bitkép nyomtatása")
			, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE, 50, 150, 170, 30
			, hwnd, (HMENU)(OBJ_ID102), ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		return 0;
	case WM_HSCROLL:
		switch (LOWORD(wParam))
		{
			;
		}
		return 0;
	case WM_VSCROLL:
		switch (LOWORD(wParam))
		{
			;
		}
		return 0;
	case WM_NOTIFY: {
		return 0; }
	case WM_LBUTTONUP:
		MousePos.x = LOWORD(lParam);
		MousePos.y = HIWORD(lParam);
		return 0;
	case WM_MBUTTONUP:
		MousePos.x = LOWORD(lParam);
		MousePos.y = HIWORD(lParam);
		return 0;
	case WM_RBUTTONUP:
		MousePos.x = LOWORD(lParam);
		MousePos.y = HIWORD(lParam);
		return 0;
	case WM_COMMAND:
		MousePos.x = LOWORD(lParam);
		MousePos.y = HIWORD(lParam);
		switch (LOWORD(wParam))
		{
		case OBJ_ID100:szoveg_nyomtat(); break;
		case OBJ_ID101:grafika_nyomtat(); break;
		case OBJ_ID102:bitkep_nyomtat(); break;
		}
		return 0;
	case WM_SIZE:
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

void ShowMessage(LPCTSTR uzenet, LPCTSTR cim, HWND kuldo)
{
	MessageBox(kuldo, uzenet, cim, MB_OK);
}

int QuestionBox(LPCTSTR uzenet, LPCTSTR cim, HWND kuldo)
{
	return MessageBox(kuldo, uzenet, cim, MB_YESNO);
}

void szoveg_nyomtat()
{
	HDC hdc, nyomtato;
	static DOCINFO di = { sizeof(DOCINFO), TEXT("Szöveges tartalom") };
	HFONT DisplayFont;

	nyomtato = GetPrinterDC(Form1);
	StartDoc(nyomtato, &di);
	StartPage(nyomtato);
	kepmeret_beallitas(nyomtato);
	HPEN ceruza;
	ceruza = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
	SelectObject(nyomtato, ceruza);
	SetDCBrushColor(nyomtato, RGB(255, 0, 0));
	SetTextColor(nyomtato, RGB(0, 200, 0));
	SetBkColor(nyomtato, RGB(0, 0, 255));

	DisplayFont = CreateFont(166, 0, 0, 0, FW_DONTCARE, false, false, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial Rounded MT Bold"));
	SelectObject(nyomtato, DisplayFont);
	TextOut(nyomtato, 0, 10, TEXT("Szöveg1"), 7);
	TextOut(nyomtato, 0, 100, TEXT("Szöveg2"), 18);

	DeleteObject(ceruza);
	EndPage(nyomtato);
	EndDoc(nyomtato);
	DeleteDC(nyomtato);
}

void grafika_nyomtat()
{
	HDC hdc, nyomtato;
	static DOCINFO di = { sizeof(DOCINFO), TEXT("Grafikus tartalom") };

	nyomtato = GetPrinterDC(Form1);
	StartDoc(nyomtato, &di);
	StartPage(nyomtato);
	kepmeret_beallitas(nyomtato);
	HPEN ceruza;
	ceruza = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
	SelectObject(nyomtato, ceruza);

	MoveToEx(nyomtato, 50, 500, NULL);
	LineTo(nyomtato, 500, 50);
	MoveToEx(nyomtato, 50, 50, NULL);
	LineTo(nyomtato, 500, 500);

	DeleteObject(ceruza);
	EndPage(nyomtato);
	EndDoc(nyomtato);
	DeleteDC(nyomtato);
}

HDC GetPrinterDC(HWND Hwnd)
{
	HDC hdc;
	PRINTDLG pd = { 0 };
	pd.lStructSize = sizeof(pd);
	pd.hwndOwner = Hwnd;
	pd.Flags = PD_RETURNDC;
	PrintDlg(&pd);
	hdc = pd.hDC;
	return hdc;
}

BITMAPFILEHEADER *DibLoadImage(PTSTR pstrFileName)
{
	BOOL bSuccess;
	DWORD dwFileSize, dwHighSize, dwBytesRead;
	HANDLE hFile;
	BITMAPFILEHEADER *pbmfh;

	hFile = CreateFileA((LPCSTR)pstrFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (hFile == INVALID_HANDLE_VALUE) { return NULL; }
	dwFileSize = GetFileSize(hFile, &dwHighSize);

	if (dwHighSize) { CloseHandle(hFile); return NULL; }
	pbmfh = (BITMAPFILEHEADER *)malloc(dwFileSize);
	if (!pbmfh) { CloseHandle(hFile); return NULL; }
	bSuccess = ReadFile(hFile, pbmfh, dwFileSize, &dwBytesRead, NULL);
	CloseHandle(hFile);

	if (!bSuccess || (dwBytesRead != dwFileSize)
		|| (pbmfh->bfType != *(WORD *)"BM")
		|| (pbmfh->bfSize != dwFileSize))
	{
		free(pbmfh); return NULL;
	}
	return pbmfh;
}

void ScaleBMP2(HDC hdc, BITMAPINFO *pbmi, BYTE *pBits, int xkoo, int ykoo, int width, int height, int kep_width, int kep_height)
{
	SetStretchBltMode(hdc, COLORONCOLOR);
	StretchDIBits(hdc, xkoo, ykoo, width, height, 0, 0, kep_width, kep_height, pBits, pbmi, DIB_RGB_COLORS, SRCCOPY);
}

void BMP_betoltes(kepinfo *bitkep, const char kepfajl[])
{
	if (bitkep->pbmfh) { free(bitkep->pbmfh); bitkep->pbmfh = NULL; }
	bitkep->pbmfh = DibLoadImage(PTSTR(kepfajl));
	if (bitkep->pbmfh == NULL);
	else
	{
		bitkep->pbmi = (BITMAPINFO *)(bitkep->pbmfh + 1);
		bitkep->pBits = (BYTE *)bitkep->pbmfh + bitkep->pbmfh->bfOffBits;
		if (bitkep->pbmi->bmiHeader.biSize == sizeof(BITMAPCOREHEADER))
		{
			bitkep->Width = ((BITMAPCOREHEADER *)bitkep->pbmi)->bcWidth;
			bitkep->Height = ((BITMAPCOREHEADER *)bitkep->pbmi)->bcHeight;
		}
		else
		{
			bitkep->Width = bitkep->pbmi->bmiHeader.biWidth;
			bitkep->Height = abs(bitkep->pbmi->bmiHeader.biHeight);
		}
	}
}

void bitkep_nyomtat()
{
	BMP_betoltes(&Bitmap2,"kep.bmp");
	HDC hdc, nyomtato;
	static DOCINFO di = { sizeof(DOCINFO), TEXT("Bitkép tartalom") };

	nyomtato = GetPrinterDC(Form1);
	StartDoc(nyomtato, &di);
	StartPage(nyomtato);
	kepmeret_beallitas(nyomtato);
	ScaleBMP2(nyomtato, Bitmap2.pbmi, Bitmap2.pBits, 10, 10, 1000, 600, 1000, 600);
	EndPage(nyomtato);
	EndDoc(nyomtato);
	DeleteDC(nyomtato);
}

void kepmeret_beallitas(HDC akt_hdc)
{
	/*RECT ablakterulet;
	GetClientRect(Form1, &ablakterulet);

	int WPrinter = GetDeviceCaps(akt_hdc, HORZRES);
	int HPrinter = GetDeviceCaps(akt_hdc, VERTRES);

	SetMapMode(akt_hdc, MM_ANISOTROPIC);

	SetWindowExtEx(akt_hdc, ablakterulet.right,
		ablakterulet.bottom, NULL);

	SetViewportExtEx(akt_hdc, WPrinter,
		HPrinter, NULL);*/

	HDC kepernyoHDC = GetDC(Form1);

	int LogPixelsXWin = GetDeviceCaps(kepernyoHDC, LOGPIXELSX);
	int LogPixelsYWin = GetDeviceCaps(kepernyoHDC, LOGPIXELSY);
	ReleaseDC(Form1,kepernyoHDC);

	HDC printerHDC = akt_hdc;

	int LogPixelsXWinPrinter = GetDeviceCaps(printerHDC, LOGPIXELSX);
	int LogPixelsYWinPrinter = GetDeviceCaps(printerHDC, LOGPIXELSY);

	SetMapMode(printerHDC, MM_ANISOTROPIC);
	SetWindowExtEx(printerHDC, LogPixelsXWin, LogPixelsYWin, NULL);

	SetViewportExtEx(printerHDC, LogPixelsXWinPrinter,
		LogPixelsYWinPrinter, NULL);

}