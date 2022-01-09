#pragma once

/* bitmask rule

   [28 - 31]					[0-27]
  |   HEADER |	    USER & SERVER INDEX FIELD	    |	
  |   0000   |   0000 0000 0000 0000 0000 0000 0000 |

  ACTOR_TYPE [31] bit field
  When SERVER , 1  
  When USER ,	0
  
  ACTION TYPE [30-28] bit field
  When Recv TCP Packet	, 000
  When Recv UDP Paccket , 001
  When Disconnect Socket, 010
  Whend Must Switch Que,  011
  


*/

enum ACTOR_TYPE 
{
	ACTOR_TYPE_SERVER = 0x80000000,
	ACTOR_TYPE_USER	 = 0x00000000
};
enum ACTION_TYPE
{
	ACTION_TYPE_TCP =			0x00000000,
	ACTION_TYPE_UDP =			0x10000000,
	ACTION_TYPE_DISCONNECT =	0x20000000,
	ACTION_TYPE_SWITCH_QUE =	0x30000000,
	ACTION_TYPE_DESTROY =		0x40000000
};

enum NETWORK_ID 
{
	ID_NETWORK_FOR_USER		=	ACTOR_TYPE_USER,
	ID_NETWORK_FOR_SERVER	=	ACTOR_TYPE_SERVER,
	ID_NETWORK_FOR_UDP		=	ACTION_TYPE_UDP
};

#define ACTION_TYPE_BIT_MASK		0x70000000
#define ACTOR_TYPE_BIT_MASK			0x80000000
#define HEADER_BIT_MASK				0xf0000000
#define CONNECTION_INDEX_BIT_MASK	0x0fffffff 

#define INVERSE_ACTION_TYPE_BIT_MASK	0x8fffffff
#define INVERSE_ACTOR_TYPE_BIT_MASK		0x7fffffff

enum CONNECTION_STATUS 
{
	CONNECTION_STATUS_CRASHED = 1011 ,
	CONNECTION_STATUS_CANNOT_SEND = 1012,
	CONNECTION_STATUS_CAN_SEND = 1013
};

enum MAIN_THREAD_EVENT_INDEX
{
	EVENT_INDEX_DESTROY			= 0,
	EVENT_INDEX_MSG_EVENT		= 1,
	EVENT_INDEX_BREAK			= 2,
	EVENT_INDEX_RESUME			= 3,
	EVENT_INDEX_USER_DEFINE_0	= 4,
	EVENT_INDEX_USER_DEFINE_1	= 5,
	EVENT_INDEX_USER_DEFINE_2	= 6,
	EVENT_INDEX_USER_DEFINE_3	= 7,
	EVENT_INDEX_USER_DEFINE_4	= 8
};

#define MIM_MAIN_THREAD_EVENT_NUM  4
#define MAX_MAIN_THREAD_EVENT_NUM  64
#define	MAX_MAIN_THREAD_USER_DEFINE_EVENT_NUM 60