#ifndef __DUNGEON_H__
#define __DUNGEON_H__
//////////////////////////////////////////////////////////
// template <class T1> class TKeinList
//class CDungeonHistory		// 메시지 처리를 위해 

// 각 던전 맵 서버에서 처리
//class CDungeonObject		// 던전에 있는 오브젝트 클래스
//class CDungeonMap			// 맵단위( 2개의 층이 존재한다. )
//class CDungeonMapMgr		// 2개의 층을 관리

// 관리서버
//class CDungeonBasic		// 던전 ( 4개의 층이 존재한다. )
//class CDungeonMgr			// 4개의 던전을 관리


////////////////////////////////////////////////////////////////////////

#define SEND_ALL		-100

// SCENARIO_ITEM_NUMBER
#define SIN_TRAP_KEY_3	10210	
#define SIN_TRAP_KEY_5	10211
#define SIN_TRAP_KEY_7	10212


#define SMT_CREATE_DRAGON		7
#define SMT_CREATE_BOSS			8
#define SMT_DRAGONROAD_MESSAGE1	10		// 맨처음 드래곤 로드를 만났을때 
#define SMT_DRAGONROAD_MESSAGE2	11		// 모두 찬성했을때
#define SMT_DRAGONROAD_MESSAGE3	12		// 모두 반대 했을때
#define SMT_DRAGONROAD_MESSAGE4	13		// 결투 시작
#define SMT_DRAGONROAD_MESSAGE5	14		// 결투가 20분이 안지나도 안끝날때
#define SMT_DRAGONROAD_MESSAGE6	15		// 드래곤 로드의 패배
#define SMT_LAYER_POTAL_MESSAGE	16		// 
#define SMT_START_LAYER			17		// 몇번 층이 활성화 됐다.
#define SMT_KILL_BOSS			18		// 보스 몬스터를 죽였다.





// kein sub
//021030 YGI
template <class T1> class TKeinList
{
	T1	*m_pData;
	TKeinList<T1> *m_pPrev;
	TKeinList<T1> *m_pNext;
	TKeinList<T1> *m_pFind;
	
public :
	TKeinList() { Set(); }
	~TKeinList() { SAFE_DELETE(m_pData); SAFE_DELETE(GetNext()); }
	TKeinList<T1> *&GetNext(){ return m_pNext; }
	TKeinList<T1> *&GetPrev(){ return m_pPrev; }
	
	TKeinList<T1> *GetEndNode()
	{ 
		TKeinList<T1> *temp = this;
		for( ; temp->GetNext(); temp = temp->GetNext() );
		return temp;
	}
	void Set() { m_pData = 0; GetNext() = 0; GetPrev() = 0; }
	void clear()
	{
		SAFE_DELETE(m_pData);
		Set();
	}
	void SetFind()
	{
		m_pFind = this;
	}
	TKeinList<T1> *FindNext()
	{
		m_pFind = m_pFind->GetNext();
		return m_pFind;
	}
	T1 GetFind()
	{
		return *m_pFind->GetData();
	}
	bool AddNode( T1 data )
	{
		TKeinList<T1> *pEnd = GetEndNode();
		TKeinList<T1> *pAddData = new TKeinList<T1>;
		pAddData->m_pData = new T1;
		*pAddData->m_pData = data;
		pEnd->GetNext() = pAddData;
		pAddData->GetPrev() = pEnd;
		return true;
	}
	bool AddNode( T1 *pData )
	{
		TKeinList<T1> *pEnd = GetEndNode();
		TKeinList<T1> *pAddData = new TKeinList<T1>;
		pAddData->m_pData = pData;
		pEnd->GetNext() = pAddData;
		pAddData->GetPrev() = pEnd;
		return true;
	}
	bool DeleteNode( T1 data )
	{
		TKeinList<T1> *temp = this;
		while( temp=temp->FindNode(data) )
		{
			temp->GetPrev()->GetNext() = temp->GetNext();
			if( temp->GetNext() ) 
				temp->GetNext()->GetPrev() = temp->GetPrev();
			
			TKeinList<T1> *temp2;
			temp2 = temp->GetPrev();
			temp->GetNext() = 0;
			SAFE_DELETE(temp);
			temp = temp2;
		}
		return true;
	}
	TKeinList<T1> * FindNode( T1 data )
	{
		for( TKeinList<T1> *temp = GetNext(); temp; temp = temp->GetNext() ) 
		{
			if( *temp->m_pData == data )
			{
				return temp;
			}
		}
		return NULL;
	}
	bool DeleteFirst()
	{
		TKeinList<T1> *temp = GetNext();
		if( !temp ) return 0;
		TKeinList<T1> *next = temp->GetNext();
		GetNext() = next;
		
		if( next )
			next->GetPrev() = this;
		
		temp->GetNext() = 0;
		SAFE_DELETE(temp);
		return true;
	}
	bool DeleteEnd()
	{
		TKeinList<T1> *pEnd = GetEndNode();
		if( pEnd == this ) return 0;
		pEnd->GetPrev()->GetNext() = 0;
		SAFE_DELETE(pEnd);
		return 1;
	}
	int GetCount()
	{
		int count = 0;
		for( TKeinList<T1> *temp = this; temp->GetNext(); temp = temp->GetNext() ) count++;
		return count;
	}
	int AddNodeLimit( T1 *pData, int max, int delete_first = 1 )
	{
		AddNode( pData );
		if( GetCount() > max )
		{
			if( delete_first ) DeleteFirst();
		}
		return 1;
	}
	T1* GetData(int nIndex)
	{
		TKeinList<T1> *temp = GetNext();
		for( int i=0; i<nIndex; i++ )
		{
			if( !temp ) return NULL;
			temp = temp->GetNext();
		}
		return temp?temp->m_pData:NULL;
	}
	T1 *GetData()
	{
		return m_pData;
	}
};

