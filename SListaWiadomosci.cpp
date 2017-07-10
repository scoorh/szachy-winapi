#include "SListaWiadomosci.h"
#include <string>
#include <sstream>

namespace Szachy
{
	HWND g_ListaWidomosciBialyGracz;
	HWND g_ListaWidomosciCzarnyGracz;
	SInfoRuchu g_InfoRuchu;


	void ResetujInfoRuchu()
	{
		SendMessage(g_ListaWidomosciBialyGracz, LB_RESETCONTENT, 0, 0);
		SendMessage(g_ListaWidomosciCzarnyGracz, LB_RESETCONTENT, 0, 0);
		UpdateWindow(g_ListaWidomosciBialyGracz);
		UpdateWindow(g_ListaWidomosciCzarnyGracz);
	}

	void ZapiszInfoRuchu()
	{
		if (SendMessage(g_ListaWidomosciCzarnyGracz, LB_GETCOUNT, 0, 0) > 21)
			ResetujInfoRuchu();

		std::string napis;
		napis.reserve(20);
		std::string numerCelu;
		std::string numerZrodla;

		{
			std::stringstream ss;
			ss << g_InfoRuchu.cel->Numer();
			numerCelu = ss.str();
		}
		{
			std::stringstream ss;
			ss << g_InfoRuchu.zrodlo->Numer();
			numerZrodla = ss.str();
		}

		napis += KolorFiguryNaChar(g_InfoRuchu.kolorFigury);
		napis += "   ";
		switch (g_InfoRuchu.typRuchu)
		{
		case TR_NORMALNY:
			napis += TypFiguryNaChar(g_InfoRuchu.typFigury);
			napis += tolower(g_InfoRuchu.cel->Litera());
			napis += numerCelu;
			break;

		case TR_BICIE:
			if (g_InfoRuchu.typFigury == TF_PIONEK)
			{
				napis += tolower(g_InfoRuchu.zrodlo->Litera());
				napis += numerZrodla;
				napis += "x";
				napis += tolower(g_InfoRuchu.cel->Litera());
				napis += numerCelu;
			}
			else
			{
				napis += TypFiguryNaChar(g_InfoRuchu.typFigury);
				napis += 'x';
				napis += tolower(g_InfoRuchu.cel->Litera());
				napis += numerCelu;
			}
			break;

		case TR_ROSZADA_KROTKA:
			napis += "0-0";
			break;

		case TR_ROSZADA_DLUGA:
			napis += "0-0-0";
			break;

		case TR_PIONEK:
			napis += tolower(g_InfoRuchu.zrodlo->Litera());
			napis += numerZrodla;
			napis += " ";
			napis += tolower(g_InfoRuchu.cel->Litera());
			napis += numerCelu;
			break;

		case TR_ENPASSANT:
			napis += tolower(g_InfoRuchu.zrodlo->Litera());
			napis += numerZrodla;
			napis += ".";
			napis += tolower(g_InfoRuchu.cel->Litera());
			napis += numerCelu;
			break;
		}

		switch (g_InfoRuchu.typSzachu)
		{
		case TR_SZACH:
			napis += "+";
			break;

		case TR_SZACHMAT:
			napis += "#";
			break;
		}
		
		LPCSTR pstr = napis.c_str();

		if (g_InfoRuchu.kolorFigury == KF_BIALY)
		{
			SendMessage(g_ListaWidomosciBialyGracz, LB_ADDSTRING, 0, (LPARAM)&pstr[0]);
			UpdateWindow(g_ListaWidomosciBialyGracz);
		}
		else
		{
			SendMessage(g_ListaWidomosciCzarnyGracz, LB_ADDSTRING, 0, (LPARAM)&pstr[0]);
			UpdateWindow(g_ListaWidomosciCzarnyGracz);
		}
	}
}
