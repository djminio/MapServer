// WheatherControl.cpp: implementation of the CWeatherControl class.
//
//////////////////////////////////////////////////////////////////////
#include "..\stdafx.h"
#include "WeatherControl.h"
#include "DefaultHeader.h"

#include "CItem.h"
#include "UserManager.h"
	
int RainTable[12][31];
short Temprature[12][31][2];


DWORD today_rainstart[5];
DWORD today_rainend[5];
int   today_rainamount[5];
int   today_temperature;
int   today_weathercount;

bool g_X_MAS;	

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CWeatherControl WeatherControl;
CRanWeatherSystem RandomWeatherSystem;

CWeatherControl::CWeatherControl()
{
	dwCheckedTime = timeGetTime();
}

CWeatherControl::~CWeatherControl()
{

}

void CWeatherControl::AutoSetDayLight(CHARLIST *ch)
{
	short int nTempDaylight = nDayLightControl;//nTempDaylight 셋팅될 결과물
	if(ch->dwDayLightControlCustomTime  )
	{
		nTempDaylight = nDayLightControl;
		if(ch->dwDayLightControlCustomTime < global_time)//커스텀 시간이 있다 그러나 시간이 지났다
		{
			ch->dwDayLightControlCustomTime = 0;
		}
		else
		{
			nTempDaylight = nDayLightControl+ ch->nDayLightControlCustom;
			if(nTempDaylight > nMaximumLight)
			{
				nTempDaylight = nMaximumLight;
			}
		}
	}

	//if(ch->nDayLightControl != nTempDaylight )
	//{
	//	t_packet packet;
	//	packet.h.header.type = CMD_DAYLIGHT;
	//	packet.h.header.size = sizeof(t_DayLight);

	//	ch->nDayLightControl = nTempDaylight;

	//	packet.u.DayLight.nDayLightControl	= nTempDaylight;//011215 lsw 
	//	QueuePacket(connections, ch->GetServerID(), &packet, 1);
	//}
}

void CWeatherControl::CheckDayLight()
{
	if( timeGetTime()  <=  dwCheckedTime )//시간 체크
	{
		return;
	}

//	dwCheckedTime = timeGetTime()+ (60000 * 10 );//10분
	dwCheckedTime = timeGetTime()+ (10 );//10분

	switch(MapInfo[MapNumber].daylightcontrol)
	{
	case 1://2: 3: 1//새벽(0,1) , 낮(2,3,4), 밤(5)
		{
			const int iGameHour = g_hour%6;
			short int nValue = nMinimumLight;
			switch(iGameHour)
			{
			case 0:
				{
					nValue = 15;
				}break;
			case 1:
				{
					nValue = 20;
				}break;
			case 2:
				{
					nValue = 25;
				}break;
			case 3:
				{
					nValue = nMaximumLight;
				}break;
			case 4:
				{
					nValue = 25;
					if(g_min >30)//30분이면 라이트가 5 감소
					{
						nValue-=5;
					}
				}break;
			case 5:
				{
					nValue = nMinimumLight;
				}break;			
			default:
				{
					nValue = nMinimumLight;
				}break;
			}

			SetDayLight(nValue);
/*			
			int min = (g_hour/4) * 60 + g_min;
			if( min >= 360 && min <= 360 + (nMaximumLight-nMinimumLight) )	
			{
				SetDayLight(min + nMinimumLight - 360);
			}
			else if( min > 360 + (nMaximumLight-nMinimumLight)  && min < 18 * 60 )	
			{
				SetDayLight(nMaximumLight);
			}
			else if( min >= 18 * 60 && min < 18*60 + (nMaximumLight-nMinimumLight))	
			{
				SetDayLight(18*60 + nMaximumLight - min);
			}
			else 
			{
				SetDayLight(nMinimumLight);
			}
*/
		}break;
	case 2:
		{
			SetDayLight(nMaximumLight);
		}break;
	case 0:
	default:
		{
			SetDayLight(nMinimumLight);
		}break;
	}
}

void CWeatherControl::SetChLight(CHARLIST *ch,const int iLight,const int iTimeSec )
{
	if(!ch)	{ return; }
	ch->dwDayLightControlCustomTime = global_time +iTimeSec*1000;//*1000;//5분
	ch->nDayLightControlCustom = nDayLightControl + iLight;
	ch->nDayLightControl = nMinimumLight-1;
}

void CWeatherControl::SetDayLight(const short int nValue)
{
	nDayLightControl = nValue;
}

