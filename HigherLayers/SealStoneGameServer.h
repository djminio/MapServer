
extern int KingCommandBlock;
extern int SealStoneBreakable;

extern int  ReturnSealStoneBreakable( void );

extern void Recv_CMD_SEALSTONE_STATUS( t_sealstone_status *tp );
extern void Recv_CMD_SEALSTONE_WAR_START( int nation );
extern void Recv_CMD_SEALSTONE_RESULT( t_sealstone_result *tp );
extern void Recv_CMD_SEALSTONE_WAR_LEFTTIME(	int nation, DWORD lefttime );

extern void SendSealStoneFromKing( int cn );
extern void Send_CMD_SEALSTONE_STATUS( LPCHARLIST ch, int status );

extern void RecvNation2NationRelationResult( t_packet *p );


extern int SendNation2NationRelation( int a_nation, int b_nation, int relation, int year = 0, int yday = 0, int hour = 0, DWORD howlong = 0 );