///////////////////////////////////////////////////////////////////////////////

class CDungeonHistory
{
public :
	enum eDungeonHistoryType
	{
		NORMAL_TRAP_OPEN	= 1,		// 보통의 트랩
		NORMAL_TRAP_CLOSE	= 2,			
		MIDDLE_BOSS_OPEN	= 3,			// 중간 보스로 가는 포탈
		MIDDLE_BOSS_CLOSE	= 4,			
		LAYER_POTAL_OPEN	= 5,			// 다음 층으로 가는 포탈
		LAYER_POTAL_CLOSE	= 6,			
		DUNGEON_POTAL_OPEN	= 7,			// 밖으로 나가는 포탈
		DUNGEON_POTAL_CLOSE	= 8,
		DUNGEON_OPEN		= 9,				// 시나리오에 의해서 던전이 오픈되고 안되고
		DUNGEON_CLOSE		= 10,
		DUNGEON_WHEN_OPEN_CLOSE_NOTICE = 11,//시나리오가 언제 열리는지
	};
	
public :
	int m_nType;			// 공지 타입
	int m_nMonth;			// 시간
	int m_nDay;
	int m_nHour;
	int m_nMin;
	int m_nLayer;
	char m_szDungeon[20];
	char m_szName[20];
	
public :
	CDungeonHistory()
	{
		m_nType = 0;
		m_nMonth = 0;
		m_nDay = 0;
		m_nHour = 0;
		m_szDungeon[0] = 0;
		m_szName[0] = 0;
		m_nLayer = 0;
	}
	int SetData( int type, int layer, char *name, const char *mapname = NULL );
	CDungeonHistory & operator = ( const CDungeonHistory &data ) 
	{
		m_nType		= data.m_nType  ;
		m_nType		= data.m_nType	;	
		m_nMonth	= data.m_nMonth	;
		m_nDay		= data.m_nDay	;	
		m_nHour		= data.m_nHour	;
		m_nMin		= data.m_nMin ;
		strcpy_s( m_szDungeon, sizeof(m_szDungeon), data.m_szDungeon );
		strcpy_s( m_szName, sizeof(m_szName), data.m_szName );
		return *this;
	}
};

//////////////////////////////////////////////////////////////
class CScenarioBoss
{
public :
	enum
	{
		NT_NONE = 0,
			NT_NORMAL_DRAGON = 1,
			NT_DRAGON_LOAD = 2,
			NT_DRAGON_HEART = 3,
			NT_NORMAL_BOSS = 4,
			NT_FOLLOWER = 5,		// 졸병
	};
	enum
	{
		BT_NORMAL = 0,
			BT_LAYER_BOSS = 1,
			BT_DARAGON_HEART = 2,
			BT_DARAGON = 3,
	};
public :
	int m_nNpcType;
	int m_nSprNum;		// 스프라이트 넘버
	int m_nNpcIndex;	// NPCLIST 에서 사용하는 인덱스
	int m_nBossType;		// 그 층의 보스냐?
	
public :
	CScenarioBoss()
	{
		Clear();
	}
	void Clear()
	{
		m_nNpcType = 0;
		m_nSprNum = 0;
		m_nNpcIndex = 0;
		m_nBossType = 0;
	}
	~CScenarioBoss() { DeleteBoss(); }
	int GetBossType() { return m_nBossType; }
	int DeleteBoss();
	bool IsDragonLoad() { return (m_nNpcType==NT_DRAGON_LOAD)?true:false; }
	int CreateBoss( int npc_type, int npc_num = 0, int x=0, int y=0 );
	
};

