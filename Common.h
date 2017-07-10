#ifndef __COMMON_H__
#define __COMMON_H__


namespace Szachy
{
	const int ROZMIAR_GRAFIKI_PLANSZY = 572;
	const int OFFSET_POL_NA_GRAFICE_PLANSZY = 30;
	const int ROZMIAR_PLANSZY = 8;
	const int ROZMIAR_POLA_PLANSZY = 64;

	const int KOLOR_MASKI = 0 << 24 | 144 << 16 | 0 << 8 | 255;

	const int ILOSC_FIGUR = 32;
	const int PIERWSZA_BIALA_FIGURA_W_TABELI = 0;
	const int PIERWSZA_CZARNA_FIGURA_W_TABELI = 16;

	const int ROZMIAR_GRAFIKI_FIGURY = 64;
	const int POZYCJA_ZNACZNIKA_SELEKCJI_W_BITMAPIE = 384;


#define ZAZNACZ_W_GORE 1
#define ZAZNACZ_W_DOL -1
#define ZAZNACZ_NA_LEWO -1
#define ZAZNACZ_NA_PRAWO 1
#define ZAZNACZ_W_TYM_SAMYM 0

#define SPRAWDZ_W_GORE 1
#define SPRAWDZ_W_DOL -1
#define SPRAWDZ_NA_LEWO -1
#define SPRAWDZ_NA_PRAWO 1
#define SPRAWDZ_W_TYM_SAMYM 0
}


#endif