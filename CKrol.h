#ifndef __C_KROL_H__
#define __C_KROL_H__

#include "CFigura.h"

namespace Szachy
{
	class CKrol : public CFigura
	{
	public:
		CKrol(const EKolorFigury& kolor);
		virtual ~CKrol();

		virtual bool UstawCzyWybrany(CPlansza* plansza, const bool& b);
		virtual void PrzesunNa(CPlansza* plansza, CPole *cel);
		virtual bool CzyAtakuje(CPlansza* plansza, CPole* pole);

	private:
		void ZaznaczPole(CPlansza* plansza, const int& kolumna, const int& wiersz);
	};
}

#endif