#ifndef __E_KOD_BLEDU_H__
#define __E_KOD_BLEDU_H__

#include <string>

namespace Szachy
{
	enum EKodBledu
	{
		KB_BRAK = 0,
		KB_PUSTY_WSKAZNIK,
		KB_BRAK_PAMIECI,
		KB_REJESTRACJA_KLASY_OKNA,
		KB_TWORZENIE_OKNA,
		KB_ODCZYT_PLIKU,
		KB_TWORZENIE_CONTEKSTU
	};

	inline std::string KodBleduNaString(const EKodBledu& e)
	{
		std::string ret = "";
		switch (e)
		{
		case KB_BRAK_PAMIECI:
			ret = "Brak pamieci";
			break;
		case KB_PUSTY_WSKAZNIK:
			ret = "Zly wskaznik";
			break;
		case KB_REJESTRACJA_KLASY_OKNA:
			ret = "Rejestracja klasy okna";
			break;
		case KB_TWORZENIE_OKNA:
			ret = "Tworzenie okna";
			break;
		case KB_ODCZYT_PLIKU:
			ret = "Odczyt pliku";
			break;
		case KB_TWORZENIE_CONTEKSTU:
			ret = "Tworzenie kontekstu bitmapy";
			break;
		}
		return ret;
	}
}

#endif