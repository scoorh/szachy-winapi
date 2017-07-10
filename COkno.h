#ifndef __C_OKNO_H__
#define __C_OKNO_H__

#include "EKodBledu.h"
#include "SUstawieniaAplikacji.h"
#include <string>

namespace Szachy
{
	class COkno
	{
		friend class CAplikacja;

	public:
		bool CzyZadaZamkniecia() const;
		void PokazOkno(const bool& pokaz);
		HWND UchwytOkna() const;

	private:
		COkno();
		~COkno();
		void operator= (const COkno&);
		COkno(const COkno&);
	
		EKodBledu Stworz(SUstawieniaAplikacji* ustawienia);
		void Zniszcz();
		void PompaWiadomosci();

		bool ProceduraWiadomosci(HWND uchwytOkna, UINT wiadomosc, WPARAM parametrW, LPARAM parametrL);

	private:
		HWND mUchwytOkna;
		bool mCzyZarejestrowanoKlaseOkna;
		SUstawieniaAplikacji *mAppustawienia;
		bool mCzyZadaZakmniecia;
		bool mCzyZminimalizowane;
		bool mCzyZmaksymalizowane;
		bool mCzySchowane;
	};
}

#endif