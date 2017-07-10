#ifndef __S_LISTA_WIADOMOSCI_H__
#define __S_LISTA_WIADOMOSCI_H__

#include <Windows.h>
#include "ETypy.h"
#include "CPole.h"

namespace Szachy
{
	extern HWND g_ListaWidomosciBialyGracz;
	extern HWND g_ListaWidomosciCzarnyGracz;

	struct SInfoRuchu
	{
		EKolorFigury kolorFigury;
		ETypFigury typFigury;
		ETypRuchu typRuchu;
		ETypRuchu typSzachu;
		CPole *zrodlo;
		CPole *cel;
	};

	extern SInfoRuchu g_InfoRuchu;

	void ZapiszInfoRuchu();
	void ResetujInfoRuchu();
}

#endif