//////////////////////////////////////////////////////////////
class CDungeonObject
{
public : 
	void *m_pMother;
public:
	enum 
	{
		TILE_NOT = 0,
			TILE_DONT = 1,
			TILE_EVENT = 2,
	};
	enum	// m_nNeedType		// 액티브 되는 조건
	{
		ONT_NEED_ITEM	= 1,		// 액티브 될때 아이템이 필요
			ONT_NEED_TRAP	= 2,		// 액티브시 다른 트랩이 필요 ( OET_NORMAL_TRAP 이 전부 액티브 되야 한다. )
			ONT_NEED_TRAP2	= 3,		// 액티브시 다른 트랩이 필요 ( 바로전 인덱스의 값이 액티브 되야 한다. )
			ONT_NEED_QUBE	= 4,		// 아아템으로 번호를 맞춰야 한다.
			ONT_NEED_QUBE2	= 5,		// 큐브가 다 액티브 될때 액티브 된다.
			ONT_NEED_DRAGON	= 6,		// 드래곤이 나타날때 액티브 ( 드래곤 로드가 나타나면 close )
			ONT_NEED_MONSTER = 7,		// 몬스터가 죽을때 액티브 된다.
	};
	enum	// m_nExecuteType	// 액티브시 해야 할 행동
	{
		OET_NORMAL_TRAP			= 1,
			OET_POTAL_BOSS			= 2,
			OET_POTAL_OUT_LAYER		= 3,
			OET_POTAL_OUT_DUNGEON	= 4,
			OET_POTAL_DRAGON		= 5,		// 드래곤으로 가는 포탈
			
			//OET_OTHER_ACTIVE_TRAP	= 5,	// 자기가 액티브 될때 다른 놈도 액티브 시켜준다.
	};
	
public:
	int m_Index;					// 트랩 번호 0번은 사용하지 않는다.
	int m_nLayer;					// 해당 층
	int m_nCloseObjectImage;		// 크로즈 됐을때 찍을 이미지 번호
	int m_nActiveObjectImage;		// 액티브 됐을때 찍을 이미지 번호
	int m_nActiveObjectType;		// 클라이언트에서 찍는 방법
	int m_nInputX;
	int m_nInputY;
	int m_nOutputX;
	int m_nOutputY;
	int m_nTime;					// 초
	int m_nNeedType;					// active 되는 조건( 1:아이템, 2:m_nType2의 값이 1인 전부 액티브 됐을때 액티브 된다. )
	int m_nExecuteType;					// active 됐을때 하는 행동( 1: m_nType1 의 값이 2인 오브젝트를 오픈 시켜라, 2:포탈기능
	bool m_bActive;					// 열렸는가 안 열렸는가		// active 와 같은 개념
	DWORD m_dwOpenTime;
	int m_ObjectId;					// 생성시킨 오브젝트 번호	// -1은 없음
	int m_nActiveItemNo;			// 작동시키는 아이템
	char m_szOutputMap[20];
	
	// 타일에 관련된
	// close 용
	int m_eTileType1;		// 0이면 없다.	enum 사용
	int m_nRange1;			// 속성을 칠 범위
	int m_nEventNo1;		// 밟아야 하는 이벤트 번호
	
	// active용
	int m_eTileType2;		// 0이면 없다.	enum 사용
	int m_nRange2;			// 속성을 칠 범위
	int m_nEventNo2;		// 밟아야 하는 이벤트 번호
	
	// 보스 생성
	CScenarioBoss	*m_pBoss;			// 보스
	int				m_nBossCount;		// 용일 경우 2개이다.
	CScenarioBoss	*m_pFollower;		// 쫄병
	int m_nBoss;				// 생성시킬 중간보스
	int m_nBossX;
	int m_nBossY;
	int m_nBossItem;
	int m_nResenTime;
	
