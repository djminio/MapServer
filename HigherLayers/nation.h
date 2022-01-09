#ifndef __NATION_H__
#define __NATION_H__

#define NM_ADD			1		// 국고에 더해준다.
#define NM_SUBTRACT		2		// 국고에서 뺀다.

struct tagNationInfo
{
//	char	name[30];			// 국가 이름
	int		main_map_port;		// 데이타를 관리할 맵서버
	char	king_name[20];		// 왕이름
	double	nation_money;		// 국고
	int		king_support;		// 왕 지지도
	int		visa_va;			// 바이서스에 팔 비자 번호
	int		visa_zy;
	int		visa_ill;

	int		visa_pri_va;		// 바이서스에 팔 비자 가격
	int		visa_pri_zy;
	int		visa_pri_ill;

	int		visa_mer_va;		// 상인용 비자 번호( 바이서스 용 )
	int		visa_mer_zy;
	int		visa_mer_ill;

	int		visa_mer_pri_va;	// 상인에게 팔 비자 가격( 바이서스 용 )
	int		visa_mer_pri_zy;
	int		visa_mer_pri_ill;

	int		visit_limit_va;		// 바이서스인 방문자수 최대
	int		visit_limit_zy;
	int		visit_limit_ill;

	int		friend_va;
	int		friend_zy;
	int		friend_ill;

	int		cond_va;			// 1: 전쟁,		5: 기본,	9: 동맹
	int		cond_zy;
	int		cond_ill;

	int		dec_anti_va;		// 개인의 적대감을 1 줄이기 위한 돈
	int		dec_anti_zy;
	int		dec_anti_ill;

	int		dec_max_va;			// 개인이 줄일수 있는 하루 감소 최대량
	int		dec_max_zy;
	int		dec_max_ill;

	int		nation_item[20];		// 국가가 가질수 있는 20가지의 아이템의 양 -> 아이템이 뭔지는 따로
	
	int		prize_money_va;			// 침입자 처단 포상 돈
	int		prize_money_zy;
	int		prize_money_ill;

	int		prize_item_va;			// 침입자 처단시 포상 아이템
	int		prize_item_zy;
	int		prize_item_ill;
};

extern tagNationInfo NationInfo[MAX_NATION];
extern int	 g_MapPort;		// 010522 YGI
extern void CheckMainMap();
extern void SendNationMoney( int type, int nation, double moneny );
extern void SaveNationInfo( int nation );		// 디비 데몬에 저장한다.
extern int LoadNationInfoOfMainMap( int nation );

#endif