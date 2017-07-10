#ifndef __S_USTAWIENIA_APLIKACJI_H__
#define __S_USTAWIENIA_APLIKACJI_H__

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

namespace Szachy
{
	struct SUstawieniaAplikacji
	{
		unsigned int szerokoscOkna;
		unsigned int wysokoscOkna;
		HINSTANCE instancja;
		std::string tytul;
	};
}

#endif