#ifndef __C_GONIEC_H__
#define __C_GONIEC_H__

#include "CFigura.h"

namespace Szachy
{
	class CGoniec : public CFigura
	{
	public:
		CGoniec(const EKolorFigury& kolor);
		virtual ~CGoniec();

		virtual bool UstawCzyWybrany(CPlansza* plansza, const bool& b);
		virtual bool CzyAtakuje(CPlansza* plansza, CPole* pole);
	};
}

#endif