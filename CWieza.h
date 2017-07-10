#ifndef __C_WIEZA_H__
#define __C_WIEZA_H__

#include "CFigura.h"

namespace Szachy
{
	class CWieza : public CFigura
	{
	public:
		CWieza(const EKolorFigury& kolor);
		virtual ~CWieza();

		virtual bool UstawCzyWybrany(CPlansza* plansza, const bool& b);
		virtual bool CzyAtakuje(CPlansza* plansza, CPole* pole);
	};
}

#endif