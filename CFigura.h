#ifndef __C_FIGURA_H__
#define __C_FIGURA_H__

#include "Common.h"
#include <Windows.h>
#include "ETypy.h"
#include "EKodBledu.h"
#include "SListaWiadomosci.h"



namespace Szachy
{
	class CPole;
	class CPlansza;

	class CFigura
	{
	public:
		CFigura(const ETypFigury& typ, const EKolorFigury& kolor);
		virtual ~CFigura();

		static EKodBledu Stworz(HWND uchwytOkna);
		static void Zniszcz();
		void UstawZajmowanePole(CPole* pole);

		void UstawTyp(const ETypFigury& typ);
		void UstawKolor(const EKolorFigury& kolor);
		void UstawCzyZyje(const bool& b);	
		void UstawCzyWykonalRuch(const bool& b);
		virtual bool UstawCzyWybrany(CPlansza* plansza, const bool& b);
		virtual void PrzesunNa(CPlansza* plansza, CPole *cel);

		virtual bool CzyAtakuje(CPlansza* plansza, CPole* pole);

		ETypFigury Typ() const;
		EKolorFigury Kolor() const;
		CPole* ZajmowanePole() const;
		bool CzyZyje() const;
		bool CzyWykonalRuch() const;

		void Rysuj(HDC dc);

	protected:
		void ZaznaczLinie(CPlansza* plansza, const int& kolumna, const int& wiersz);
		void ZaznaczPole(CPlansza* plansza, const int& kolumna, const int& wiersz);

		bool SprawdzLinie(CPlansza* plansza, CPole* sprawdzOd, const int& kolumna, const int& wiersz);
		bool SprawdzPole(CPole* sprawdzOd, const int& kolumna, const int& wiersz);

		bool SprawdzCzyBroniKrola(CPlansza* plansza);

		void ZnajdzKolumneIWiersz(CPole* other, int& kolumna, int& wiersz);

	protected:
		ETypFigury mTyp;
		EKolorFigury mKolor;
		CPole* mZajmowanePole;
		bool mCzyZyje;
		bool mCzyWybrany;
		bool mCzyWykonalRuch;

		static HBITMAP mBitmapaFigur;
		static HDC mKontekstBitmapyFigur;
		static HBITMAP mDomyslnaBitmapaKontekstuFigur;
	};
}

#endif