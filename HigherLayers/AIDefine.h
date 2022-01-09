#ifndef _AI_DEFINE_H_
#define _AI_DEFINE_H_

// NPC_AI_CONTROL TABLE FILED DEFINE

#define	FAT_TYPE				1					// AI타입번호 NPC_GENERATION_SP의 SEL_AGE_MAX갑
#define FATTACK_RANGE			2					// 물리공격 범위
#define	FSEARCH_RANGE			3					// 검색 범위
#define FWANDER_RATE			4					// 배회 율 (%)
#define	FSTART_AI_PATTERN		5					// 시작 AI패턴 번호
#define	FSTATUS01_DELAY			6					// 상태1 딜레이		PATTERN_WANDER
#define FSTATUS01_SUBDELAY		7					// 상태1 적용 랜덤 치 
#define	FSTATUS02_DELAY			8					// 상태2 딜레이		PATTERN_ACCESS
#define FSTATUS02_SUBDELAY		9					// 상태2의 적용 랜덤치
#define	FSTATUS03_DELAY			10					// 상태3 딜레이		PATTERN_ATTACK
#define FSTATUS03_SUBDELAY		11					// 상태3의 적용 랜덤치
#define	FSTATUS04_DELAY			12					// 상태4 딜레이		PATTERN_ATTACKED
#define FSTATUS04_SUBDELAY		13					// 상태4의 적용 랜덤치
#define	FSTATUS05_DELAY			14					// 상태5 딜레이		PATTERN_MOVE
#define FSTATUS05_SUBDELAY		15					// 상태5의 적용 랜덤치
#define	FSTATUS06_DELAY			16					// 나머지 예약
#define FSTATUS06_SUBDELAY		17
#define	FSTATUS07_DELAY			18
#define FSTATUS07_SUBDELAY		19
#define	FSTATUS08_DELAY			20
#define FSTATUS08_SUBDELAY		21
#define	FSTATUS09_DELAY			22
#define FSTATUS09_SUBDELAY		23
#define	FSTATUS10_DELAY			24
#define FSTATUS10_SUBDELAY		25
#define	FRECOVERY_DELAY			26					// 회복 유지시간
#define FRECOVERY_AMOUNT		27					// 회복량
#define FRECOVERY_PERCENT		28					// 회복 랜덤치
#define	FRUN_SPEED				29					// 달리기 속도 (%)
#define	FRUN_DELAY				30					// 달리기 유지시간
#define FRUN_PERCENT			31					// 달리기 랜덤치
#define	FSTATUSUP_DELAY			32					// 광폭상태 유지시간
#define FSTATUSUP_PERCENT		33					// 광폭 랜덤치
#define FSTATUSUP_LIFE_PERCENT	34					// HP의 %에의한 광폭 전이 수치 (30%)
#define	FSTATUSUP1				35					// 물리공격 증가 (%)
#define	FSTATUSUP2				36					// 방어력 증가 (%)
#define	FSTATUSUP3				37					// 속도 증가 (%)
#define	FSTATUSUP4				38					// 예약	
#define	FSTATUSUP5				39					// 예약
#define FSUMMON_DELAY			40
#define FSUMMON_PERCENT			41					// 소환몬 랜덤수치
#define	FSUMMON_MAX				42					// 소환 최대치
#define	FSUMMON_NPC1			43					// 소환 NPC인덱스1
#define	FSUMMON_NPC2			44					// 소환 NPC인덱스2
#define	FSUMMON_NPC3			45					// 소환 NPC인덱스3
#define	FSUMMON_NPC4			46					// 소환 NPC인덱스4
#define	FSUMMON_NPC5			47					// 소환 NPC인덱스5
#define	FSUMMON_TERM			48					// 소환 주기
#define	FATTACK_METHOD1			49					// 공격 
#define FATTACK_RATE1			50					// 공격 방법에 의한 최초 선택 비율1
#define	FATTACK_METHOD2			51					// 공격 방법2
#define FATTACK_RATE2			52					// 공격 방업에 의한 최초 선택 비율2
#define	FATTACK_METHOD3			53					// 공격 방법3	
#define FATTACK_RATE3			54					// 공격 방업에 의한 최초 선택 비율3
#define	FATTACK_CHANCE			55					// 공격 CHNAGE값
#define FSPECIAL_ATTACK			56					// 특수공격 마법 백 번호

