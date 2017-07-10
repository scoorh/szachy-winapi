#include "CAplikacja.h"


/*

PROJECT SOURCE :: http://en.wikipedia.org/wiki/Chess

*/


int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
//int main()
{
	Szachy::SUstawieniaAplikacji ustawienia;
	ustawienia.wysokoscOkna = 572;
	ustawienia.szerokoscOkna = 1000;
	ustawienia.instancja = instance;
//	ustawienia.instancja = GetModuleHandle(0);
	ustawienia.tytul = "Szachy";


	Szachy::EKodBledu kodBledu;
	if ((kodBledu = Szachy::CAplikacja::Stworz(&ustawienia)) == Szachy::KB_BRAK)
	{
		Szachy::CAplikacja::Wykonuj();
		Szachy::CAplikacja::Zwolnij();
	}
	else
	{
		MessageBox(0, (LPCSTR)Szachy::KodBleduNaString(kodBledu).c_str(), "ERROR", MB_OK);
	}

	return 0;
}