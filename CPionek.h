#ifndef __C_PIONEK_H__
#define __C_PIONEK_H__

#include "CFigura.h"

namespace Szachy
{
	class CPionek : public CFigura
	{
	public:
		CPionek(const EKolorFigury& kolor);
		virtual ~CPionek();

		virtual bool UstawCzyWybrany(CPlansza* plansza, const bool& b);
		virtual void PrzesunNa(CPlansza* plansza, CPole *cel);
		virtual bool CzyAtakuje(CPlansza* plansza, CPole* pole);

		bool CzyPromocja() const;

	private:
		void OznaczPole(CPlansza* plansza, CPole *pole);
		
	private:
		bool mPromocja;
	};
}

#endif