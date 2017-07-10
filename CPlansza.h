#ifndef __C_PLANSZA_H__
#define __C_PLANSZA_H__

#include <Windows.h>
#include "EKodBledu.h"
#include "CPole.h"
#include "ETypy.h"
#include "Common.h"
#include <vector>

namespace Szachy
{
	class CPlansza
	{
	public:
		CPlansza();
		~CPlansza();

		void Rysuj(HDC dc);
		EKodBledu Stworz(const int& x, const int& y, HWND uchwytOkna, CFigura **figury);
		void Zniszcz();

		void Resetuj();

		void ZaznaczPole(CPole* pole);
		void CzyscZaznaczenie();

		void UstawPoleBialegoKrola(CPole* pole);
		void UstawPoleCzarnegoKrola(CPole* pole);
		CPole* PoleBialegoKrola() const;
		CPole* PoleCzarnegoKrola() const;
		void UstawMozliwoscEnPassant(CPole* pole);
		CPole* PoleEnPassant() const;
		bool CzyEnPassantMozliwy() const;
		
		CPole* Pole(const char& litera, const int& numer);
		CPole* NacisnietePole(const int& myszX, const int& myszY, const bool& marked);
			
		bool CzyPoleAtakowane(CPole* doSprawdzenia, const EKolorFigury& kolor, const bool& ignorujKrola = false);
		void UstawAtakujacego(CPole* pole);
		CPole *OstatniAtakujacy() const;
		void SprawdzCzyKrolJestAtakowany(const EKolorFigury& aktywnyGracz);
		bool CzyKrolAtakowany() const;		
		bool CzySzachMat();
		bool SprawdzCzySzachMat(const EKolorFigury& aktywnyGracz);
		bool CzyBlokujeAtak(CPole* pole, const EKolorFigury& kolor);

	private:
		CFigura **mFigury;
		CPole mPola[ROZMIAR_PLANSZY*ROZMIAR_PLANSZY];
		std::vector<CPole*> mZaznaczonePola;
		CPole *mPoleEnPassant;
		CPole *mPoleBialegoKrola;
		CPole *mPoleCzarnegoKrola;
		CPole *mPoleAtakujacego;
		bool mCzyKrolAtakowany;
		bool mBrakMozliwosciRuchu;
		int mPozycjaX;
		int mPozycjaY;
		HBITMAP mBitmapaTla;
		HDC mKontekstTla;
		HBITMAP mDomyslnaBitmapaKontekstuTla;
		HBITMAP mBitmapaZaznaczenia;
		HDC mKontekstZaznaczenia;
		HBITMAP mDomyslnaBitmapaKontekstuZazaczenia;
	};
}


#endif