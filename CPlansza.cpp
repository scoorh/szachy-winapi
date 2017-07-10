#include "CPlansza.h"
#include "ETypy.h"
#include "CFigura.h"
#include <iostream>
using namespace std;
namespace Szachy
{
	CPlansza::CPlansza() :
		mBitmapaTla(0),
		mKontekstTla(0),
		mDomyslnaBitmapaKontekstuTla(0),
		mBitmapaZaznaczenia(0),
		mKontekstZaznaczenia(0),
		mDomyslnaBitmapaKontekstuZazaczenia(0),
		mPoleEnPassant(0),
		mPoleBialegoKrola(0),
		mPoleCzarnegoKrola(0),
		mPoleAtakujacego(0),
		mFigury(0)
	{}

	CPlansza::~CPlansza()
	{}

	bool CPlansza::CzySzachMat() 
	{
		return mBrakMozliwosciRuchu;
	}

	bool CPlansza::SprawdzCzySzachMat(const EKolorFigury& aktywnyGracz)
	{
		if (mPoleAtakujacego == 0)
			return false;

		bool szachmat = false;
		CPole* krol = (aktywnyGracz == KF_BIALY) ? mPoleCzarnegoKrola : mPoleBialegoKrola;

		CzyscZaznaczenie();
		if (!CzyPoleAtakowane(mPoleAtakujacego, aktywnyGracz))
		{
			bool czyMozeBronic = false;
			for (int i = 0; i < ILOSC_FIGUR && !czyMozeBronic; ++i)
			{
				if (mFigury[i] && mFigury[i]->CzyZyje() && mFigury[i]->Kolor() != aktywnyGracz)
				{
					mFigury[i]->UstawCzyWybrany(this, true);
					if (mZaznaczonePola.size() != 0)
					{	
						cout << mFigury[i]->ZajmowanePole()->Litera() << mFigury[i]->ZajmowanePole()->Numer() << endl;
						czyMozeBronic = true;
					}
					mFigury[i]->UstawCzyWybrany(0, false);
					CzyscZaznaczenie();
				}
			}

			if (!czyMozeBronic)
			{
				krol->Figura()->UstawCzyWybrany(0, true);
				if (mZaznaczonePola.size() == 0)
				{
					szachmat = true;
				}
				krol->Figura()->UstawCzyWybrany(0, false);
			}
		}


		if (!CzyPoleAtakowane(mPoleAtakujacego, aktywnyGracz, true))
		{
			bool czyMozeBronic = false;
			for (int i = 0; i < ILOSC_FIGUR && !czyMozeBronic; ++i)
			{
				if (mFigury[i] && mFigury[i]->CzyZyje() && mFigury[i]->Kolor() != aktywnyGracz)
				{
					mFigury[i]->UstawCzyWybrany(this, true);
					if (mZaznaczonePola.size() != 0)
					{
						cout << mFigury[i]->ZajmowanePole()->Litera() << mFigury[i]->ZajmowanePole()->Numer() << endl;
						czyMozeBronic = true;
					}
					mFigury[i]->UstawCzyWybrany(0, false);
					CzyscZaznaczenie();
				}
			}

			if (!czyMozeBronic)
				szachmat = true;
		}

		if (szachmat)
		{
			CzyscZaznaczenie();
			krol->UstawZaznaczenie(true, false);
			mPoleAtakujacego->UstawZaznaczenie(true, false);
			ZaznaczPole(krol);
			ZaznaczPole(mPoleAtakujacego);
			return true;
		}

		return false;
	}

	bool CPlansza::CzyBlokujeAtak(CPole* pole, const EKolorFigury& kolor)
	{
		if (mPoleAtakujacego->Litera() == pole->Litera() &&
			mPoleAtakujacego->Numer() == pole->Numer())
			return true;

		CPole* poleKrola = (kolor == KF_BIALY) ? mPoleBialegoKrola : mPoleCzarnegoKrola;
		
		if (mPoleAtakujacego->Numer() == poleKrola->Numer())
		{
			if (mPoleAtakujacego->Litera() < pole->Litera() &&
				poleKrola->Litera() > pole->Litera() &&
				pole->Numer() == mPoleAtakujacego->Numer())
				return true;
			if (mPoleAtakujacego->Litera() > pole->Litera() &&
				poleKrola->Litera() < pole->Litera() &&
				pole->Numer() == mPoleAtakujacego->Numer())
				return true;
		}
		if (mPoleAtakujacego->Litera() == poleKrola->Litera())
		{
			if (mPoleAtakujacego->Numer() > pole->Numer() &&
				poleKrola->Numer() < pole->Numer() &&
				pole->Litera() == mPoleAtakujacego->Litera())
				return true;
			if (mPoleAtakujacego->Numer() < pole->Numer() &&
				poleKrola->Numer() > pole->Numer() &&
				pole->Litera() == mPoleAtakujacego->Litera())
				return true;
		}	
		
		int a = abs(mPoleAtakujacego->Litera() - pole->Litera());
		int b = abs(mPoleAtakujacego->Numer() - pole->Numer());
		int c = abs(poleKrola->Litera() - pole->Litera());
		int d = abs(poleKrola->Numer() - pole->Numer());
		
		if (a == b && c == d)
		{
			if (mPoleAtakujacego->Litera() < poleKrola->Litera())
			{
				if (mPoleAtakujacego->Litera() < pole->Litera() &&
					poleKrola->Litera() > pole->Litera())
					return true;
			}
			if (mPoleAtakujacego->Litera() > poleKrola->Litera())
			{
				if (mPoleAtakujacego->Litera() > pole->Litera() &&
					poleKrola->Litera() < pole->Litera())
					return true;
			}
		}

		return false;
	}


