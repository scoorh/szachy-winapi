#ifndef __C_SKOCZEK_H__
#define __C_SKOCZEK_H__

#include "CFigura.h"

namespace Szachy
{
	class CSkoczek : public CFigura
	{
	public:
		CSkoczek(const EKolorFigury& kolor);
		virtual ~CSkoczek();

		virtual bool UstawCzyWybrany(CPlansza* plansza, const bool& b);
		virtual bool CzyAtakuje(CPlansza* plansza, CPole* pole);
		
	private:

	};
}

#endif