// NPC_AI_CONTROL TABLE DEFINE END.........



// AI_BOSSSTATUS TABLE DEFINE

#define FBTYPE_NO				1					// 등장관련 번호
#define FBHP_GRADE1				2					// HP상태 1
#define FBHP_GRADE2				3					// HP상태 2
#define FBHP_GRADE3				4					// HP상태 3
#define FBHP_GRADE4				5					// HP상태 4
#define FBHP_GRADE5				6					// HP상태 5
#define FBHP_GRADE6				7					// HP상태 6
#define FBATTACK_METHOD1		8					// HP상태 1의 공격방법
#define FBATTACK_METHOD2		9					// HP상태 2의 공격방법
#define FBATTACK_METHOD3		10					// HP상태 3의 공격방법
#define FBATTACK_METHOD4		11					// HP상태 4의 공격방법
#define FBATTACK_METHOD5		12					// HP상태 5의 공격방법
#define FBATTACK_METHOD6		13					// HP상태 6의 공격방법
#define FBACTION0_METHOD1		14					// HP상태 1에서의 처리방법 1
#define FBACTION0_METHOD2		15					// HP상태 1에서의 처리방법 2
#define FBACTION0_METHOD3		16					// HP상태 1에서의 처리방법 3
#define FBACTION1_METHOD1		17					// HP상태 1에서의 처리방법 1
#define FBACTION1_METHOD2		18					// HP상태 1에서의 처리방법 2
#define FBACTION1_METHOD3		19					// HP상태 1에서의 처리방법 3
#define FBACTION2_METHOD1		20					// HP상태 2에서의 처리방법 1
#define FBACTION2_METHOD2		21					// HP상태 2에서의 처리방법 2
#define FBACTION2_METHOD3		22					// HP상태 2에서의 처리방법 3
#define FBACTION3_METHOD1		23					// HP상태 3에서의 처리방법 1
#define FBACTION3_METHOD2		24					// HP상태 3에서의 처리방법 2
#define FBACTION3_METHOD3		25					// HP상태 3에서의 처리방법 3
#define FBACTION4_METHOD1		26					// HP상태 4에서의 처리방법 1
#define FBACTION4_METHOD2		27					// HP상태 4에서의 처리방법 2	
#define FBACTION4_METHOD3		28					// HP상태 4에서의 처리방법 3
#define FBACTION5_METHOD1		29					// HP상태 5에서의 처리방법 1
#define FBACTION5_METHOD2		30					// HP상태 5에서의 처리방법 2
#define FBACTION5_METHOD3		31					// HP상태 5에서의 처리방법 3
#define FBACTION6_METHOD1		32					// HP상태 6에서의 처리방법 1
#define FBACTION6_METHOD2		33					// HP상태 6에서의 처리방법 2
#define FBACTION6_METHOD3		34					// HP상태 6에서의 처리방법 3

// AI_BOSSSTATUS TABLE DEFINE END.....


// AI_PHYSICALATTACK_BAG TABLE DEFINE

//#define FPTYPE_NO				1					// 공격번호 어택백 고유번호
#define FPATTACK1				0					// 공격번호, 마법번호 
#define FPATTACK2				1					// 
#define FPATTACK3				2
#define FPATTACK4				3
#define FPATTACK5				4

// AI_PHYSICALATTACK_BAG TABLE DEFINE END


// AI_ACTIONBAG TABLE DEFINE

#define	FAFUNC_NO				0
#define FAPARAM1				1
#define FAPARAM2				2

// Dragon SprNo DEFINE
#define	DRAGON_INDEX_NO		  50
#define DRAGON_HEART_INDEX_NO 52 
#define DRAGON_LORD_INDEX_NO  51

#endif