	int m_nFollower;			// 주변에 같이 나오는 쫄병
	int m_nFollowerCount;		// 쫄병 수

	int m_current_floor;		//Eleval 10/08/09 - To bring ppl back in quest
	
	
public:
	CDungeonObject()
	{
		m_Index = 0;
		m_bActive = 0;
		m_ObjectId = -1;
		m_nCloseObjectImage = 0;
		m_nActiveObjectImage = 0;
		m_eTileType1 = TILE_NOT;
		m_eTileType2 = TILE_NOT;
		m_pMother = 0;
		m_pBoss = 0;
		m_pFollower = 0;
		m_nBoss = 0;
		m_nBossX = 0;
		m_nBossY = 0;
		m_nBossItem = 0;
		m_nFollower = 0;
		m_nFollowerCount = 0;
		m_nResenTime = -1;
		m_nBossCount = 0;
	}
	~CDungeonObject();
	
	bool IsThisLayerActive();
	bool IsNormalTrap() { return ( m_nExecuteType == OET_NORMAL_TRAP )?true:false; }
	void SetMotherPointer( void *pointer ) { m_pMother = pointer; }
	int CheckCloseTime();
	int IsActive() { return m_bActive; }
	void CloseObject( int npc_create_type = 1 );
	int CreateNormalBoss();		// 중간 보스 생성
	void ActiveObject();
	int Execute( short int cn );
	bool TryActive( CHARLIST *ch );
	bool SetActive( CHARLIST *ch );
	int DeleteObjectImage();
	int Start();
	int CreateScenarioBoss( int boss_type = CScenarioBoss::NT_NORMAL_BOSS);
	int KillBoss( CHARLIST *user, CHARLIST *boss );		// npc가 죽었을때 그 npc가 시나리오 보스일경우 처리
	int DeletBoss();									// 강제로 보스를 없앨 경우
	bool IsDragonLoad();
	void SendObjectTileAttr( short int cn );
};

//#define _KEIN_TEST_
const int MESSAGE_COUNT = 2;
class CDungeonMap		// 맵단위
{
public : 
	enum	// m_nDungeonType 
	{
		DT_NONE = 1,
			DT_PASSWORD = 2,
	};
public:
	TKeinList<CDungeonHistory> m_History;
	
	CDungeonObject *m_pObject;
	int m_nObjectCount;
	
	int m_nLayer;
	int m_nTrapCount;			// 대미궁에서 암호로 사용한다.
	int m_nDungeonType;			// 
	
	bool m_bActive;
	int m_nMessageCount;
	int m_pKey[7];
	int m_nMessageOption;
	
	bool m_bDragonRoad;
	
	char m_szOutMap[20];
	int m_nOutX;
	int m_nOutY;
	
public :
	CDungeonMap( int flag = 1 )
	{
		m_pObject = 0;
		m_bDragonRoad = 0;
		Clear();
	}
	~CDungeonMap()
	{
		Clear();
	}
	void SetDragonRoad() { m_bDragonRoad = !m_bDragonRoad; }
	
	bool IsDragonRoad() 
	{	//< CSD-030304
		
		/*
		#ifdef _KEIN_TEST_
		return 1;
		#endif
		return m_bDragonRoad; 
		*/
		return false; // 무조건 레드 드래곤이 나오도록 설정
	}	//> CSD-030304
	
	int SetTrapKey( int item_no, CHARLIST *ch );
	int Clear()
	{
		SAFE_DELETE_ARRAY(m_pObject);
		m_nObjectCount = 0;
		m_nLayer = 0;
		m_nTrapCount = 0;
		m_nDungeonType = 0;
		m_bActive = 0;
		m_nMessageCount = 0;
		m_szOutMap[0] = 0;
		m_nOutX = 0;
		m_nOutY = 0;
		
		ResetKey();
		return 1;
	}
	int Load( int layer );
	int SetDungeonObject( int count )
	{
		m_nObjectCount = count;
		m_pObject = new CDungeonObject[count];
		for( int i=0; i<count; i++ )
		{
			m_pObject[i].SetMotherPointer( this );
		}
		
		return 1;
	}
	void ResetKey()
	{
		memset( m_pKey, 0, sizeof( int )*7 );
		m_nMessageOption = 1;
	}
	int IsActive() { return m_bActive; }
	int SetActive();
	int SetClose();
	int NewJoin( short int cn );
	bool CheckObject( int cn, int event_no );
	CDungeonObject *GetObjectByEventNo( int event_no );
	int SendEventWhenAddItem( int item_index, short int cn );
	int SetInt( int *pTemp, int nMax );
	int SetHistory( CHARLIST *ch, CDungeonObject *pDO, int type ); 
	void Proc();
	int GetTrapCount( int &count, int &intctive_count , int &aactive_time);
	int KillBoss( CHARLIST *user, CHARLIST *boss );		// 보스가 죽었다.
	void OpenDragonRoadObject();
	void CloseDragonRoadObject();
	int DeleteDragon();
	int SendTileAttrToNewUser( short int cn );		// 새로 들어온 놈에게 이미 만들어 져 있는 속성을 모두 보낸다.
};

