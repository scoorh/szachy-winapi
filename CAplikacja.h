#ifndef __C_APLIKACJA_H__
#define __C_APLIKACJA_H__

#include "Common.h"
#include "EKodBledu.h"
#include "SUstawieniaAplikacji.h"
#include "SListaWiadomosci.h"
#include "COkno.h"
#include "CMysz.h"
#include "CPlansza.h"
#include "CFigura.h"
#include "CPionek.h"
#include "CWieza.h"
#include "CGoniec.h"
#include "CSkoczek.h"
#include "CHetman.h"
#include "CKrol.h"

#pragma comment(lib,"msimg32")

namespace Szachy
{
	class CAplikacja
	{
	public:
		static EKodBledu Stworz(SUstawieniaAplikacji* ustawienia);
		static void Zwolnij();
		static EKodBledu Wykonuj();

		static LRESULT CALLBACK Win32WndProc(HWND uchwytOkna, UINT wiadomosc, WPARAM parametrW, LPARAM parametrL);

	private:
		CAplikacja();	
		~CAplikacja();
		CAplikacja(const CAplikacja&);
		void operator= (const CAplikacja&);

		EKodBledu Inicjalizuj(SUstawieniaAplikacji* ustawienia);
		void Zniszcz();
		EKodBledu _Wykonuj();	

		void Resetuj();
		bool WybierzFigure();
		void ZaznaczPola();
		bool PrzesunFigure();

	private:
		static CAplikacja *msAplikacja;
		SUstawieniaAplikacji mUstawienia;
		COkno mOkno;
		CMysz mMysz;
		CPlansza mPlansza;
		CFigura *mFigury[ILOSC_FIGUR];
		CFigura *mWybranaFigura;
		EKolorFigury mAktywnyGracz;
		bool mRysuj;
		bool mZasobyStworzone;
		HWND mPrzyciskRestart;
		HWND mPrzyciskWyjscie;
		HWND mListaWiadomosciBialy;
		HWND mListaWiadomosciCzarny;
	};
}

#endif