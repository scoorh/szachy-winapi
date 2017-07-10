#include "CAplikacja.h"

namespace Szachy
{

	const std::string gWindowClassName = "SzachyWndClass";

	COkno::COkno() :
		mUchwytOkna(0),
		mCzyZarejestrowanoKlaseOkna(false),
		mAppustawienia(0),
		mCzyZadaZakmniecia(false),
		mCzyZminimalizowane(false),
		mCzyZmaksymalizowane(false),
		mCzySchowane(false)
	{
	}

	COkno::~COkno()
	{
	}

	EKodBledu COkno::Stworz(SUstawieniaAplikacji* ustawienia)
	{
		mAppustawienia = ustawienia;

		WNDCLASSEX wcex;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wcex.hCursor = LoadCursor(0, IDC_ARROW);
		wcex.hIcon = 0;
		wcex.hIconSm = 0;
		wcex.hInstance = ustawienia->instancja;
		wcex.lpfnWndProc = CAplikacja::Win32WndProc;
		wcex.lpszClassName = gWindowClassName.c_str();
		wcex.lpszMenuName = 0;
		wcex.style = CS_VREDRAW | CS_HREDRAW;
		if (!RegisterClassEx(&wcex))
			return KB_REJESTRACJA_KLASY_OKNA;
		mCzyZarejestrowanoKlaseOkna = true;

		DWORD style = WS_POPUP, exStyle = WS_EX_APPWINDOW;
		int xpos = 0, ypos = 0;
		style = WS_OVERLAPPEDWINDOW;
		exStyle |= WS_EX_WINDOWEDGE;
		xpos = (GetSystemMetrics(SM_CXSCREEN) - ustawienia->szerokoscOkna) / 2;
		ypos = (GetSystemMetrics(SM_CYSCREEN) - ustawienia->wysokoscOkna) / 2;
		

		RECT wndRect = { 0, 0, ustawienia->szerokoscOkna, ustawienia->wysokoscOkna };
		AdjustWindowRectEx(&wndRect, style, false, exStyle);

		mUchwytOkna = CreateWindowEx(exStyle, gWindowClassName.c_str(), ustawienia->tytul.c_str(), style, xpos, ypos,
			wndRect.right - wndRect.left, wndRect.bottom - wndRect.top, 0, 0, ustawienia->instancja, 0);
		if (!mUchwytOkna)
			return KB_TWORZENIE_OKNA;
		mCzySchowane = true;

		return KB_BRAK;
	}

	void COkno::Zniszcz()
	{
		if (mUchwytOkna)
		{
			DestroyWindow(mUchwytOkna);
			mUchwytOkna = 0;
		}
		if (mCzyZarejestrowanoKlaseOkna)
		{
			UnregisterClass(gWindowClassName.c_str(), mAppustawienia->instancja);
			mCzyZarejestrowanoKlaseOkna = false;
		}
	}

	void COkno::PompaWiadomosci()
	{
		MSG wiadomosc = { 0 };
		if (PeekMessage(&wiadomosc, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&wiadomosc);
			DispatchMessage(&wiadomosc);
		}

		if (wiadomosc.message == WM_QUIT)
			mCzyZadaZakmniecia = true;
	}

	bool COkno::ProceduraWiadomosci(HWND uchwytOkna, UINT wiadomosc, WPARAM parametrW, LPARAM parametrL)
	{
		switch (wiadomosc)
		{
		case WM_SIZE:
			mAppustawienia->szerokoscOkna = LOWORD(parametrL);
			mAppustawienia->wysokoscOkna = HIWORD(parametrL);
			if (parametrW == SIZE_MINIMIZED)
			{
				mCzySchowane = true;
				mCzyZminimalizowane = true;
				mCzyZmaksymalizowane = false;
			}
			else if (parametrW == SIZE_MAXIMIZED)
			{
				mCzyZminimalizowane = false;
				mCzySchowane = false;
				mCzyZmaksymalizowane = true;
			}
			else if (parametrW == SIZE_RESTORED)
			{
				if (mCzyZminimalizowane)
				{
					mCzyZminimalizowane = false;
					mCzySchowane = false;
				}
				else if (mCzyZmaksymalizowane)
				{
					mCzyZmaksymalizowane = false;
				}
			}
		return 0;

		case WM_GETMINMAXINFO:
			((MINMAXINFO*)parametrL)->ptMinTrackSize.x = 800;
			((MINMAXINFO*)parametrL)->ptMinTrackSize.y = 600;
			return true;

		case WM_MOVE:
			return true;

		case WM_CLOSE:
		case WM_DESTROY:
			PostQuitMessage(0);
			return true;
		}

		return false;
	}

	void COkno::PokazOkno(const bool& pokaz)
	{
		mCzySchowane = !pokaz;
		if (!mCzySchowane)
		{
			::ShowWindow(mUchwytOkna, SW_SHOW);
			::SetForegroundWindow(mUchwytOkna);
			::SetFocus(mUchwytOkna);
			::UpdateWindow(mUchwytOkna);
		}
		else
		{
			::ShowWindow(mUchwytOkna, SW_HIDE);
		}
	}

	bool COkno::CzyZadaZamkniecia() const
	{
		return mCzyZadaZakmniecia;
	}

	HWND COkno::UchwytOkna() const
	{
		return mUchwytOkna;
	}
}