class CDungeonMapMgr
{
public :
	enum		// GET_OUT_TYPE
	{
		GOT_END_BOSS_POTAL	= 1,	// 보스 포탈이 닫일때, 보스 포탈 안 탄놈
			GOT_END_LAYER		= 2,	// 층 포탈이 닫일때, 팅기기
			GOT_END_SCENARIO	= 3,	// 시나리오 종료
			GOT_END_VOTE		= 4,	// 찬반 끝났을때
			// 늘어나면 GetOutDugeon() 에 배열을 늘려야 한다.
	};
	
public :
	int m_nCount;
	CDungeonMap* m_pData;
	TKeinList< int > *pVoteTrue;		// 찬성
	TKeinList< int > *pVoteFalse;		// 반대
	TKeinList< int > *pPotalList;		// 보스 포탈을 거친 사람 등록
	
public :
	CDungeonMapMgr( int N )
	{
		m_pData = new CDungeonMap[N];
		m_nCount = N;
		pVoteTrue = 0;
		pVoteFalse = 0;
		pPotalList = 0;
	}
	~CDungeonMapMgr()
	{
		m_nCount = 0;
		SAFE_DELETE_ARRAY(m_pData);
		CloseDungeon();
	}
	void CloseDungeon()
	{
		ClearVote();
		DeletePotalMember();
	}
	
	void ClearVote()
	{
		SAFE_DELETE(pVoteTrue);
		SAFE_DELETE(pVoteFalse);
	}
	void SetVote()
	{
		ClearVote();
		pVoteTrue = new TKeinList< int >;
		pVoteFalse = new TKeinList< int >;
	}
	void DeletePotalMember()
	{
		if( pPotalList ) delete pPotalList;
		pPotalList = 0;
	}
	void InitPotalMember()		// 포탈이 생성 될때 사용
	{
		DeletePotalMember();
		pPotalList = new TKeinList< int >;
	}
	void AddPotalMember( int cn )
	{
		int *data = new int;
		*data = cn;
		if( !pPotalList ) 
		{
			InitPotalMember();
		}
		pPotalList->AddNode( data );
	}
	int SendEventWhenAddItem( int id, int cn )
	{
		int ret = 0;
		for( int i=0; i<m_nCount; i++ )
		{
			ret += m_pData[i].SendEventWhenAddItem( id, cn );
		}
		return ret;
	}
	bool CheckObject( int cn, int event_no )
	{
		int ret = 0;
		for( int i=0; i<m_nCount; i++ )
		{
			if( m_pData[i].m_bActive )
				ret += m_pData[i].CheckObject( cn, event_no )?1:0;
		}
		return ret?true:false;
	}
	void SetSatus( char *value )
	{
		int scenario_end = 1;
		for( int i=0; i<m_nCount; i++ )
		{
			if( value[m_pData[i].m_nLayer-1] )
			{
				m_pData[i].SetActive();
				scenario_end = 0;
			}
			else
			{
				m_pData[i].SetClose();
			}
		}
		
		if( scenario_end )		// 전부 팅기자..
			GetOutDugeon( CDungeonMapMgr::GOT_END_SCENARIO );
	}
	CDungeonMap* GetDungeonMapByLayer( int layer )
	{
		for( int i=0; i<m_nCount; i++ )
		{
			if( m_pData[i].m_nLayer == layer ) return &m_pData[i];
		}
		return NULL;
	}
	CDungeonMap* GetActiveDungeonMap()
	{
		for( int i=m_nCount-1; i>=0; i-- )
		{
			if( m_pData[i].m_bActive ) return &m_pData[i];
		}
		return NULL;
	}
	CDungeonMap* GetDungeonMap()		// 처음껄 가져온다.
	{
		return &m_pData[0];
	}
	void LoadTrap()
	{
		for( int i=0; i<m_nCount; i++ )
		{
			m_pData[i].Load( m_pData[i].m_nLayer );
		}
	}
	void Proc()
	{
		for( int i=0; i<m_nCount; i++ )
		{
			m_pData[i].Proc();
		}
	}
	int GetTrapCount( int &count, int &active_count , int &active_time )
	{
		CDungeonMap* pDM = GetActiveDungeonMap();
		return pDM?pDM->GetTrapCount( count, active_count, active_time ):0;
	}
	int KilledScenarioBoss( CHARLIST *user, CHARLIST *npc )
	{
		for( int i=0; i<m_nCount; i++ )
		{
			m_pData[i].KillBoss( user, npc );
		}
		return 1;
	}
	int CloseConnect( int cn )
	{
		if( pVoteTrue ) 
		{
			pVoteTrue->DeleteNode( cn );
			pVoteFalse->DeleteNode( cn );
			return 1;
		}
		if( pPotalList )
		{
			pPotalList->DeleteNode( cn );
			return 1;
		}
		else return 0;
	}
	void GetOutDugeon( int type );
	int ScenarioFightProc( CDungeonObject *pDO );
	int EndUserFightByDragon();
	int DropDragonItem();
	void NewJoin( short int cn )
	{
		// 언데드 끼임 현상 해결 // 030811
		for( int i=0; i<m_nCount; i++ )
		{
			m_pData[i].NewJoin( cn );
		}
	}
};

