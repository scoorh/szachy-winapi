#include "CMysz.h"


namespace Szachy
{
	CMysz::CMysz()
	{}

	CMysz::~CMysz()
	{}

	bool CMysz::CzyNacisnietyLB() const
	{
		return mCzyNacisnietoLB;
	}

	bool CMysz::CzyNacisnietyRB() const
	{
		return mCzyNacisnietoRB;
	}

	int CMysz::X() const
	{
		return mX;
	}

	int CMysz::Y() const
	{
		return mY;
	}

	bool CMysz::ProceduraWiadomosci(HWND uchwytOkna, UINT wiadomosc, WPARAM parametrW, LPARAM parametrL)
	{
		switch (wiadomosc)
		{
		case WM_LBUTTONDOWN:
			mCzyNacisnietoLB = true;
			mX = LOWORD(parametrL);
			mY = HIWORD(parametrL);
			return true;

		case WM_LBUTTONUP:
			mCzyNacisnietoLB = false;
			return true;

		case WM_RBUTTONDOWN:
			mCzyNacisnietoRB = true;
			return true;

		case WM_RBUTTONUP:
			mCzyNacisnietoRB = false;
			return true;
		}

		return false;
	}
}