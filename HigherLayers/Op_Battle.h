#ifndef __OP_BATTLE__
#define __OP_BATTLE__

inline int IsHeNK(LPCHARLIST ch, int nation);
void InNK(LPCHARLIST ch, int nation, short int add_nk);
void killCharacter(CHARLIST *a, CHARLIST *d);
void CheckNK(int a_id, int d_id, int type);
inline int  IsHeNK(LPCHARLIST ch, int nation);

bool CanBattleArea(LPCHARLIST a, LPCHARLIST d);
int getDistance(CHARLIST *a, CHARLIST *d);
int addEXP(CHARLIST *a, CHARLIST *d, int criteria, int damage);
#endif