int LoadWeatherTable( void )
{
	FILE *fp;
	int dummy;
	int i;

	char temp[ MAX_PATH];
	wsprintf( temp, "%s/data/rain.tbl", GameServerDataPath );
	fp = fopen( temp, "rt" );
	if( fp == NULL ) return 0;

	for( i = 0 ; i < 30 ; i ++)
	{
		fscanf_s( fp, "%d %d %d %d %d %d %d %d %d %d %d %d %d \n", 
			&dummy,
			&RainTable[0][i], 
			&RainTable[1][i], 
			&RainTable[2][i], 
			&RainTable[3][i], 
			&RainTable[4][i], 
			&RainTable[5][i], 
			&RainTable[6][i], 
			&RainTable[7][i], 
			&RainTable[8][i], 
			&RainTable[9][i], 
			&RainTable[10][i], 
			&RainTable[11][i] );
	}
	fclose(fp);


	fp = fopen( "./data/temprature.tbl", "rt" );
	if( fp == NULL ) return 0;
	for( i = 0 ; i < 31 ; i ++)
	{	
		fscanf_s( fp, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n", 
			&dummy,
			&Temprature[0][i][0],  &Temprature[0][i][1],
			&Temprature[1][i][0],  &Temprature[1][i][1],
			&Temprature[2][i][0],  &Temprature[2][i][1],
			&Temprature[3][i][0],  &Temprature[3][i][1],
			&Temprature[4][i][0],  &Temprature[4][i][1],
			&Temprature[5][i][0],  &Temprature[5][i][1],
			&Temprature[6][i][0],  &Temprature[6][i][1],
			&Temprature[7][i][0],  &Temprature[7][i][1],
			&Temprature[8][i][0],  &Temprature[8][i][1],
			&Temprature[9][i][0],  &Temprature[9][i][1],
			&Temprature[10][i][0], &Temprature[10][i][1],
			&Temprature[11][i][0], &Temprature[11][i][1] );
	
	}	
	fclose(fp);

	return 1;
}
		
void CurrentGameDate( const DWORD t, int *y, int *mo, int *d, int *h, int *mi, int *sec )
{		
	DWORD rest;
		
	*y		= t / 31536000;
	rest	= t % 31536000;
		
	*mo		= rest / 2592000;
	rest	= rest % 2592000;
		
	*d		= rest / 86400;
	rest	= rest % 86400;
		
	*h		= rest / 3600;
	rest	= rest % 3600;
		
	*mi		= rest / 60;
	*sec	= rest % 60;
}		

void CheckWeatherSystem( void )
{		
	static DWORD time = g_curr_time;
		
	if( g_curr_time - time > 600 )
	{	
		time = g_curr_time; // 한 게임시간 Check한다. 
	}	
	else return;
			
	static int mon, day, rain_amount;
	int y, mo, d, h, mi, sec;
	int i;	
			
	CurrentGameDate( g_curr_time * 6, &y, &mo, &d, &h, &mi, &sec );
			
	static int to;
	//	만약에 크리스마스이면..
	g_X_MAS = false;
	if( ( g_mon == 11 && g_day >=1 ) || (g_mon==0 && g_day <= 30 ) )		// 021128 YGI
	{
		g_X_MAS = true;
	}

	if( g_X_MAS && MapInfo[ MapNumber].rain )		// 눈내려라..
	{		
		to ++;
		if( to < 10 ) 
		{	
			today_rainstart[0]  =  time;
			today_rainend[0]    =  time + 1200;
			today_rainamount[0] =  600;
			today_weathercount  =  0;

			/*int xx, yy;
			for( i = 0 ; i < 10 ; i ++)
			{		
				int xx, yy;
				xx = rand()%g_Map.file.wWidth;
				yy = rand()%g_Map.file.wHeight;
				if( !TileMap[ xx][ yy].attr_inside && !TileMap[ xx][ yy].attr_dont )
				{	
					xx *= TILE_SIZE;
					yy *= TILE_SIZE;
					ItemAttr item;
					item = GenerateItem( 7, 44, (rand()%6)+10, 0, 100 );	
					AddItemList( item.item_no, item.attr,0,  xx+rand()%TILE_SIZE, yy+rand()%TILE_SIZE, 0,0 );
				}	
			}*/		
		}			
		else		
		{			
			today_rainstart[0]  =  0;
			today_rainend[0]    =  0;
			today_rainamount[0] =  0;
			today_weathercount  =  0;

			if( to > 13 ) to = 0;
		}
		
		return;
	}	
		
	if( mon == 6 && day == 10 ) 
	{	
		today_weathercount --;
		if( today_weathercount < 0 ) return;
	}	
	else
	{	
		mon			= 6, day = 10;	
		today_weathercount		= (rand()%5) +1;	// 며몇번으로 나누어 뿌릴것인가?
		rain_amount = RainTable[mon][day] / today_weathercount;	// 오늘 내려야할 양에서 몇먼으로 나누어 뿌릴지 양을 계산한다. 
		today_temperature	= (Temprature[mon][day][0] + Temprature[mon][day][1])/2;	// 오늘의 기온은...
		
		for( i = 0 ; i < 5 ; i ++)
		{	
			today_rainstart[i]	= 0;
			today_rainend[i]	= 0;
			today_rainamount[i] = 0;
		}
		
		if( rain_amount )
		for( i = 0 ; i < today_weathercount ; i ++)
		{
			today_rainstart[i]  =  (3600 * 24 / time) * i + ( rand()% (3600 * 24 / time - 3600) );
			today_rainend[i]    = today_rainstart[i] + 60 * (rand()%10 + 20 );
			today_rainamount[i] = rain_amount;
		}
		today_weathercount--;
	}	
}

