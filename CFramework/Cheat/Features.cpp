#include "FrameCore.h"

int bone = 0; 
const int ReadCount = 256;
Vector3 GetPrediction(CPed& target, CPed& local);

bool CFramework::Init()
{
    std::thread([&]() { this->UpdateList(); }).detach();

    return sdk.InitOffset();
}

bool CFramework::AimBot(CPed& target)
{
    /*----| SomeChecks |--------------------------------------------------------------------------------*/
   // 1st
    if (g.AimKey0 == NULL || !IsKeyDown(g.AimKey0) && !IsKeyDown(g.AimKey1) || g.ShowMenu)
        return false;

    // 2nd
    switch (g.AimKeyType)
    {
    case 0: // and
        if (g.AimKey1 == NULL && IsKeyDown(g.AimKey0))
            break;
        else if (!IsKeyDown(g.AimKey0) || !IsKeyDown(g.AimKey1))
            return false;
        else if (!IsKeyDown(g.AimKey0))
            return false;
        break;
    case 1: // or
        if (g.AimKey1 == NULL && IsKeyDown(g.AimKey0))
            break;
        else if (IsKeyDown(g.AimKey0) || IsKeyDown(g.AimKey1))
            break;

        break;
    }
    /*--------------------------------------------------------------------------------------------------*/

    uintptr_t camera = m.Read<uintptr_t>(m.m_gProcessBaseAddr + sdk.Camera);
    Vector3 ViewAngle = m.Read<Vector3>(camera + 0x3D0); // 0x40
    Vector3 CameraPos = m.Read<Vector3>(camera + 0x60);

    switch (g.Aim_Bone)
    {
    case 0: bone = 0; break;
    case 1: bone = 7; break;
    case 2: bone = 8; break;
    }

    std::vector<Vector3> BoneList = target.GetBoneList();
    Vector3 TargetPos = g.Aim_Prediction ? BoneList[bone] + GetPrediction(target, local) : BoneList[bone];
    Vector3 Angle = CalcAngle(CameraPos, TargetPos);
    Vector3 Delta = Angle - ViewAngle;
    Vector3 WriteAngle = ViewAngle + (Delta / g.Aim_Smooth);

    if (!Vec3_Empty(WriteAngle))
    {
        m.Write<Vector3>(camera + 0x40, WriteAngle);
        m.Write<Vector3>(camera + 0x3D0, WriteAngle);
    }

    target = CPed();
}

void CFramework::MiscAll()
{
    // GodMode
    if (g.GodMode && !local.IsGod())
        m.Write<bool>(local.address + sdk.m_bGodMode, true);
    else if (!g.GodMode && local.IsGod())
        m.Write<bool>(local.address + sdk.m_bGodMode, false);

    // HealMe
    if (g.HealMe && local.m_fHealth <= local.m_fMaxHealth)
        m.Write<float>(local.address + sdk.m_fHealth, local.m_fHealth + 0.2f);

    // NoRecoil
    if (g.NoRecoil && m.Read<float>(local.CurrentWeapon + sdk.m_fWeaponRecoil) > 0.f)
        m.Write<float>(local.CurrentWeapon + sdk.m_fWeaponRecoil, 0.f);

    // NoSpread
    if (g.NoSpread && m.Read<float>(local.CurrentWeapon + sdk.m_fWeaponSpread) > 0.f)
        m.Write<float>(local.CurrentWeapon + sdk.m_fWeaponSpread, 0.f);
}

struct Entity {
    uint64_t address;
    uint64_t junk0;
};

struct EntityList_t {
    Entity entity[ReadCount];
};

void CFramework::UpdateList()
{
    while (g.Run)
    {
        std::vector<CPed> temp_pedlist;

        GameWorld = m.Read<uintptr_t>(m.m_gProcessBaseAddr + sdk.GWorld);
        local.address = m.Read<uintptr_t>(GameWorld + 0x8);
        ViewPort = m.Read<uintptr_t>(m.m_gProcessBaseAddr + sdk.ViewPort);

        uintptr_t ReplayInterface = m.Read<uintptr_t>(m.m_gProcessBaseAddr + sdk.ReplayInterface);
        uintptr_t EntityListPtr = m.Read<uintptr_t>(ReplayInterface + 0x18);
        uintptr_t entitylist_addr = m.Read<uintptr_t>(EntityListPtr + 0x100);

        EntityList_t base = m.Read<EntityList_t>(entitylist_addr), *list = &base;

        for (int i = 0; i < ReadCount; i++)
        {
            if (list->entity[i].address != NULL && list->entity[i].address != local.address)
            {
                CPed p = CPed();

                p.address = list->entity[i].address;

                if (p.Update()) {
                    p.Index = i;
                    temp_pedlist.push_back(p);
                }
            }
        }

        this->EntityList = temp_pedlist;
        temp_pedlist.clear();

        Sleep(500);
    }
}

Vector3 GetPrediction(CPed& target, CPed& local)
{
    Vector3 vOut{};
    Vector3 m_pVecVelocity = target.GetVelocity();

    if (Vec3_Empty(m_pVecVelocity))
        return Vector3();

    float distance = GetDistance(target.m_pVecLocation, local.m_pVecLocation);
    float BulletSpeed = m.Read<float>(local.CurrentWeapon + 0xE0);

    if (BulletSpeed == 0.f)
        return Vector3();
    else if (BulletSpeed < 100.f)
        BulletSpeed = 1800.f; // 仮

    // 仮。もっといい感じの案を出すべきだ。
    float time = distance / g.Aim_Predict;
    vOut = m_pVecVelocity * time;
    
    return vOut;
}