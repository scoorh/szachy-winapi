#include "CKrol.h"
#include "CPole.h"
#include "CPlansza.h"


namespace Szachy
{
	CKrol::CKrol(const EKolorFigury& kolor) :
		CFigura(TF_KROL, kolor)
	{
	}

	CKrol::~CKrol()
	{
	}

	void CKrol::PrzesunNa(CPlansza* plansza, CPole *cel)
	{
		g_InfoRuchu.typFigury = TF_KROL;
		g_InfoRuchu.zrodlo = mZajmowanePole;
		g_InfoRuchu.cel = cel;
		g_InfoRuchu.typRuchu = TR_NORMALNY;
		g_InfoRuchu.kolorFigury = mKolor;

		if (cel->Figura() && cel->Figura()->Typ() == TF_WIEZA)
		{
			CPole *rookpole;
			CPole *poleKrola;
			if (cel->Litera() == 'A')
			{
				rookpole = plansza->Pole('D', cel->Numer());
				poleKrola = plansza->Pole('C', cel->Numer());
				g_InfoRuchu.typRuchu = TR_ROSZADA_DLUGA;
			}
			else
			{
				rookpole = plansza->Pole('F', cel->Numer());
				poleKrola = plansza->Pole('G', cel->Numer());
				g_InfoRuchu.typRuchu = TR_ROSZADA_KROTKA;
			}

			cel->Figura()->UstawZajmowanePole(rookpole);
			rookpole->UstawZajmujacego(cel->Figura());
			cel->Figura()->UstawCzyWykonalRuch(true);
			cel->UstawZajmujacego(0);
			mZajmowanePole->UstawZajmujacego(0);
			mZajmowanePole = poleKrola;
			poleKrola->UstawZajmujacego(this);
			mCzyWykonalRuch = true;
		}
		else
		{
			plansza->UstawAtakujacego(0);
			CFigura::PrzesunNa(plansza, cel);
		}

		if (mKolor == KF_BIALY)
			plansza->UstawPoleBialegoKrola(cel);
		else
			plansza->UstawPoleCzarnegoKrola(cel);
	}

	void CKrol::ZaznaczPole(CPlansza* plansza, const int& kolumna, const int& wiersz)
	{
		CPole *pole;
		pole = plansza->Pole(mZajmowanePole->Litera() + kolumna, mZajmowanePole->Numer() + wiersz);

		if (pole && !plansza->CzyPoleAtakowane(pole, mKolor))
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

	bool CKrol::UstawCzyWybrany(CPlansza* plansza, const bool& b)
	{
		CFigura::UstawCzyWybrany(plansza, b);
		if (plansza == 0)
			return false;

		ZaznaczPole(plansza, ZAZNACZ_NA_LEWO, ZAZNACZ_W_TYM_SAMYM);
		ZaznaczPole(plansza, ZAZNACZ_NA_LEWO, ZAZNACZ_W_GORE);
		ZaznaczPole(plansza, ZAZNACZ_W_TYM_SAMYM, ZAZNACZ_W_GORE);
		ZaznaczPole(plansza, ZAZNACZ_NA_PRAWO, ZAZNACZ_W_GORE);
		ZaznaczPole(plansza, ZAZNACZ_NA_PRAWO, ZAZNACZ_W_TYM_SAMYM);
		ZaznaczPole(plansza, ZAZNACZ_NA_PRAWO, ZAZNACZ_W_DOL);
		ZaznaczPole(plansza, ZAZNACZ_W_TYM_SAMYM, ZAZNACZ_W_DOL);
		ZaznaczPole(plansza, ZAZNACZ_NA_LEWO, ZAZNACZ_W_DOL);

		char wiersz = (mKolor == KF_BIALY) ? 1 : 8;
		if (mCzyWykonalRuch || (mZajmowanePole->Litera() != 'E' && mZajmowanePole->Numer() != wiersz))
			return false;

		CPole* pole;
		pole = plansza->Pole('A', wiersz);
		if (!pole->CzyPuste() && pole->Figura()->Typ() == TF_WIEZA && !pole->Figura()->CzyWykonalRuch())
		{
			bool canMove = true;
			for (int i = 1; i <= 3 && canMove; ++i)
			{
				CPole *pole2 = plansza->Pole(pole->Litera() + i, pole->Numer());
				if (pole && !pole2->CzyPuste())
					canMove = false;
			}

			if (canMove)
			{
				pole->UstawZaznaczenie(false, true);
				plansza->ZaznaczPole(pole);
			}
		}

		pole = plansza->Pole('H', wiersz);
		if (!pole->CzyPuste() && pole->Figura()->Typ() == TF_WIEZA && !pole->Figura()->CzyWykonalRuch())
		{
			bool canMove = true;
			for (int i = 1; i <= 2 && canMove; ++i)
			{
				CPole *pole2 = plansza->Pole(pole->Litera() - i, pole->Numer());
				if (pole && !pole2->CzyPuste())
					canMove = false;
			}

			if (canMove)
			{
				pole->UstawZaznaczenie(false, true);
				plansza->ZaznaczPole(pole);
			}
		}

		return true;
	}

	bool CKrol::CzyAtakuje(CPlansza* plansza, CPole* pole)
	{
		if (SprawdzPole(pole, SPRAWDZ_NA_LEWO, SPRAWDZ_W_TYM_SAMYM)) return true;
		if (SprawdzPole(pole, SPRAWDZ_NA_LEWO, SPRAWDZ_W_GORE)) return true;
		if (SprawdzPole(pole, SPRAWDZ_W_TYM_SAMYM, SPRAWDZ_W_GORE)) return true;
		if (SprawdzPole(pole, SPRAWDZ_NA_PRAWO, SPRAWDZ_W_GORE)) return true;
		if (SprawdzPole(pole, SPRAWDZ_NA_PRAWO, SPRAWDZ_W_TYM_SAMYM)) return true;
		if (SprawdzPole(pole, SPRAWDZ_NA_PRAWO, SPRAWDZ_W_DOL)) return true;
		if (SprawdzPole(pole, SPRAWDZ_W_TYM_SAMYM, SPRAWDZ_W_DOL)) return true;
		if (SprawdzPole(pole, SPRAWDZ_NA_LEWO, SPRAWDZ_W_DOL)) return true;
		return false;
	}
}