class CDungeonBasic
{
public:
	enum 
	{
		TYPE_STATUS = 1,
		TYPE_STATUS_REGIST = 2,
	};

	typedef vector<START_UP_INFO>     VECTOR_START_UP; // CSD-030323
	typedef VECTOR_START_UP::iterator ITOR_START_UP;   // CSD-030323
	
public :
	int m_nId;
	int m_nDungeonMapPort1;
	int m_nDungeonMapPort2;
	union 
	{
		int m_nActive;
		BYTE m_pActive[4];
	};
	
protected:
	int             m_nIndex;
	VECTOR_START_UP m_vtStartUpInfo;
	
public:
	int m_nContinueTime; // 액티브 되어 있는 시간 - 2시간
	int m_nStartTime;	 // 현재 상태 진행된 시간
	int m_nWaitTime;	 // 기달려야 하는 시간
	
public :
	CDungeonBasic()
	{
		m_nId = 0;
		m_nContinueTime = 0;
		m_nStartTime = 0;
		m_nWaitTime = 0;
		m_nDungeonMapPort1 = 0;
		m_nDungeonMapPort2 = 0;
		m_nActive = 0;
		m_nIndex = -1;
		m_vtStartUpInfo.clear();
	}
	~CDungeonBasic()
	{
		m_vtStartUpInfo.clear();
	}

	void SetID(int nID) { m_nId = nID; }
	
	void AddStartUpInfo(DWORD idIndex, int nApplyDay, int nStartHour, int nEndHour) 
	{ 
		START_UP_INFO infStartUp;
		infStartUp.idIndex = idIndex;
		infStartUp.nApplyDay = nApplyDay;
		infStartUp.nStartHour = nStartHour;
		infStartUp.nEndHour = nEndHour;
		infStartUp.nEndMinute = 0;
		infStartUp.nStartMinute = 0;
		m_vtStartUpInfo.push_back(infStartUp);
	}
	
	START_UP_INFO* GetStartUpInfo(DWORD idIndex)
	{
		for (int i = 0; i < GetStartUpInfoCount(); ++i)
		{
			if (idIndex == m_vtStartUpInfo[i].idIndex)
			{
				return &m_vtStartUpInfo[i];
			}
		}
		
		return NULL;
	}
	
	int GetStartUpInfoCount()const{return m_vtStartUpInfo.size();}//030321 lsw
	
	VECTOR_START_UP& GetStartUpList()
	{
		return m_vtStartUpInfo;
	}
	
