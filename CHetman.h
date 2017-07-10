#ifndef __C_HETMAN_H__
#define __C_HETMAN_H__

#include "CFigura.h"

namespace Szachy
{
	class CHetman : public CFigura
	{
	public:
		CHetman(const EKolorFigury& kolor);
		virtual ~CHetman();

		virtual bool UstawCzyWybrany(CPlansza* plansza, const bool& b);
		virtual bool CzyAtakuje(CPlansza* plansza, CPole* pole);
	};
}

#endif