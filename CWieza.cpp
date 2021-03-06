#include "CWieza.h"


namespace Szachy
{
	CWieza::CWieza(const EKolorFigury& kolor) :
		CFigura(TF_WIEZA, kolor)
	{
	}

	CWieza::~CWieza()
	{
	}

	bool CWieza::UstawCzyWybrany(CPlansza* plansza, const bool& b)
	{
		if (CFigura::UstawCzyWybrany(plansza, b) || plansza == 0)
			return false;

		ZaznaczLinie(plansza, ZAZNACZ_NA_PRAWO, ZAZNACZ_W_TYM_SAMYM);
		ZaznaczLinie(plansza, ZAZNACZ_NA_LEWO, ZAZNACZ_W_TYM_SAMYM);
		ZaznaczLinie(plansza, ZAZNACZ_W_TYM_SAMYM, ZAZNACZ_W_GORE);
		ZaznaczLinie(plansza, ZAZNACZ_W_TYM_SAMYM, ZAZNACZ_W_DOL);

		return true;
	}

	bool CWieza::CzyAtakuje(CPlansza* plansza, CPole* pole)
	{
		if (SprawdzLinie(plansza, pole, SPRAWDZ_NA_PRAWO, SPRAWDZ_W_TYM_SAMYM)) return true;
		if (SprawdzLinie(plansza, pole, SPRAWDZ_NA_LEWO, SPRAWDZ_W_TYM_SAMYM)) return true;
		if (SprawdzLinie(plansza, pole, SPRAWDZ_W_TYM_SAMYM, SPRAWDZ_W_GORE)) return true;
		if (SprawdzLinie(plansza, pole, SPRAWDZ_W_TYM_SAMYM, SPRAWDZ_W_DOL)) return true;
		return false;
	}
}