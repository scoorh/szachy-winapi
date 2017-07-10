#ifndef __E_TYPY_H__
#define __E_TYPY_H__


namespace Szachy
{
	enum ETypRuchu
	{
		TR_NORMALNY,
		TR_BICIE,
		TR_PIONEK,
		TR_ENPASSANT,
		TR_ROSZADA_DLUGA,
		TR_ROSZADA_KROTKA,
		TR_BRAK_SZACHU,
		TR_SZACH,
		TR_SZACHMAT
	};

	enum ETypFigury
	{
		TF_PIONEK,
		TF_WIEZA,
		TF_GONIEC,
		TF_SKOCZEK,
		TF_KROL,
		TF_HETMAN	
	};

	enum EKolorFigury
	{
		KF_BIALY,
		KF_CZARNY
	};

	inline char TypFiguryNaChar(const ETypFigury& e)
	{
		switch (e)
		{
		case TF_WIEZA:
			return 'R';
		case TF_GONIEC:
			return 'B';
		case TF_SKOCZEK:
			return 'N';
		case TF_KROL:
			return 'K';
		case TF_HETMAN:
			return 'Q';

		default:
			return ' ';
		}
	}

	inline char KolorFiguryNaChar(const EKolorFigury& e)
	{
		switch (e)
		{
		case KF_BIALY:
			return 'W';
		case KF_CZARNY:
			return 'B';

		default:
			return ' ';
		}
	}
}


#endif