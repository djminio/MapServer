extern void CreateCharacter( t_connection c[], int cn, t_packet *packet);
extern void UserAddBasicData(  t_connection c[], int i, t_server_user_add *p );
extern void PutPacketCharDB( t_connection c[], int cn, t_packet *packet );
extern void RecvLevelUpPoint( int cn, t_client_levelup_point *p );
extern void SendItemInventory( int, t_connection c[], int cn );
extern void SendItemEquip( t_connection c[], int cn );
extern void SendItemQuick( t_connection c[], int cn );
extern void SendCharInfoMagic( t_connection c[], int cn );
extern void SendCharInfoSkill( t_connection c[], int cn );
extern void SendCharInfotac_skillEXP( t_connection c[], int cn );
extern void SendItemIndex( t_connection c[], int cn );		// 배운 아이템 보내주기
extern void SendDiesease( t_connection c[], int cn );		// diesease 보내주기 
extern void SendEmployment( t_connection c[], int cn );		// employment 보내주기 
extern void SendRelation( t_connection c[], int cn );		// 사제 관계보내주기
extern void SendParty( t_connection c[], int cn );			// 파티원 보내주기 
extern void RecvCharInfoPlus( int type, t_connection c[], int cn );
extern void SendPartyJoinOk( char *name, int cn );
extern int	ExistHe( char *name );
extern int ExistConnectingHe( char *name );
extern void ReqPartyDB( short int client_id, char para, char *name );		// 0217 YGI
extern void CheckCharacterWhenJoinGame( CHARLIST *ch );	// 011101 YGI
extern int InitPartyMemberServerToServer( CHARLIST *ch );		//  이름만 디비에 저장하고 이름만 꺼내온다.
extern int EndsetPartyMenberForUpdate( CHARLIST *ch );
extern int SetMySkill( CHARLIST *ch );		// 일단 기본적으로 알아야하는 스킬 셋
extern void ExpDown(CHARLIST* pCaster, CHARLIST* pTarget, bool bDrop); // CSD-030314
extern void HungryMuchAdd( CHARLIST *ch, const int iType);//020214 lsw
//extern void CheckHungryTimeElapsed( CHARLIST *ch, int cn );//020214 lsw
