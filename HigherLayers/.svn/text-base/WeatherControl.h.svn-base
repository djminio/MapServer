// WheatherControl.h: interface for the CWheatherControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WHEATHERCONTROL_H__164EB5A2_DF0A_4DE3_8F1B_12320DBB32AA__INCLUDED_)
#define AFX_WHEATHERCONTROL_H__164EB5A2_DF0A_4DE3_8F1B_12320DBB32AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const short int nMinimumLight = 9;
const short int nMaximumLight = 31;

class CWeatherControl  
{
public:
	CWeatherControl();
	virtual ~CWeatherControl();
public:
	DWORD dwCheckedTime;
	void CheckDayLight();
	void AutoSetDayLight(CHARLIST *ch);
	void SetDayLight(const short int nValue);
	void SetChLight(CHARLIST *ch,const int iLight,const int iTimeSec );
private:
	short int nDayLightControl;
};

//////////////////////////////////////////////////
//												//
//	Random Weather System by Eleval 11/08/09	//
//	By checking the date, the map and several	//
//	other values, the system will set different //
//	kinds of weather such as Good Weather, Rain //
//	Weather, Thunderstorm and Snowy Weather		//
//												//
//////////////////////////////////////////////////
enum WeatherKind{
	WK_GOOD = 0,
	WK_RAIN = 1,
	WK_SNOW = 2,
	WK_THUNDER = 3
};

enum WeatherMapKind{
	WMK_NORMAL,
	WMK_RAINY,
	WMK_SNOWY,
	WMK_DESERT,
	WMK_DUNGEON
};

enum WeatherDateKind{
	WDK_SPRING,
	WDK_SUMMER,
	WDK_FALL,
	WDK_WINTER
};

class CRanWeatherSystem
{
public:
	CRanWeatherSystem();
	//~CRanWeatherSystem();

	void ChangeWeather(int nKind);
	void ChangeForce(int nForce);
	bool IsChangeable();
	void Proc();
	int GetMapKind();
	int GetDateKind();
	void StartNewWeather();
	void UpdateTime();
	void SetRandomTime();
	void SendWeather(short int cn);
	int SetRandomWeatherTypeOfTheDay();

private:

	int m_wait_time;
	DWORD m_last_time;
	int m_current_weather;
	int m_raining_chances;
	int m_snowing_chances;
	int m_thunder_chances;
		int m_thunder_time;
	int m_force;
};

//End of it

extern CWeatherControl WeatherControl;
extern CRanWeatherSystem RandomWeatherSystem;

extern void CurrentGameDate( const DWORD t, int *y, int *mo, int *d, int *h, int *mi, int *sec );
extern int  LoadWeatherTable( void );
extern void CheckWeatherSystem( void );
extern short int getWeatherCorrection( void );

extern DWORD today_rainstart[5];
extern DWORD today_rainend[5];
extern int   today_rainamount[5];
extern int   today_temperature;
extern int	  today_weathercount;

#endif // !defined(AFX_WHEATHERCONTROL_H__164EB5A2_DF0A_4DE3_8F1B_12320DBB32AA__INCLUDED_)
