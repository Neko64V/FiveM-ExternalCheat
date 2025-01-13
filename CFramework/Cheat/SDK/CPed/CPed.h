#pragma once
#include "../GameSDK/GameSDK.h"

class CPed
{
public:
	// Pointer
    uintptr_t address;
	uintptr_t Vehicle;
	uintptr_t PlayerInfo;
	uintptr_t CurrentWeapon;

	int Index;
	float m_fArmor;
	float m_fHealth;
	float m_fMaxHealth;
	Vector3 m_pVecLocation;
	Vector3 m_pVecVelocity;
	Matrix m_bMatrix;

	bool GetPlayer(uintptr_t& address);
	bool Update();
	bool IsDead();
	bool IsPlayer();
	bool InVehicle();
	bool IsGod();

	Vector3 GetVelocity();
	std::vector<Vector3> GetBoneList();
};