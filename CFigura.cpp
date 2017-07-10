#include "CFigura.h"
#include "CPlansza.h"

namespace Szachy
{
	HBITMAP CFigura::mBitmapaFigur = 0;
	HDC CFigura::mKontekstBitmapyFigur = 0;
	HBITMAP CFigura::mDomyslnaBitmapaKontekstuFigur = 0;

	CFigura::CFigura(const ETypFigury& typ, const EKolorFigury& kolor) :
		mTyp(typ), mKolor(kolor), mCzyZyje(true), mCzyWykonalRuch(false), mCzyWybrany(false),
		mZajmowanePole(0)
	{
	}

	CFigura::~CFigura()
	{
	}

	EKodBledu CFigura::Stworz(HWND uchwytOkna)
	{
		HDC appDC = GetDC(uchwytOkna);

		mBitmapaFigur = (HBITMAP)LoadImage(0, ".\\data\\figury.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (mBitmapaFigur)
		{
			mKontekstBitmapyFigur = CreateCompatibleDC(appDC);
			if (mKontekstBitmapyFigur)
				mDomyslnaBitmapaKontekstuFigur = (HBITMAP)SelectObject(mKontekstBitmapyFigur, mBitmapaFigur);
		}

		ReleaseDC(uchwytOkna, appDC);	

		if (!mBitmapaFigur)
			return KB_ODCZYT_PLIKU;

		if (!mKontekstBitmapyFigur)
			return KB_TWORZENIE_CONTEKSTU;

		return KB_BRAK;
	}
	
	void CFigura::Zniszcz()
	{
		DeleteObject(mBitmapaFigur);
		SelectObject(mKontekstBitmapyFigur, mDomyslnaBitmapaKontekstuFigur);
		DeleteDC(mKontekstBitmapyFigur);
		mBitmapaFigur = 0;
		mKontekstBitmapyFigur = 0;
		mDomyslnaBitmapaKontekstuFigur = 0;
	}

	void CFigura::Rysuj(HDC dc)
	{
		TransparentBlt(dc, mZajmowanePole->GornyX(), mZajmowanePole->GornyY(), ROZMIAR_GRAFIKI_FIGURY, ROZMIAR_GRAFIKI_FIGURY,
			mKontekstBitmapyFigur, (int)mTyp*ROZMIAR_GRAFIKI_FIGURY, mKolor == KF_BIALY ? 0 : ROZMIAR_GRAFIKI_FIGURY, 
			ROZMIAR_GRAFIKI_FIGURY,ROZMIAR_GRAFIKI_FIGURY, KOLOR_MASKI);

		if (mCzyWybrany)
		{
			TransparentBlt(dc, mZajmowanePole->GornyX(), mZajmowanePole->GornyY(), ROZMIAR_GRAFIKI_FIGURY, ROZMIAR_GRAFIKI_FIGURY,
				mKontekstBitmapyFigur, POZYCJA_ZNACZNIKA_SELEKCJI_W_BITMAPIE, 0,
				ROZMIAR_GRAFIKI_FIGURY, ROZMIAR_GRAFIKI_FIGURY, KOLOR_MASKI);
		}
	}

	bool CFigura::CzyAtakuje(CPlansza* plansza, CPole* pole)
	{ 
		return false; 
	}

	void CFigura::UstawKolor(const EKolorFigury& kolor)
	{
		mKolor = kolor;
	}

	void CFigura::UstawZajmowanePole(CPole* pole)
	{
		mZajmowanePole = pole;
	}

	void CFigura::UstawTyp(const ETypFigury& typ)
	{
		mTyp = typ;
	}

	void CFigura::UstawCzyZyje(const bool& b)
	{
		mCzyZyje = b;
	}

	bool CFigura::UstawCzyWybrany(CPlansza *plansza, const bool& b) 
	{
		mCzyWybrany = b;
		return SprawdzCzyBroniKrola(plansza);
	}

	void CFigura::UstawCzyWykonalRuch(const bool& b)
	{
		mCzyWykonalRuch = b;
	}

	ETypFigury CFigura::Typ() const
	{
		return mTyp;
	}

	EKolorFigury CFigura::Kolor() const
	{
		return mKolor;
	}

	CPole* CFigura::ZajmowanePole() const
	{
		return mZajmowanePole;
	}

	bool CFigura::CzyZyje() const
	{
		return mCzyZyje;
	}

	bool CFigura::CzyWykonalRuch() const
	{
		return mCzyWykonalRuch;
	}

	void CFigura::PrzesunNa(CPlansza* plansza, CPole *cel)
	{
		g_InfoRuchu.typRuchu = TR_NORMALNY;
		g_InfoRuchu.typFigury = mTyp;
		g_InfoRuchu.cel = cel;
		g_InfoRuchu.zrodlo = mZajmowanePole;
		g_InfoRuchu.kolorFigury = mKolor;

		if (!cel->CzyPuste())
		{
			g_InfoRuchu.typRuchu = TR_BICIE;
			cel->Figura()->UstawCzyZyje(false);
		}
	
		mZajmowanePole->UstawZajmujacego(0);
		mZajmowanePole = cel;
		cel->UstawZajmujacego(this);
		mCzyWykonalRuch = true;

		if (plansza->CzyKrolAtakowany())
		{
			if (cel == plansza->OstatniAtakujacy())
				plansza->UstawAtakujacego(0);
			if (SprawdzCzyBroniKrola(plansza))
				plansza->UstawAtakujacego(0);
		}
	}

	void CFigura::ZaznaczLinie(CPlansza* plansza, const int& kolumna, const int& wiersz)
	{
		CPole *pole;
		for (int i = 1; i < ROZMIAR_PLANSZY; ++i)
		{
			pole = plansza->Pole(mZajmowanePole->Litera() + (i*kolumna), mZajmowanePole->Numer() + (i*wiersz));
			if (pole)
			{
				if (plansza->CzyKrolAtakowany())
				{
					CPole *poleKrola = (mKolor == KF_BIALY) ? plansza->PoleBialegoKrola() : plansza->PoleCzarnegoKrola();
					if (poleKrola == pole)
						return;

					if (plansza->CzyBlokujeAtak(pole, mKolor))
					{
						if (!pole->CzyPuste())
							pole->UstawZaznaczenie(true, false);
						else pole->UstawZaznaczenie(false, true);
						plansza->ZaznaczPole(pole);
					}
					else if (!pole->CzyPuste())
					{
						return;
					}
				}
				else
				{
					if (!pole->CzyPuste())
					{
						if (pole->Figura()->Kolor() != mKolor)
						{
							pole->UstawZaznaczenie(true, false);
							plansza->ZaznaczPole(pole);
						}
						break;
					}
					pole->UstawZaznaczenie(false, true);
					plansza->ZaznaczPole(pole);
				}
			}
		}
	}

	void CFigura::ZaznaczPole(CPlansza* plansza, const int& kolumna, const int& wiersz)
	{
		CPole *pole;
		pole = plansza->Pole(mZajmowanePole->Litera() + kolumna, mZajmowanePole->Numer() + wiersz);
		if (pole)
		{
			if (plansza->CzyKrolAtakowany())
			{
				if (plansza->CzyBlokujeAtak(pole, mKolor))
				{
					if (!pole->CzyPuste())
						pole->UstawZaznaczenie(true, false);
					else pole->UstawZaznaczenie(false, true);
					plansza->ZaznaczPole(pole);
					return;
				}
			}
			else
			{
				if (!pole->CzyPuste())
				{
					if (pole->Figura()->Kolor() != mKolor)
					{
						pole->UstawZaznaczenie(true, false);
						plansza->ZaznaczPole(pole);
					}
				}
				else
				{
					pole->UstawZaznaczenie(false, true);
					plansza->ZaznaczPole(pole);
				}			
			}
		}
	}


	bool CFigura::SprawdzCzyBroniKrola(CPlansza* plansza)
	{
		if (mTyp == TF_KROL || plansza == 0)
			return false;

		CPole *poleKrola = (mKolor == KF_BIALY) ? plansza->PoleBialegoKrola() : plansza->PoleCzarnegoKrola();

		int kolumna = 0;
		int wiersz = 0;
		ZnajdzKolumneIWiersz(poleKrola, kolumna, wiersz);
		if (wiersz == 0 && kolumna == 0)
			return false;

		int i = 1;
		bool sprawdzony = false;
		bool cosZa = false;
		bool cosPrzed = false;
		bool nicPrzed = false;
		bool atakowany = false;
		for (int i = 1; !sprawdzony; ++i)
		{
			CPole *pole = plansza->Pole(poleKrola->Litera() + (i*kolumna), poleKrola->Numer() + (i*wiersz));
			if (pole)
			{
				if (!pole->CzyPuste())
				{
					if (!nicPrzed && pole != mZajmowanePole)
					{
						cosPrzed = true;
						sprawdzony = true;
					}
					else if (pole == mZajmowanePole)
					{
						nicPrzed = true;
					}
					else if (nicPrzed)
					{
						cosZa = true;
						if (pole->Figura()->Kolor() != mKolor)
						{
							if ((kolumna == ZAZNACZ_NA_LEWO || kolumna == ZAZNACZ_NA_PRAWO) && wiersz == ZAZNACZ_W_TYM_SAMYM)
							{
								if (pole->Figura()->Typ() == TF_WIEZA ||
									pole->Figura()->Typ() == TF_HETMAN)
									cosZa = false;
							}
							else if (kolumna == ZAZNACZ_W_TYM_SAMYM && (wiersz == ZAZNACZ_W_GORE || wiersz == ZAZNACZ_W_DOL))
							{
								if (pole->Figura()->Typ() == TF_WIEZA ||
									pole->Figura()->Typ() == TF_HETMAN)
									cosZa = false;
							}
							else if ((kolumna == ZAZNACZ_NA_LEWO || kolumna == ZAZNACZ_NA_PRAWO) && wiersz != ZAZNACZ_W_TYM_SAMYM)
							{
								if (pole->Figura()->Typ() == TF_GONIEC ||
									pole->Figura()->Typ() == TF_HETMAN)
									cosZa = false;
							}
						}

						if (!cosZa)
						{
							atakowany = true;
						}
						sprawdzony = true;
					}
				}
			}
			else
			{
				sprawdzony = true;
			}
		}

		if (atakowany && !cosPrzed && !cosZa)
			return true;

		return false;
	}

	bool CFigura::SprawdzLinie(CPlansza* plansza, CPole* sprawdzOd, const int& kolumna, const int& wiersz)
	{
		CPole *pole;
		bool sprawdzony = false;
		for (int i = 1; !sprawdzony; ++i)
		{		
			pole = plansza->Pole(sprawdzOd->Litera() + (i*kolumna), sprawdzOd->Numer() + (i*wiersz));	

			if (pole)
			{
				if (pole == mZajmowanePole)
				{
					return true;
				}

				if (!pole->CzyPuste())
					return false;
			}
			else sprawdzony = true;
		}

		return false;
	}

	void CFigura::ZnajdzKolumneIWiersz(CPole *other, int &kolumna, int& wiersz)
	{
		int literaPolaFigury = mZajmowanePole->Litera();
		int numerPolaFigury = mZajmowanePole->Numer();
		int literaInnegoPola = other->Litera();
		int numerInnegoPola = other->Numer();


		if (numerInnegoPola != numerPolaFigury)
		{
			if (abs(literaInnegoPola - literaPolaFigury) == abs(numerPolaFigury - numerInnegoPola) || literaPolaFigury == literaInnegoPola)
			{
				if (numerPolaFigury - numerInnegoPola < 0)
					wiersz = -1;
				else wiersz = 1;
			}
		}

		if (literaInnegoPola != literaPolaFigury)
		{
			if (abs(literaInnegoPola - literaPolaFigury) == abs(numerPolaFigury - numerInnegoPola) || numerPolaFigury == numerInnegoPola)
			{
				if (literaPolaFigury - literaInnegoPola < 0)
					kolumna = -1;
				else kolumna = 1;
			}
		}
	}

	bool CFigura::SprawdzPole(CPole* sprawdzOd, const int& kolumna, const int& wiersz)
	{
		if (mZajmowanePole->Numer() + wiersz == sprawdzOd->Numer() &&
			mZajmowanePole->Litera() + kolumna == sprawdzOd->Litera())
			return true;
		return false;
	}

}