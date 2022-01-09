// AttackMagic.h: interface for the CAttackMagic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATTACKMAGIC_H__C035ACD8_EEC0_4957_A052_618A22B01275__INCLUDED_)
#define AFX_ATTACKMAGIC_H__C035ACD8_EEC0_4957_A052_618A22B01275__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Battle.h"
///////////////////////////////////////////////////////////////////////////////
// 공격계열 마법
class CAttackMagic : public TBinder<CMagic, CAttackMagic>
{ 
public:
	CAttackMagic();
	virtual ~CAttackMagic();
	
public:
	virtual bool Bind();
	
public:
	virtual bool Execute() 
	{ 
		return (!IsExist(m_nIndex)) ? false:(*this.*m_tblHash[m_nIndex])(); 
	}
	
	virtual bool Elapse(CHARLIST* pTarget) 
	{ 
		return true; 
	}
	
public:
	bool FireArrow();            // 불계열의 공격마법
	bool FireWall();
	bool FireBall();
	bool Scorcher();
	bool CircleFlame();
	bool FlameWave();
	bool Apocalypse();
	bool Pheonix();
	bool MeteorStrike();
	bool GreatMeteor();
	bool FlyingSpark();
	bool FireExplosion();
	bool FireStorm(); 
	bool FireBreath();
	bool FlamePillar();
	bool FlamePour();
	bool IceArrow();             // 얼음계열의 공격마법
	bool IceWall();
	bool IceBall();
	bool Freeze5TileRadius();
	bool CircleIce();
	bool IceStorm();
	bool Freeze9TileRadius();
	bool Freezing();
	bool IceBreath();
	bool LightningBolt();        // 전격계열의 공격마법 
	bool ChainLightning();
	bool CircleLightning();
	bool Lightning();
	bool GreatThunderBolt();
	bool Lightning5TileRadius();
	bool BlitzSpear();
	bool LightningCloud();
	bool LightningBreath();
	bool OctaLightning();
	bool DeathFire();            // 신력계열의 공격마법
	bool HolyFire();
	bool HolyLight();
	bool HolyRage();
	bool TurnUndead();
	bool DivinePower();
	bool AuthorityCharisNumen(); // CSD-021015
	bool RageStone();
	bool RageBlast();
	bool RageLightBall();
	bool ShadowOfEvil();         // 언데드계열의 공격마법
	bool BooldSprinkle();
	bool FlameArrow(); // CSD-031020
	bool AreaBatAttack();

	bool SnowStorm();	// 030415 kyo
	bool IceBreath2();	// 030415 kyo
	bool BloodLightning();	// 030415 kyo
	bool AuthorityOfWhoarencha(); // 030415 kyo

protected:
	void SetFrozen(int nPeriod);
	bool AttackMagic(int nDamage);
	
private:
	void Correct(int& rDamage);
	bool Result(int nHD);
	
private:
	bool IsMiss() const
	{ // 마법 성공률에 따른 마법 실패 여부 검사
		if( m_pTarget && m_pTarget->IsDead() ) return true;		// 020818 YGI
		const int nDefense = RareEM.GetStaticRareEvasion(m_pTarget->StaticRare);
		const int nRate= m_pCaster->CalcMagicSuccessRate(nDefense);
		return (nRate < rand()%101) ? true:false;
	}
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_ATTACKMAGIC_H__C035ACD8_EEC0_4957_A052_618A22B01275__INCLUDED_)
