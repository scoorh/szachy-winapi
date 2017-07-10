#include "CSkoczek.h"

namespace Szachy
{
	CSkoczek::CSkoczek(const EKolorFigury& kolor) :
		CFigura(TF_SKOCZEK, kolor)
	{
	}

	CSkoczek::~CSkoczek()
	{
	}

	bool CSkoczek::UstawCzyWybrany(CPlansza* plansza, const bool& b)
	{
		if (CFigura::UstawCzyWybrany(plansza, b) || plansza == 0)
			return false;

		ZaznaczPole(plansza, ZAZNACZ_NA_LEWO - 1, ZAZNACZ_W_GORE);
		ZaznaczPole(plansza, ZAZNACZ_NA_LEWO, ZAZNACZ_W_GORE + 1);
		ZaznaczPole(plansza, ZAZNACZ_NA_PRAWO, ZAZNACZ_W_GORE + 1);
		ZaznaczPole(plansza, ZAZNACZ_NA_PRAWO + 1, ZAZNACZ_W_GORE);
		ZaznaczPole(plansza, ZAZNACZ_NA_PRAWO + 1, ZAZNACZ_W_DOL);
		ZaznaczPole(plansza, ZAZNACZ_NA_PRAWO, ZAZNACZ_W_DOL - 1);
		ZaznaczPole(plansza, ZAZNACZ_NA_LEWO, ZAZNACZ_W_DOL - 1);
		ZaznaczPole(plansza, ZAZNACZ_NA_LEWO - 1, ZAZNACZ_W_DOL);

		return true;
	}

	bool CSkoczek::CzyAtakuje(CPlansza* plansza, CPole* pole)
	{
		if (SprawdzPole(pole, SPRAWDZ_NA_LEWO-1, SPRAWDZ_W_GORE)) return true;
		if (SprawdzPole(pole, SPRAWDZ_NA_LEWO, SPRAWDZ_W_GORE + 1)) return true;
		if (SprawdzPole(pole, SPRAWDZ_NA_PRAWO, SPRAWDZ_W_GORE+1)) return true;
		if (SprawdzPole(pole, SPRAWDZ_NA_PRAWO+1, SPRAWDZ_W_GORE)) return true;
		if (SprawdzPole(pole, SPRAWDZ_NA_PRAWO+1, SPRAWDZ_W_DOL)) return true;
		if (SprawdzPole(pole, SPRAWDZ_NA_PRAWO, SPRAWDZ_W_DOL - 1)) return true;
		if (SprawdzPole(pole, SPRAWDZ_NA_LEWO, SPRAWDZ_W_DOL-1)) return true;
		if (SprawdzPole(pole, SPRAWDZ_NA_LEWO - 1, SPRAWDZ_W_DOL)) return true;
		return false;
	}
}