	void SetStartUpInfo(DWORD idIndex, int nApplyDay, int nStartHour, int nStartMinute, int nEndHour, int nEndMinute)
	{
		for (int i = 0; i < GetStartUpInfoCount(); ++i)
		{
			if (idIndex == m_vtStartUpInfo[i].idIndex)
			{
				m_vtStartUpInfo[i].nApplyDay = nApplyDay;
				m_vtStartUpInfo[i].nStartHour = nStartHour;
				m_vtStartUpInfo[i].nStartMinute = nStartMinute;
				m_vtStartUpInfo[i].nEndHour = nEndHour;
				m_vtStartUpInfo[i].nEndMinute = nEndMinute;
				return;
			}
		}
	}
	
	
	bool IsExistStartUp(DWORD idIndex)const
	{
		for (int i = 0; i < GetStartUpInfoCount(); ++i)
		{
			if (idIndex == m_vtStartUpInfo[i].idIndex)
			{
				return true;
			}
		}
		
		return false;
	}
	
	void SetDungeonMapPort1(int nPort) { m_nDungeonMapPort1 = nPort; }
	void SetDungeonMapPort2(int nPort) { m_nDungeonMapPort2 = nPort; }
	
	int SetInt(int* data)
	{	//< CSD-030723
		int* set_int =  &m_nId;
		
		for (int i = 0; i < MAX_CDUNGEONBASIC_INT; ++i)
		{
			set_int[i] = data[i];
		}

		return 1;
	}	//> CSD-030723

	int Proc();
	int SendScenarioStatus();
	int SaveDungeonStatus();
	int SetActiveFlag( int layer, int value=0, int type = 0 )
	{
		if( !layer ) m_nActive = 0;
		else
		{
			if( type )
			{
				for( int i=0; i<4; i++ )
				{
					if( i == layer-1 ) continue;
					m_pActive[i] = 0;
				}
			}
			m_pActive[layer-1] = value;
		}
		SendScenarioStatus();
		return 1;
	}
	int IsActive( int layer )
	{
		if( layer < 0 ) return 0;
		return m_pActive[layer-1];
	}
	
	bool IsOpenDungeon(int nIndex) const
	{
		struct tm* NewTime;
		time_t LongTime;
		time(&LongTime);
		NewTime = localtime(&LongTime);
		
		if (NewTime->tm_wday != m_vtStartUpInfo[nIndex].nApplyDay)
		{
			return false;
		}
		
		if (g_hour != m_vtStartUpInfo[nIndex].nStartHour)
		{
			return false;
		}

		if (g_min != m_vtStartUpInfo[nIndex].nStartMinute)
		{
			return false;
		}

		return true;
	}
	
	bool IsCloseDungeon(int nIndex) const
	{
		if (nIndex < 0)
		{
			return false;
		}
		
		if (g_hour != m_vtStartUpInfo[nIndex].nEndHour)
		{
			return false;
		}
		
		if (g_min != m_vtStartUpInfo[nIndex].nEndMinute)
		{
			return false;
		}

		return true;
	}

	// calcurate remain time	// 031110 YGI
	int GetRemainTime( ) const 
	{
		if(m_nIndex< 0)
		{
			return false;
		}
		int temp_time = 0;

		temp_time = (m_vtStartUpInfo[m_nIndex].nEndHour%24) - (g_hour%24);
		temp_time += 24;
		temp_time %= 24;
		temp_time = 60*temp_time+m_vtStartUpInfo[m_nIndex].nEndMinute - g_min;	
		return temp_time;
	}
};

class CDungeonMgr		// 4개의 던전을 관리
{
	CDungeonBasic** m_pData;
	int m_nCount;
public :
	TKeinList<CDungeonHistory> m_History;
	
	
public : 
	CDungeonMgr( int N )
	{
		m_pData = new CDungeonBasic*[N];
		for( int i=0; i<N; i++ )
		{
			m_pData[i] = 0;
		}
		m_nCount = N;
	}
	~CDungeonMgr() 
	{ 
		for( int i=0; i<m_nCount; i++ )
		{
			CDungeonBasic* pTemp = GetDungeon(i);
			if( pTemp )
			{
				SAFE_DELETE(pTemp);
			}
		}
		SAFE_DELETE_ARRAY(m_pData);
		m_nCount = 0;
	}
	int SaveDungeonStatus()
	{
		for( int i=0; i<m_nCount; i++ )
		{
			CDungeonBasic* pTemp = GetDungeon(i);
			if(pTemp)
			{
				pTemp->SaveDungeonStatus();
			}
		}
		return 1;
	}
	bool SetData( int i, CDungeonBasic *pData )
	{
		if( i>=m_nCount ){return false;}
		
		SAFE_DELETE(m_pData[i]);
		m_pData[i] = pData;
		return true;
	}
	CDungeonBasic* GetDungeon( int index ) 
	{
		if( index>=m_nCount ) {return NULL;}
		return m_pData[index];
	}
	
