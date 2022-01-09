enum ChatTypes
{
	CHATTYPE_NORMAL				=0,
	CHATTYPE_WHISPER			=1,
	CHATTYPE_PARTY				=2,
	CHATTYPE_GUILD				=3,
	CHATTYPE_COLOSSUS_GLOBAL	=4,
	CHATTYPE_COLOSSUS			=5,
	CHATTYPE_LOCAL_WAR			=6,
	CHATTYPE_NATION_WAR			=7,
	CHATTYPE_GUILD_BBS			=8,
	CHATTYPE_YES_OR_NO			=9,
	CHATTYPE_NOTICE				=10,
	CHATTYPE_MERCHANT_BBS		=11,
	CHATTYPE_LMS				=12
};

void RecvChatSet( int cn, t_chat_set *p );
void SendWhisper(t_connection c[], int cn, t_packet *packet);
void SendChatParty( t_connection c[], int cn, t_packet *packet );
void SendCounselorChat( string chatData, ChatTypes chatType );

extern void CheckChatingData( char *name, char *message, unsigned char type );		// 0417 YGI