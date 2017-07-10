#include "CAplikacja.h"
#include <iostream>
using namespace std;

#define RETURN_IF_ERROR(c) \
	{\
		EKodBledu e =  c; \
		if(e != KB_BRAK) \
			return e; \
	}

namespace Szachy
{
	CAplikacja* CAplikacja::msAplikacja = 0;

	EKodBledu CAplikacja::Stworz(SUstawieniaAplikacji* ustawienia)
	{
		if (!ustawienia)
			return KB_PUSTY_WSKAZNIK;

		if (!msAplikacja)
		{
			msAplikacja = new CAplikacja();
			if (!msAplikacja)
				return KB_BRAK_PAMIECI;
			return msAplikacja->Inicjalizuj(ustawienia);
		}
		return KB_BRAK;
	}

	void CAplikacja::Zwolnij()
	{
		msAplikacja->Zniszcz();
		delete msAplikacja;
		msAplikacja = 0;
	}

	EKodBledu CAplikacja::Wykonuj()
	{
		return msAplikacja->_Wykonuj();
	}

	CAplikacja::CAplikacja() :
		mZasobyStworzone(false),
		mRysuj(false),
		mWybranaFigura(0),
		mPrzyciskRestart(0),
		mPrzyciskWyjscie(0),
		mListaWiadomosciBialy(0),
		mListaWiadomosciCzarny(0)
	{
		for (int i = 0; i < ILOSC_FIGUR; ++i)
			mFigury[i] = 0;
	}

	CAplikacja::~CAplikacja()
	{
	}

	EKodBledu CAplikacja::Inicjalizuj(SUstawieniaAplikacji* ustawienia)
	{
		mUstawienia = (*ustawienia);

		RETURN_IF_ERROR(mOkno.Stworz(ustawienia));
		RETURN_IF_ERROR(mPlansza.Stworz(0, 0, mOkno.UchwytOkna(), mFigury));
		RETURN_IF_ERROR(CFigura::Stworz(mOkno.UchwytOkna()));

		mPrzyciskRestart = CreateWindowEx(0, "BUTTON", "RESETUJ", WS_CHILD | WS_VISIBLE, 610, 450, 160, 64, mOkno.UchwytOkna(), 0, ustawienia->instancja, 0);
		mPrzyciskWyjscie = CreateWindowEx(0, "BUTTON", "WYJSCIE", WS_CHILD | WS_VISIBLE, 800, 450, 160, 64, mOkno.UchwytOkna(), 0, ustawienia->instancja, 0);
		mListaWiadomosciBialy = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", 0, WS_CHILD | WS_VISIBLE | WS_BORDER, 610, 50, 175, 380, mOkno.UchwytOkna(), 0, ustawienia->instancja, 0);
		mListaWiadomosciCzarny = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", 0, WS_CHILD | WS_VISIBLE | WS_BORDER, 785, 50, 175, 380, mOkno.UchwytOkna(), 0, ustawienia->instancja, 0);
		g_ListaWidomosciBialyGracz = mListaWiadomosciBialy;
		g_ListaWidomosciCzarnyGracz = mListaWiadomosciCzarny;

		Resetuj();
		mZasobyStworzone = true;
		mRysuj = true;
		return KB_BRAK;
	}

	void CAplikacja::Zniszcz()
	{
		g_ListaWidomosciBialyGracz = 0;
		g_ListaWidomosciCzarnyGracz = 0;
		DestroyWindow(mPrzyciskRestart);
		DestroyWindow(mPrzyciskWyjscie);
		DestroyWindow(mListaWiadomosciCzarny);
		DestroyWindow(mListaWiadomosciBialy);
		mPlansza.Zniszcz();
		mOkno.Zniszcz();
		mZasobyStworzone = false;
		for (int i = 0; i < ILOSC_FIGUR; ++i)
		{
			delete mFigury[i];
			mFigury[i] = 0;
		}
	}

	EKodBledu CAplikacja::_Wykonuj()
	{
		mOkno.PokazOkno(true);

		do
		{
			mOkno.PompaWiadomosci();
			if (mOkno.CzyZadaZamkniecia())
				break;

			if (mPlansza.CzySzachMat())
				continue;

			if (WybierzFigure())
				mRysuj = true;
			if (mWybranaFigura)
			{
				if (PrzesunFigure())
				{
					mPlansza.SprawdzCzyKrolJestAtakowany(mAktywnyGracz);
					ZapiszInfoRuchu();

					if (mAktywnyGracz == KF_BIALY)
						mAktywnyGracz = KF_CZARNY;
					else
						mAktywnyGracz = KF_BIALY;

					mRysuj = true;
				}
			}

		} while (true);

		return KB_BRAK;
	}