	CDungeonBasic* GetDungeonByPort( WORD port ) 
	{
		for( int i=0; i<m_nCount; i++ )
		{
			CDungeonBasic* pTemp = GetDungeon(i);
			if( pTemp )
			{
				if( pTemp->m_nDungeonMapPort1 == port 
					|| pTemp->m_nDungeonMapPort2 == port )
				{
					return m_pData[i];
				}
			}
		}
		return NULL;
	}
	
	int GetCount() { return m_nCount; }
	int Proc();
	int CheckActive( int port )		// 그 맵에 들어갈 수 있는지 없는지
	{
		CDungeonBasic* temp = GetDungeonByPort(port);
		if( temp ) 
		{
			if( !temp->m_nActive || temp->m_pActive[0] ) return 1;
		}
		return 0;
	}
	int SendScenarioStatus()
	{
		for( int i=0; i<m_nCount; i++ )
		{
			CDungeonBasic* pTemp = GetDungeon(i);
			if( pTemp )
			{
				pTemp->SendScenarioStatus();
			}
		}
		return 1;
	}
};

//////////////////////////////////////////////////////////////////
#define DRAGON_ITEMCONTROL_NUMBER	81

class CScenarioDragonItem
{
public :
	ItemAttr *m_pTotalItem;
	int		m_nTotalCount;
	
public :
	CScenarioDragonItem()
	{
		m_pTotalItem = 0;
		m_nTotalCount = 0;
	}
	~CScenarioDragonItem()
	{
		SAFE_DELETE_ARRAY(m_pTotalItem);
		m_pTotalItem = 0;
		m_nTotalCount = 0;
	}
	
	int MakeDragonItem( int total );
	int DropDragonItem( TKeinList<int> *pLink1, TKeinList<int> *pLink2 );
};




//////////////////////////////////////////////////////////////////

extern void SendTileAttr( int index, int type, int number, int x, int y, int range, short int cn );
extern int LoadDungeonTrap( CDungeonObject *pData, int layer );
extern int LoadDungeonMapEach( int port, int *temp );


extern CDungeonMapMgr *g_pDungeonMap;		// 해당 맵에 있는 던전들( 1층, 2층 )의 관리 클래스
extern CDungeonMgr *g_pDungeonMgr;				// 모든 던전( 아비스 미궁,.. )의 관리 클래스

extern CScenarioDragonItem	*g_pDragonItem;


extern void RecvGetScenarioInfo( t_packet *p, short int cn );
extern void RecvGetScenarioInfoMap( t_packet *p, int type = 0 );
extern void RecvScenarioCommand( t_packet *p );
extern void RecvCheckGotoScenarioDungeon( t_packet *p, short int cn );
extern void RecvCheckGotoScenarioDungeonResult( t_packet *p, short int cn );
extern void SendCheckGotoScenarioDungeon( short int cn, char *map_name, int x, int y );
extern int SendDungeonHistory( CDungeonHistory *pHistory );
extern void RecvScenarioMessageMap( t_packet *p, short int cn );
extern void SendRegistDungeonMgrServer( int type, void *msg, int size );
extern void RecvScenarioDetailInfo( t_packet *p, short int cn );
extern void RecvScenarioDetailInfoMap( t_packet *p, short int cn );
extern void SendDeleteScenarioEventBox( int index );
extern void RecvScenarioTrapKeyInfo( t_packet *p, short int cn );
extern void SendScenarioMessage( short int cn, int type, void *data = NULL, int size = 0 );

extern void DeleteScenarioItem( CHARLIST *ch );
extern void RecvDragonVote( t_packet *p, short int cn );
extern void ScenarioDragonEndWar();
extern void ScenarioDragonMsgFunc();
extern void RecvGetScenarioTime( t_packet *p, short int cn );
extern void SendGetScenarioTime(int port, int cn, k_client_scenario_time_info* pPacket); // CSD-030306
extern void RecvChangeScenarioTime( t_packet *p, short int cn );
extern int KilledScenarioBoss( CHARLIST *user, CHARLIST *npc );		// 시나라리오 몬스터가 죽었다.

extern void BringBackInSQuest(short int ClientNumber, int QuestFloor, int nbMap, int nX, int nY); //Eleval 10/08/09 - To bring ppl back in quest


#endif