	bool CPlansza::CzyPoleAtakowane(CPole* doSprawdzenia, const EKolorFigury& kolor, const bool& ignorujKrola)
	{
		for (int i = 0; i < ILOSC_FIGUR; ++i)
		{
			if (mFigury[i] && mFigury[i]->CzyZyje() &&
				mFigury[i]->Kolor() != kolor)
			{
				if (mFigury[i]->Typ() == TF_KROL && ignorujKrola)
					continue;

				if (mFigury[i]->CzyAtakuje(this, doSprawdzenia))
				{
					if (doSprawdzenia && !doSprawdzenia->CzyPuste() && doSprawdzenia->Figura()->Typ() == TF_KROL)	
						mPoleAtakujacego = mFigury[i]->ZajmowanePole();
					return true;
				}
			}
		}
		return false;
	}


	void CPlansza::SprawdzCzyKrolJestAtakowany(const EKolorFigury& aktywnyGracz)
	{
		CPole* krol = (aktywnyGracz == KF_BIALY) ? mPoleCzarnegoKrola : mPoleBialegoKrola;
		if (CzyPoleAtakowane(krol, (aktywnyGracz == KF_BIALY) ? KF_CZARNY : KF_BIALY))
			mCzyKrolAtakowany = true;
		else mCzyKrolAtakowany = false;

		if (mCzyKrolAtakowany)
		{
			g_InfoRuchu.typSzachu = TR_SZACH;
			if (SprawdzCzySzachMat(aktywnyGracz))
			{
				g_InfoRuchu.typSzachu = TR_SZACHMAT;
				mBrakMozliwosciRuchu = true;
			}
		}
		else
		{
			g_InfoRuchu.typSzachu = TR_BRAK_SZACHU;
		}
	}

	bool CPlansza::CzyKrolAtakowany() const
	{
		return mCzyKrolAtakowany;
	}

	CPole* CPlansza::NacisnietePole(const int& myszX, const int& myszY, const bool& marked)
	{
		if (marked)
		{
			for (std::vector<CPole*>::iterator i = mZaznaczonePola.begin(); i != mZaznaczonePola.end(); ++i)
			{
				if ((*i)->CzyNacisniety(myszX,myszY))
					return (*i);
			}
		}
		else
		{
			for (int i = 0; i < ROZMIAR_PLANSZY*ROZMIAR_PLANSZY; ++i)
			{
				if (mPola[i].CzyNacisniety(myszX, myszY))
					return &mPola[i];
			}
		}
		return 0;
	}

	void CPlansza::ZaznaczPole(CPole* pole)
	{
		mZaznaczonePola.push_back(pole);
	}

	void CPlansza::CzyscZaznaczenie()
	{
		for (std::vector<CPole*>::iterator i = mZaznaczonePola.begin(); i != mZaznaczonePola.end(); ++i)
			(*i)->UstawZaznaczenie(false, false);
		mZaznaczonePola.clear();
	}

	void CPlansza::Rysuj(HDC dc)
	{
		BitBlt(dc, mPozycjaX, mPozycjaY, ROZMIAR_GRAFIKI_PLANSZY, ROZMIAR_GRAFIKI_PLANSZY, mKontekstTla, 0, 0, SRCCOPY);
		
		for (std::vector<CPole*>::iterator i = mZaznaczonePola.begin(); i != mZaznaczonePola.end(); ++i)
		{
			TransparentBlt(dc, (*i)->GornyX(), (*i)->GornyY(), ROZMIAR_POLA_PLANSZY, ROZMIAR_POLA_PLANSZY, mKontekstZaznaczenia,
				(*i)->OznaczonyDoRuchu() ? 0 : ROZMIAR_POLA_PLANSZY, 0, ROZMIAR_POLA_PLANSZY, ROZMIAR_POLA_PLANSZY, KOLOR_MASKI);
		}
	}