	bool CAplikacja::WybierzFigure()
	{
		if (mMysz.CzyNacisnietyRB() && mWybranaFigura)
		{
			mWybranaFigura->UstawCzyWybrany(0, false);
			mWybranaFigura = 0;
			mPlansza.CzyscZaznaczenie();
			mRysuj = true;
			return false;
		}

		if (!mMysz.CzyNacisnietyLB() || mWybranaFigura)
			return false;

		int i;
		mAktywnyGracz == KF_BIALY ? i = PIERWSZA_BIALA_FIGURA_W_TABELI : i = PIERWSZA_CZARNA_FIGURA_W_TABELI;
		for (; i < ILOSC_FIGUR; ++i)
		{
			if (mFigury[i] && mFigury[i]->CzyZyje())
			{	
				if (mFigury[i]->ZajmowanePole()->CzyNacisniety(mMysz.X(), mMysz.Y()) &&
					mFigury[i]->Kolor() == mAktywnyGracz)
				{
					mWybranaFigura = mFigury[i];
					mWybranaFigura->UstawCzyWybrany(&mPlansza, true);
					return true;
				}
			}
		}

		return false;
	}
	

	bool CAplikacja::PrzesunFigure()
	{
		if (!mMysz.CzyNacisnietyLB())
			return false;

		CPole *cel = mPlansza.NacisnietePole(mMysz.X(), mMysz.Y(), true);
		if (cel)
		{
			mWybranaFigura->PrzesunNa(&mPlansza, cel);
			mWybranaFigura->UstawCzyWybrany(0, false);
			if (mWybranaFigura->Typ() != TF_PIONEK)
				mPlansza.UstawMozliwoscEnPassant(0);

			if (mWybranaFigura->Typ() == TF_PIONEK)
			{
				if (((CPionek*)(mWybranaFigura))->CzyPromocja())
				{	
					for (int i = 0; i < ILOSC_FIGUR; ++i)
					{
						if (mFigury[i] == mWybranaFigura)
						{
							EKolorFigury kolor = mFigury[i]->Kolor();
							CPole* pole = mFigury[i]->ZajmowanePole();
							mFigury[i] = new CHetman(kolor);
							mFigury[i]->UstawZajmowanePole(pole);
							mFigury[i]->UstawCzyZyje(true);
							mFigury[i]->UstawCzyWykonalRuch(false);
							mFigury[i]->UstawCzyWybrany(0, false);
							break;
						}
					}
				}
			}
			mWybranaFigura = 0;
			mPlansza.CzyscZaznaczenie();
			return true;
		}

		return false;
	}

	LRESULT CALLBACK CAplikacja::Win32WndProc(HWND uchwytOkna, UINT wiadomosc, WPARAM parametrW, LPARAM parametrL)
	{
		msAplikacja->mOkno.ProceduraWiadomosci(uchwytOkna, wiadomosc, parametrW, parametrL);
		msAplikacja->mMysz.ProceduraWiadomosci(uchwytOkna, wiadomosc, parametrW, parametrL);

		switch (wiadomosc)
		{
		case WM_COMMAND:
			if ((HWND)parametrL == msAplikacja->mPrzyciskRestart)
			{
				msAplikacja->Resetuj();
				msAplikacja->mRysuj = true;
			}
			else if ((HWND)parametrL == msAplikacja->mPrzyciskWyjscie)
				PostQuitMessage(0);
			return 0;

		case WM_ACTIVATE:
			msAplikacja->mRysuj = true;
			return 0;

		case WM_EXITSIZEMOVE:
			msAplikacja->mRysuj = true;
			return 0;

		case WM_PAINT:
			if (!msAplikacja->mRysuj)
				return 0;

			HWND hAppWnd = msAplikacja->mOkno.UchwytOkna();
			HDC hAppDC = GetDC(hAppWnd);

			msAplikacja->mPlansza.Rysuj(hAppDC);

			for (int i = 0; i < ILOSC_FIGUR; ++i)
			{
				if (msAplikacja->mFigury[i] &&
					msAplikacja->mFigury[i]->CzyZyje())
					msAplikacja->mFigury[i]->Rysuj(hAppDC);
			}

			ReleaseDC(hAppWnd, hAppDC);
			msAplikacja->mRysuj = false;
			return 0;
		}

		return DefWindowProc(uchwytOkna, wiadomosc, parametrW, parametrL);
	}

