#ifndef __C_POLE_H__
#define __C_POLE_H__


namespace Szachy
{
	class CFigura;

	class CPole
	{
	public:
		CPole();
		~CPole();

		void Stworz(const char& litera, const int& numer, const int& tableIndex);
		void UstawZajmujacego(CFigura* piece);
		void UstawZaznaczenie(const bool& znacznikAtaku, const bool& znacznikRuchu);

		char Litera() const;
		int Numer() const;
		int IndeksTabeli() const;
		bool CzyPuste() const;
		CFigura* Figura() const;
		bool OznaczonyDoRuchu() const;
		bool OznaczonyDoAtaku() const;
		bool CzyOznaczony() const;

		int GornyX() const;
		int GornyY() const;

		bool CzyNacisniety(const int& myszX, const int& myszY) const;

	private:
		char mLitera;
		int mNumer;
		int mIndeksTabeli;
		CFigura *mZajetyPrzez;
		bool mZnacznikRuchu;
		bool mZnacznikAtaku;
	};

}

#endif