void SendWeatherRoutine(t_connection c[])
{	//< CSD-CN-031213
	static DWORD weathertime;

	if (today_weathercount < 0) 
	{
		today_weathercount = 1;
	}	

	if (global_time - weathertime > 10000)
	{
		weathertime = global_time;
		
		t_packet packet;
		packet.h.header.type = CMD_WEATHER;
		packet.h.header.size = sizeof(t_server_weather);
		packet.u.server_weather.weather = weathertime;
		packet.u.server_weather.another = g_curr_time;
		packet.u.server_weather.rainstart = today_rainstart[today_weathercount];
		packet.u.server_weather.rainend = today_rainend[today_weathercount];
		packet.u.server_weather.amount = today_rainamount[today_weathercount];
		packet.u.server_weather.temperature = getWeatherCorrection();
		g_pUserManager->SendPacket(&packet);
	}
}	//> CSD-CN-031213

short int getWeatherCorrection( void )
{//021230 lsw
	short int time = 0;
	time = (g_mon+1) <<10;
	time |= (g_day) <<5;
	time |= g_hour+1;

	return time;
}

//////////////////////////////////////////////////
//												//
//	Random Weather System by Eleval 11/08/09	//
//	By checking the date, the map and several	//
//	other values, the system will set different //
//	kinds of weather such as Good Weather, Rain //
//	Weather, Thunderstorm and Snowy Weather		//
//												//
//////////////////////////////////////////////////
//Constructor
CRanWeatherSystem::CRanWeatherSystem()
{
	m_wait_time = 0;
	m_current_weather = WK_GOOD;
	m_raining_chances = 5;
	m_snowing_chances = 3;
	m_thunder_chances = 3;
	m_thunder_time = 0;
	m_force = 100;

	m_last_time = g_curr_time;
}
//

void CRanWeatherSystem::ChangeWeather(int nKind)
{
	m_current_weather = nKind;

	switch(nKind)
	{
		case WK_GOOD:
			m_raining_chances +=5;
			m_snowing_chances +=5;
			m_thunder_chances += 5;
			m_force = 0;
			MyLog(0, "Good Weather started in map");
			break;
		case WK_RAIN:
			m_force = m_raining_chances * 20;
			m_raining_chances -= 30;
			m_snowing_chances += 1;
			m_thunder_chances -= 15;
			MyLog(0, "Rainy Weather started in map with force: %d", m_force);
			break;
		case WK_THUNDER:
			m_force = m_thunder_chances * 40;
			m_raining_chances -= 30;
			m_snowing_chances += 1;
			m_thunder_chances -= 20;
			MyLog(0, "Thunder Weather started in map with force: %d", m_force);
			break;
		case WK_SNOW:
			m_current_weather = WK_GOOD; //To remove temp the snow weather
			m_force = m_snowing_chances * 20;
			m_raining_chances += 1;
			m_snowing_chances -= 30;
			m_thunder_chances += 1;
			MyLog(0, "Snowy Weather started in map with force: %d", m_force);
			break;
	}
}

