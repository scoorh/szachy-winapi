#include "CGoniec.h"


namespace Szachy
{
	CGoniec::CGoniec(const EKolorFigury& kolor) :
		CFigura(TF_GONIEC, kolor)
	{
	}

	CGoniec::~CGoniec()
	{
	}

	bool CGoniec::UstawCzyWybrany(CPlansza* plansza, const bool& b)
	{
		if (CFigura::UstawCzyWybrany(plansza, b) || plansza == 0)
			return false;

		ZaznaczLinie(plansza, ZAZNACZ_NA_LEWO, ZAZNACZ_W_GORE);
		ZaznaczLinie(plansza, ZAZNACZ_NA_PRAWO, ZAZNACZ_W_GORE);
		ZaznaczLinie(plansza, ZAZNACZ_NA_LEWO, ZAZNACZ_W_DOL);
		ZaznaczLinie(plansza, ZAZNACZ_NA_PRAWO, ZAZNACZ_W_DOL);

		return true;
	}

	bool CGoniec::CzyAtakuje(CPlansza* plansza, CPole* pole)
	{
		if (SprawdzLinie(plansza, pole, SPRAWDZ_NA_LEWO, SPRAWDZ_W_GORE)) return true;
		if (SprawdzLinie(plansza, pole, SPRAWDZ_NA_PRAWO, SPRAWDZ_W_GORE)) return true;
		if (SprawdzLinie(plansza, pole, SPRAWDZ_NA_LEWO, SPRAWDZ_W_DOL)) return true;
		if (SprawdzLinie(plansza, pole, SPRAWDZ_NA_PRAWO, SPRAWDZ_W_DOL)) return true;
		return false;
	}
}