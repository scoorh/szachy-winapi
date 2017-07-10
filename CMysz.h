#ifndef __C_MYSZ_H__
#define __C_MYSZ_H__

#include <Windows.h>
#include "EKodBledu.h"

namespace Szachy
{
	class CMysz
	{
		friend class CAplikacja;

	public:
		bool CzyNacisnietyLB() const;
		bool CzyNacisnietyRB() const;
		int X() const;
		int Y() const;

	private:
		CMysz();
		~CMysz();
		CMysz(const CMysz&);
		void operator= (const CMysz&);

		bool ProceduraWiadomosci(HWND uchwytOkna, UINT wiadomosc, WPARAM parametrW, LPARAM parametrL);

	private:
		int mX;
		int mY;
		bool mCzyNacisnietoLB;
		bool mCzyNacisnietoRB;
	};
}

#endif