void CRanWeatherSystem::ChangeForce(int nForce)
{
	m_force = nForce;
}
bool CRanWeatherSystem::IsChangeable()
{
	if(m_wait_time <= 0)
		return true;
	else if (m_wait_time > 0)
		return false;

	return true;
}
void CRanWeatherSystem::Proc()
{
	UpdateTime();

	if(!IsChangeable())
		return;

	int current_chance = rand()%100;

	int map_kind = GetMapKind();
	int date_kind = GetDateKind();


	switch(date_kind)
	{
		case WDK_SPRING:
			m_raining_chances += 5;
			m_thunder_chances += 2;
			m_snowing_chances = 5;
			break;
		case WDK_SUMMER:
			m_raining_chances += 5;
			m_thunder_chances += 10;
			m_snowing_chances = 1;
				break;
		case WDK_FALL:
			m_raining_chances += 12;
			m_thunder_chances += 5;
			m_snowing_chances += 2;
			break;
		case WDK_WINTER:
			m_raining_chances += 3;
			m_thunder_chances = 1;
			m_snowing_chances += 15;
			break;
	}

	switch(map_kind)
	{
		case WMK_NORMAL:
			break;
		case WMK_RAINY:
			m_raining_chances += 5;
			m_thunder_chances += 2;
			break;
		case WMK_SNOWY:
			m_raining_chances += 1;
			m_snowing_chances += 5;
			break;
		case WMK_DESERT:
			m_raining_chances -= 5;
			m_thunder_chances -= 2;
			m_snowing_chances -= 5;
			break;
		case WMK_DUNGEON:
			m_raining_chances = 0;
			m_thunder_chances = 0;
			m_snowing_chances = 0;
			break;
	}
	
	if(current_chance >= 25 && rand()%100 < m_raining_chances)
		ChangeWeather(WK_RAIN);
	else if(current_chance >= 50 && rand()%100 < m_snowing_chances)
		ChangeWeather(WK_SNOW);
	else if(current_chance >= 75 && rand()%100 < m_thunder_chances)
		ChangeWeather(WK_THUNDER);
	else
		ChangeWeather(WK_GOOD);

	StartNewWeather();
	SetRandomTime();
}

int CRanWeatherSystem::GetDateKind()
{
	struct tm *now;
	time_t nowTime;

	int mon = 0;

	time( &nowTime );
	now = localtime(&nowTime);

	// Make it usable.
	mon  = now->tm_mon + 1;

	if(mon >= 3 && mon <= 5)
		return WDK_SPRING;
	if(mon >= 6 && mon <= 8)
		return WDK_SUMMER;
	if(mon >= 9 && mon <= 11)
		return WDK_FALL;

	return WDK_WINTER;
}

int CRanWeatherSystem::GetMapKind()
{
	switch(MapNumber)
	{
	case 2:
	case 3:
	case 5:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 20:
	case 21:
	case 24:
	case 28:
	case 29:
	case 37:
	case 38:
	case 39:
	case 40:
	case 45:
	case 59:
	case 60:
	case 73:
	case 74:
	case 75:
	case 76:
	case 77:
	case 78:
	case 90:
	case 91:
	case 94:
	case 95:
	case 96:
		return WMK_DUNGEON;
		break;

	case 4:
	case 17:
	case 33:
	case 55:
	case 70:
	case 83:
	case 84:
		return WMK_RAINY;
		break;

	case 86:
	case 87:
	case 88:
	case 89:
		return WMK_SNOWY;
		break;

	case 23:
	case 36:
	case 41:
	case 42:
		return WMK_DESERT;
		break;

	default:
		return WMK_NORMAL;
		break;
	}
}

void CRanWeatherSystem::StartNewWeather()
{
	t_packet packet;
	packet.h.header.type = CMD_RM_CHANGE_WEATHER;
	packet.h.header.size = sizeof(t_rm_change_weather);
	packet.u.RmChangeWeather.bWeather = m_current_weather;
	packet.u.RmChangeWeather.dwAmount = m_force;
	packet.u.RmChangeWeather.bStopWeather = 0;
			
	for (int cn = DRAGON_CONNECTIONS_START ; cn < DRAGON_MAX_CONNECTIONS ; cn ++)
	{
		if(connections[cn].dwAgentConnectionIndex && connections[cn].state >= CONNECT_JOIN)
		{
			QueuePacket(connections, cn, &packet, 1);
		}
	}
}

void CRanWeatherSystem::UpdateTime()
{
	if(m_wait_time > 0)
	{
		m_wait_time -= g_curr_time - m_last_time;
	}
	else
	{
		m_wait_time = 0;
	}

	m_last_time = g_curr_time;
}

