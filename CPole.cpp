#include "CPole.h"
#include "CPlansza.h"

namespace Szachy
{
	CPole::CPole() :
		mZajetyPrzez(0)
	{}

	CPole::~CPole()
	{}

	void CPole::Stworz(const char& litera, const int& numer, const int& tableIndex)
	{
		mLitera = litera;
		mNumer = numer;
		mIndeksTabeli = tableIndex;
		mZnacznikRuchu = false;
		mZnacznikAtaku = false;
		mZajetyPrzez = 0;
	}

	void CPole::UstawZaznaczenie(const bool& znacznikAtaku, const bool& OznaczonyDoRuchu)
	{
		mZnacznikRuchu = OznaczonyDoRuchu;
		mZnacznikAtaku = znacznikAtaku;
	}

	bool CPole::CzyNacisniety(const int& myszX, const int& myszY) const
	{
		if (myszX > GornyX() && (myszX < GornyX() + ROZMIAR_POLA_PLANSZY) &&
			myszY > GornyY() && (myszY < GornyY() + ROZMIAR_POLA_PLANSZY))
			return true;
		return false;
	}

	bool CPole::OznaczonyDoRuchu() const
	{
		return mZnacznikRuchu;
	}

	bool CPole::OznaczonyDoAtaku() const
	{
		return mZnacznikAtaku;
	}

	char CPole::Litera() const
	{
		return mLitera;
	}

	int CPole::Numer() const
	{
		return mNumer;
	}

	int CPole::IndeksTabeli() const
	{
		return mIndeksTabeli;
	}

	bool CPole::CzyPuste() const
	{
		return mZajetyPrzez ? false : true;
	}

	CFigura* CPole::Figura() const
	{
		return mZajetyPrzez;
	}

	void CPole::UstawZajmujacego(CFigura* piece)
	{
		mZajetyPrzez = piece;
	}

	int CPole::GornyX() const
	{
		return OFFSET_POL_NA_GRAFICE_PLANSZY + (((int)(mLitera - 'A')) * ROZMIAR_POLA_PLANSZY) - 1;
	}

	int CPole::GornyY() const
	{
		return OFFSET_POL_NA_GRAFICE_PLANSZY + ((ROZMIAR_PLANSZY - mNumer) * ROZMIAR_POLA_PLANSZY) - 1;
	}

	bool CPole::CzyOznaczony() const
	{
		return mZnacznikRuchu || mZnacznikAtaku;
	}
}