	void CAplikacja::Resetuj()
	{
		mWybranaFigura = 0;
		mRysuj = true;
		mPlansza.Resetuj();
		ResetujInfoRuchu();

		mAktywnyGracz = KF_BIALY;
	
		CPole* pole;
		for (int i = 0; i < 8; ++i)
		{	
			if (mFigury[i])
			{
				delete mFigury[i];
				mFigury[i] = 0;
			}
			if (!mFigury[i])
				mFigury[i] = new CPionek(KF_BIALY);
			pole = mPlansza.Pole('A' + i, 2);
			pole->UstawZajmujacego(mFigury[i]);
			mFigury[i]->UstawZajmowanePole(pole);
			mFigury[i]->UstawCzyZyje(true);
			mFigury[i]->UstawCzyWykonalRuch(false);
			mFigury[i]->UstawCzyWybrany(0, false);
		}

		if (!mFigury[8]) mFigury[8] = new CWieza(KF_BIALY);
		pole = mPlansza.Pole('A', 1);
		pole->UstawZajmujacego(mFigury[8]);
		mFigury[8]->UstawZajmowanePole(pole);
		mFigury[8]->UstawCzyZyje(true);
		mFigury[8]->UstawCzyWykonalRuch(false);
		mFigury[8]->UstawCzyWybrany(0, false);
		
		if (!mFigury[9]) mFigury[9] = new CWieza(KF_BIALY);
		pole = mPlansza.Pole('H', 1);
		pole->UstawZajmujacego(mFigury[9]);
		mFigury[9]->UstawZajmowanePole(pole);
		mFigury[9]->UstawCzyZyje(true);
		mFigury[9]->UstawCzyWykonalRuch(false);
		mFigury[9]->UstawCzyWybrany(0, false);
		
		if (!mFigury[10]) mFigury[10] = new CSkoczek(KF_BIALY);
		pole = mPlansza.Pole('B', 1);
		pole->UstawZajmujacego(mFigury[10]);
		mFigury[10]->UstawZajmowanePole(pole);
		mFigury[10]->UstawCzyZyje(true);
		mFigury[10]->UstawCzyWykonalRuch(false);
		mFigury[10]->UstawCzyWybrany(0, false);

		if (!mFigury[11]) mFigury[11] = new CSkoczek(KF_BIALY);
		pole = mPlansza.Pole('G', 1);
		pole->UstawZajmujacego(mFigury[11]);
		mFigury[11]->UstawZajmowanePole(pole);
		mFigury[11]->UstawCzyZyje(true);
		mFigury[11]->UstawCzyWykonalRuch(false);
		mFigury[11]->UstawCzyWybrany(0, false);

		if (!mFigury[12]) mFigury[12] = new CGoniec(KF_BIALY);
		pole = mPlansza.Pole('C', 1);
		pole->UstawZajmujacego(mFigury[12]);
		mFigury[12]->UstawZajmowanePole(pole);
		mFigury[12]->UstawCzyZyje(true);
		mFigury[12]->UstawCzyWykonalRuch(false);
		mFigury[12]->UstawCzyWybrany(0, false);

		if (!mFigury[13]) mFigury[13] = new CGoniec(KF_BIALY);
		pole = mPlansza.Pole('F', 1);
		pole->UstawZajmujacego(mFigury[13]);
		mFigury[13]->UstawZajmowanePole(pole);
		mFigury[13]->UstawCzyZyje(true);
		mFigury[13]->UstawCzyWykonalRuch(false);
		mFigury[13]->UstawCzyWybrany(0, false);

		if (!mFigury[14]) mFigury[14] = new CHetman(KF_BIALY);
		pole = mPlansza.Pole('D', 1);
		pole->UstawZajmujacego(mFigury[14]);
		mFigury[14]->UstawZajmowanePole(pole);
		mFigury[14]->UstawCzyZyje(true);
		mFigury[14]->UstawCzyWykonalRuch(false);
		mFigury[14]->UstawCzyWybrany(0, false);

		if (!mFigury[15]) mFigury[15] = new CKrol(KF_BIALY);
		pole = mPlansza.Pole('E', 1);
		pole->UstawZajmujacego(mFigury[15]);
		mFigury[15]->UstawZajmowanePole(pole);
		mFigury[15]->UstawCzyZyje(true);
		mFigury[15]->UstawCzyWykonalRuch(false);
		mFigury[15]->UstawCzyWybrany(0, false);

		for (int i = 0; i < 8; ++i)
		{
			if (mFigury[i + 16])
			{
				delete mFigury[i + 16];
				mFigury[i + 16] = 0;
			}
			if (!mFigury[i+16])
				mFigury[i+16] = new CPionek(KF_CZARNY);
			pole = mPlansza.Pole('A' + i, 7);
			pole->UstawZajmujacego(mFigury[i + 16]);
			mFigury[i + 16]->UstawZajmowanePole(pole);
			mFigury[i + 16]->UstawCzyZyje(true);
			mFigury[i + 16]->UstawCzyWykonalRuch(false);
			mFigury[i + 16]->UstawCzyWybrany(0, false);
		}

		if (!mFigury[24]) mFigury[24] = new CWieza(KF_CZARNY);
		pole = mPlansza.Pole('A', 8);
		pole->UstawZajmujacego(mFigury[24]);
		mFigury[24]->UstawZajmowanePole(pole);
		mFigury[24]->UstawCzyZyje(true);
		mFigury[24]->UstawCzyWykonalRuch(false);
		mFigury[24]->UstawCzyWybrany(0, false);
		
		if (!mFigury[25]) mFigury[25] = new CWieza(KF_CZARNY);
		pole = mPlansza.Pole('H', 8);
		pole->UstawZajmujacego(mFigury[25]);
		mFigury[25]->UstawZajmowanePole(pole);
		mFigury[25]->UstawCzyZyje(true);
		mFigury[25]->UstawCzyWykonalRuch(false);
		mFigury[25]->UstawCzyWybrany(0, false);
		
		if (!mFigury[26]) mFigury[26] = new CSkoczek(KF_CZARNY);
		pole = mPlansza.Pole('B', 8);
		pole->UstawZajmujacego(mFigury[26]);
		mFigury[26]->UstawZajmowanePole(pole);
		mFigury[26]->UstawCzyZyje(true);
		mFigury[26]->UstawCzyWykonalRuch(false);
		mFigury[26]->UstawCzyWybrany(0, false);

		if (!mFigury[27]) mFigury[27] = new CSkoczek(KF_CZARNY);
		pole = mPlansza.Pole('G', 8);
		pole->UstawZajmujacego(mFigury[27]);
		mFigury[27]->UstawZajmowanePole(pole);
		mFigury[27]->UstawCzyZyje(true);
		mFigury[27]->UstawCzyWykonalRuch(false);
		mFigury[27]->UstawCzyWybrany(0, false);

		if (!mFigury[28]) mFigury[28] = new CGoniec(KF_CZARNY);
		pole = mPlansza.Pole('C', 8);
		pole->UstawZajmujacego(mFigury[28]);
		mFigury[28]->UstawZajmowanePole(pole);
		mFigury[28]->UstawCzyZyje(true);
		mFigury[28]->UstawCzyWykonalRuch(false);
		mFigury[28]->UstawCzyWybrany(0, false);

		if (!mFigury[29]) mFigury[29] = new CGoniec(KF_CZARNY);
		pole = mPlansza.Pole('F', 8);
		pole->UstawZajmujacego(mFigury[29]);
		mFigury[29]->UstawZajmowanePole(pole);
		mFigury[29]->UstawCzyZyje(true);
		mFigury[29]->UstawCzyWykonalRuch(false);
		mFigury[29]->UstawCzyWybrany(0, false);

		if (!mFigury[30]) mFigury[30] = new CHetman(KF_CZARNY);
		pole = mPlansza.Pole('D', 8);
		pole->UstawZajmujacego(mFigury[30]);
		mFigury[30]->UstawZajmowanePole(pole);
		mFigury[30]->UstawCzyZyje(true);
		mFigury[30]->UstawCzyWykonalRuch(false);
		mFigury[30]->UstawCzyWybrany(0, false);

		if (!mFigury[31]) mFigury[31] = new CKrol(KF_CZARNY);
		pole = mPlansza.Pole('E', 8);
		pole->UstawZajmujacego(mFigury[31]);
		mFigury[31]->UstawZajmowanePole(pole);
		mFigury[31]->UstawCzyZyje(true);
		mFigury[31]->UstawCzyWykonalRuch(false);
		mFigury[31]->UstawCzyWybrany(0, false);


		mPlansza.UstawPoleBialegoKrola(mFigury[15]->ZajmowanePole());
		mPlansza.UstawPoleCzarnegoKrola(mFigury[31]->ZajmowanePole());
	}
}