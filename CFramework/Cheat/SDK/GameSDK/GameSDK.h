#pragma once
#include "../../../Framework/Utils/Utils.h"

class SDK
{
public:
    // address
    uintptr_t GWorld;
    uintptr_t ReplayInterface;
    uintptr_t ViewPort;
    uintptr_t Camera;

    // pointer
    uintptr_t m_pVehicle = 0xD10;
    uintptr_t m_pInfo = 0x10A8;
    uintptr_t m_pWeaponManager = 0x10B8;

    // offset
    uintptr_t m_pBoneMatrix = 0x60;
    uintptr_t m_vecLocation = 0x90;
    uintptr_t m_bGodMode = 0x189;
    uintptr_t m_fHealth = 0x280;
    uintptr_t m_fHealthMax = 0x284;
    uintptr_t m_vecVelocity = 0x300;
    uintptr_t m_pBoneList = 0x410;
    uintptr_t m_fArmor = 0x150C;
    uintptr_t m_bPedTask = 0x144B;

    // player info
    uintptr_t m_CName = 0xE0;
    uintptr_t m_iWantedLev = 0x8D8;

    // vehicle offset
    uintptr_t m_vecVehicleVelocity = 0;
    uintptr_t m_fVehicleHealth = 0;
    uintptr_t m_fVehicleEngineHealth = 0;

    // weapon
    uintptr_t m_fWeaponSpread = 0;
    uintptr_t m_fWeaponDamage = 0;
    uintptr_t m_fWeaponRecoil = 0x2F4;
    uintptr_t m_fWeaponRange = 0x28C;

    bool InitOffset();
};

enum class ePedTask {
    TASK_NONE,
    TASK_FOOT = 1 << 4,
    TASK_UNK = 1 << 5,
    TASK_DRIVING = 1 << 6
};

enum BoneId : int
{
    HEAD = 0,
    LEFTFOOT = 1,
    RIGHTFOOT = 2,
    LEFTANKLE = 3,
    RIGHTANKLE = 4,
    LEFTHAND = 5,
    RIGHTHAND = 6,
    NECK = 7,
    HIP = 8
};

extern SDK sdk;
extern bool Vec3_Empty(const Vector3& value);
extern Vector3 Vec3_Transform(Vector3* vIn, Matrix* mIn);
extern Vector3 CalcAngle(Vector3 local_cam, Vector3 to_point);
extern float GetSpeed(Vector3 value);
extern float GetDistance(Vector3 value1, Vector3 value2);
extern bool WorldToScreen(Matrix ViewMatrix, Vector3& vWorld, Vector2& vOut);