	EKodBledu CPlansza::Stworz(const int& x, const int& y, HWND uchwytOkna, CFigura **figury)
	{
		mPozycjaX = x;
		mPozycjaY = y;
		mFigury = figury;

		HDC appDC = GetDC(uchwytOkna);

		mBitmapaTla = (HBITMAP)LoadImage(0, ".\\data\\tlo_planszy.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (mBitmapaTla)
		{
			mKontekstTla = CreateCompatibleDC(appDC);
			if (mKontekstTla)
				mDomyslnaBitmapaKontekstuTla = (HBITMAP)SelectObject(mKontekstTla, mBitmapaTla);
		}

		mBitmapaZaznaczenia = (HBITMAP)LoadImage(0, ".\\data\\zaznaczenie_pol.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (mBitmapaZaznaczenia)
		{
			mKontekstZaznaczenia = CreateCompatibleDC(appDC);
			if (mKontekstZaznaczenia)
				mDomyslnaBitmapaKontekstuZazaczenia = (HBITMAP)SelectObject(mKontekstZaznaczenia, mBitmapaZaznaczenia);
		}

		ReleaseDC(uchwytOkna, appDC);

		if (!mBitmapaTla || !mBitmapaZaznaczenia)
			return KB_ODCZYT_PLIKU;

		if (!mKontekstTla || !mKontekstZaznaczenia)
			return KB_TWORZENIE_CONTEKSTU;

		int poleNumber = 8;
		for (int i = 0; i < ROZMIAR_PLANSZY; ++i)
		{		
			char poleLetter = 'A';
			for (int j = 0; j < ROZMIAR_PLANSZY; ++j)
			{
				int index = i*ROZMIAR_PLANSZY + j;
				CPole *pole = &mPola[index];
				pole->Stworz(poleLetter, poleNumber, index);
				++poleLetter;
			}
			--poleNumber;
		}

		return KB_BRAK;
	}

	void CPlansza::Zniszcz()
	{
		DeleteObject(mBitmapaTla);
		SelectObject(mKontekstTla, mDomyslnaBitmapaKontekstuTla);
		DeleteDC(mKontekstTla);
		mBitmapaTla = 0;
		mKontekstTla = 0;
		mDomyslnaBitmapaKontekstuTla = 0;

		DeleteObject(mBitmapaZaznaczenia);
		SelectObject(mKontekstZaznaczenia, mDomyslnaBitmapaKontekstuZazaczenia);
		DeleteDC(mKontekstTla);
		mBitmapaZaznaczenia = 0;
		mKontekstZaznaczenia = 0;
		mDomyslnaBitmapaKontekstuZazaczenia = 0;
	}

	CPole* CPlansza::Pole(const char& litera, const int& numer)
	{
		int index = (ROZMIAR_PLANSZY - numer)*ROZMIAR_PLANSZY + (int)(litera - 'A');
		if (index < 0 || index >= ROZMIAR_PLANSZY * ROZMIAR_PLANSZY)
			return 0;

		if (mPola[index].Litera() != litera || mPola[index].Numer() != numer)
			return 0;

		return &mPola[index];
	}

	void CPlansza::UstawMozliwoscEnPassant(CPole *pole)
	{
		mPoleEnPassant = pole;
	}
	
	bool CPlansza::CzyEnPassantMozliwy() const
	{
		return mPoleEnPassant != 0;
	}

	CPole* CPlansza::PoleEnPassant() const
	{
		return mPoleEnPassant;
	}

	void CPlansza::UstawPoleBialegoKrola(CPole* pole)
	{
		mPoleBialegoKrola = pole;
	}

	void CPlansza::UstawPoleCzarnegoKrola(CPole* pole)
	{
		mPoleCzarnegoKrola = pole;
	}

	CPole* CPlansza::PoleBialegoKrola() const
	{
		return mPoleBialegoKrola;
	}

	CPole* CPlansza::PoleCzarnegoKrola() const
	{
		return mPoleCzarnegoKrola;
	}

	void CPlansza::Resetuj()
	{
		mBrakMozliwosciRuchu = 0;
		mCzyKrolAtakowany = false;
		mPoleEnPassant = 0;
		mPoleBialegoKrola = 0;
		mPoleCzarnegoKrola = 0;
		mPoleAtakujacego = 0;
		for (int i = 0; i < ROZMIAR_PLANSZY*ROZMIAR_PLANSZY; ++i)
			mPola[i].UstawZajmujacego(0);
		CzyscZaznaczenie();
	}

	void CPlansza::UstawAtakujacego(CPole* pole)
	{
		mPoleAtakujacego = pole;
	}

	CPole *CPlansza::OstatniAtakujacy() const
	{
		return mPoleAtakujacego;
	}

}