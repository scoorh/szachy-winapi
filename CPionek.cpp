#include "CPionek.h"
#include "CPlansza.h"

namespace Szachy
{
	CPionek::CPionek(const EKolorFigury& kolor) :
		CFigura(TF_PIONEK, kolor)
	{
		mPromocja = false;
	}

	CPionek::~CPionek()
	{
	}

	void CPionek::PrzesunNa(CPlansza* plansza, CPole *cel)
	{
		g_InfoRuchu.typFigury = TF_PIONEK;
		g_InfoRuchu.zrodlo = mZajmowanePole;
		g_InfoRuchu.cel = cel;
		g_InfoRuchu.typRuchu = TR_PIONEK;
		g_InfoRuchu.kolorFigury = mKolor;

		if (!plansza->CzyEnPassantMozliwy())
		{
			int wiersz = (mKolor == KF_BIALY) ? 4 : 5;
			if (cel->Numer() == wiersz && !mCzyWykonalRuch)
				plansza->UstawMozliwoscEnPassant(cel);	
		}
		else if (plansza->CzyEnPassantMozliwy())
		{
			int wiersz = (mKolor == KF_BIALY) ? 1 : -1;
			if (cel->Numer() == plansza->PoleEnPassant()->Numer() + wiersz &&
				cel->Litera() == plansza->PoleEnPassant()->Litera())
			{
				g_InfoRuchu.typRuchu = TR_ENPASSANT;
				plansza->PoleEnPassant()->Figura()->UstawCzyZyje(false);
				plansza->PoleEnPassant()->UstawZajmujacego(0);
			}

			plansza->UstawMozliwoscEnPassant(0);
		}	

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

		if (mZajmowanePole->Numer() == 8 || mZajmowanePole->Numer() == 1)
		{
			mPromocja = true;
		}
	}

	bool CPionek::UstawCzyWybrany(CPlansza* plansza, const bool& b)
	{
		if (CFigura::UstawCzyWybrany(plansza, b) || plansza == 0)
			return false;

		CPole *pole;
		int wiersz = (mKolor == KF_BIALY) ? 1 : -1;

		pole = plansza->Pole(mZajmowanePole->Litera(), mZajmowanePole->Numer() + wiersz);
		if (pole && pole->CzyPuste())
		{
			OznaczPole(plansza, pole);

			if (mZajmowanePole->Numer() == 2 || mZajmowanePole->Numer() == 7)
			{
				pole = plansza->Pole(mZajmowanePole->Litera(), mZajmowanePole->Numer() + wiersz * 2);
				if (pole && pole->CzyPuste())
					OznaczPole(plansza, pole);
			}
		}

		pole = plansza->Pole(mZajmowanePole->Litera() - 1, mZajmowanePole->Numer() + wiersz);
		if (pole && !pole->CzyPuste() && pole->Figura()->Kolor() != mKolor)
			OznaczPole(plansza, pole);

		pole = plansza->Pole(mZajmowanePole->Litera() + 1, mZajmowanePole->Numer() + wiersz);
		if (pole && !pole->CzyPuste() && pole->Figura()->Kolor() != mKolor)
			OznaczPole(plansza, pole);

		if (plansza->CzyEnPassantMozliwy())
		{
			pole = plansza->Pole(mZajmowanePole->Litera() + 1, mZajmowanePole->Numer());
			if (pole && pole == plansza->PoleEnPassant())
			{
				pole = plansza->Pole(mZajmowanePole->Litera() + 1, mZajmowanePole->Numer() + wiersz);
				if (plansza->CzyKrolAtakowany())
				{
					if (plansza->CzyBlokujeAtak(pole, mKolor))
					{
						pole->UstawZaznaczenie(true, false);
						plansza->ZaznaczPole(pole);
					}
				}
				else
				{
					pole->UstawZaznaczenie(true, false);
					plansza->ZaznaczPole(pole);
				}
			}
			pole = plansza->Pole(mZajmowanePole->Litera() - 1, mZajmowanePole->Numer());
			if (pole && pole == plansza->PoleEnPassant())
			{
				pole = plansza->Pole(mZajmowanePole->Litera() - 1, mZajmowanePole->Numer() + wiersz);
				if (plansza->CzyKrolAtakowany())
				{
					if (plansza->CzyBlokujeAtak(pole, mKolor))
					{
						pole->UstawZaznaczenie(true, false);
						plansza->ZaznaczPole(pole);
					}
				}
				else
				{
					pole->UstawZaznaczenie(true, false);
					plansza->ZaznaczPole(pole);
				}
			}
		}

		return true;
	}

	void CPionek::OznaczPole(CPlansza *plansza, CPole *pole)
	{
		if (plansza->CzyKrolAtakowany())
		{
			if (plansza->CzyBlokujeAtak(pole, mKolor))
			{
				if (!pole->CzyPuste())
					pole->UstawZaznaczenie(true, false);
				else pole->UstawZaznaczenie(false, true);
				plansza->ZaznaczPole(pole);
			}
		}
		else
		{
			if (!pole->CzyPuste())
				pole->UstawZaznaczenie(true, false);
			else pole->UstawZaznaczenie(false, true);
			plansza->ZaznaczPole(pole);
		}
	}

	bool CPionek::CzyAtakuje(CPlansza* plansza, CPole* pole)
	{
		int wiersz = (mKolor == KF_BIALY) ? 1 : -1;
		if (SprawdzPole(pole, SPRAWDZ_NA_LEWO, wiersz)) return true;
		if (SprawdzPole(pole, SPRAWDZ_NA_PRAWO, wiersz)) return true;
		return false;
	}

	bool CPionek::CzyPromocja() const
	{
		return mPromocja;
	}
}