void CRanWeatherSystem::SetRandomTime()
{
	m_wait_time = (30 + rand()%120) * 60 + m_force;
}

void CRanWeatherSystem::SendWeather(short int cn)
{
	t_packet packet;
	packet.h.header.type = CMD_RM_CHANGE_WEATHER;
	packet.h.header.size = sizeof(t_rm_change_weather);
	packet.u.RmChangeWeather.bWeather = m_current_weather;
	packet.u.RmChangeWeather.dwAmount = m_force;
	packet.u.RmChangeWeather.bStopWeather = 0;

	QueuePacket(connections, cn, &packet, 1);
}

int CRanWeatherSystem::SetRandomWeatherTypeOfTheDay()
{
	int w_kind;
	int ran_type = 0;

		switch(GetDateKind())
		{
		case WDK_SPRING:
			ran_type = rand()%10;
			if(ran_type <= 5)
			{
				w_kind = WK_GOOD;
				m_raining_chances -= 7;
				m_snowing_chances -= 7;
				m_thunder_chances -= 7;
			}
			else if(ran_type >= 6 && ran_type <= 7)
			{
				w_kind = WK_RAIN;
				m_raining_chances += 5;
				m_snowing_chances -= 5;
				m_thunder_chances -= 5;
			}
			else if(ran_type == 8)
			{
				w_kind = WK_SNOW;
				m_raining_chances -= 5;
				m_snowing_chances += 3;
				m_thunder_chances -= 5;
			}
			else if(ran_type == 9)
			{
				w_kind = WK_THUNDER;
				m_raining_chances -= 5;
				m_snowing_chances -= 5;
				m_thunder_chances += 3;
			}
			break;
		case WDK_SUMMER:
			ran_type = rand()%12;
			if(ran_type <= 3)
			{
				w_kind = WK_GOOD;
				m_raining_chances -= 7;
				m_snowing_chances -= 7;
				m_thunder_chances -= 7;
			}
			else if(ran_type >= 4 && ran_type <= 6)
			{
				w_kind = WK_RAIN;
				m_raining_chances += 5;
				m_snowing_chances -= 5;
				m_thunder_chances -= 5;
			}
			else if(ran_type == 11)
			{
				w_kind = WK_SNOW;
				m_raining_chances -= 5;
				m_snowing_chances += 1;
				m_thunder_chances -= 5;
			}
			else if(ran_type >= 7 && ran_type <= 10)
			{
				w_kind = WK_THUNDER;
				m_raining_chances -= 5;
				m_snowing_chances -= 5;
				m_thunder_chances += 7;
			}
			break;
		case WDK_FALL:
			ran_type = rand()%10;
			if(ran_type <= 5)
			{
				w_kind = WK_GOOD;
				m_raining_chances -= 5;
				m_snowing_chances -= 5;
				m_thunder_chances -= 5;
			}
			else if(ran_type >= 6 && ran_type <= 7)
			{
				w_kind = WK_RAIN;
				m_raining_chances += 5;
				m_snowing_chances -= 5;
				m_thunder_chances -= 5;
			}
			else if(ran_type == 8)
			{
				w_kind = WK_SNOW;
				m_raining_chances -= 5;
				m_snowing_chances += 3;
				m_thunder_chances -= 5;
			}
			else if(ran_type == 9)
			{
				w_kind = WK_THUNDER;
				m_raining_chances -= 5;
				m_snowing_chances -= 5;
				m_thunder_chances += 3;
			}
			break;
		case WDK_WINTER:
			ran_type = rand()%11;
			if(ran_type <= 4)
			{
				w_kind = WK_GOOD;
				m_raining_chances -= 5;
				m_snowing_chances -= 5;
				m_thunder_chances -= 7;
			}
			else if(ran_type == 5)
			{
				w_kind = WK_RAIN;
				m_raining_chances += 5;
				m_snowing_chances -= 5;
				m_thunder_chances -= 7;
			}
			else if(ran_type >= 6 && ran_type <= 9)
			{
				w_kind = WK_SNOW;
				m_raining_chances -= 5;
				m_snowing_chances += 7;
				m_thunder_chances -= 5;
			}
			else if(ran_type == 10)
			{
				w_kind = WK_THUNDER;
				m_raining_chances -= 5;
				m_snowing_chances -= 4;
				m_thunder_chances += 1;
			}
				break;
		}

	return w_kind;
}
//End of it