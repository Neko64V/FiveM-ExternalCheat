#include "CPed.h"

bool CPed::GetPlayer(uintptr_t& address)
{
	address = address;
	return address == NULL ? false : true;
}

bool CPed::Update()
{
    Vehicle = m.Read<uintptr_t>(address + sdk.m_pVehicle);
    PlayerInfo = m.Read<uintptr_t>(address + sdk.m_pInfo);
    uintptr_t weapon = m.Read<uintptr_t>(address + sdk.m_pWeaponManager);
    CurrentWeapon = m.Read<uintptr_t>(weapon + 0x20);

    // PedData
    m_fHealth = m.Read<float>(address + sdk.m_fHealth);
    m_pVecLocation = m.Read<Vector3>(address + sdk.m_vecLocation);

    if (IsDead())
        return false;

    m_fArmor = m.Read<float>(address + sdk.m_fArmor);
    m_fMaxHealth = m.Read<float>(address + sdk.m_fHealthMax);
    m_bMatrix = m.Read<Matrix>(address + sdk.m_pBoneMatrix);

    return true;
}

bool CPed::IsDead()
{
    return m_fHealth <= 0.f || Vec3_Empty(m_pVecLocation);
}

bool CPed::IsPlayer()
{
    return PlayerInfo != NULL;
}

bool CPed::InVehicle()
{
    return m.Read<uint8_t>(address + sdk.m_bPedTask) & (uint8_t)ePedTask::TASK_DRIVING;
}

bool CPed::IsGod()
{
    return m.Read<bool>(address + sdk.m_bGodMode) == true;
}

struct Bone {
    Vector3 pos{};
    int junk0{};
};

struct AllBone {
    Bone bone[9]{};
};

std::vector<Vector3> CPed::GetBoneList()
{
    std::vector<Vector3> list;
    AllBone b_list = m.Read<AllBone>(address + sdk.m_pBoneList), *BoneList = &b_list;

    for (int b = 0; b < 9; b++) 
    {
        if (Vec3_Empty(BoneList->bone[b].pos))
            list.push_back(Vector3());

        Vector3 pos = BoneList->bone[b].pos;
        list.push_back(Vec3_Transform(&pos, &m_bMatrix));
    }

    return list;
}

Vector3 CPed::GetVelocity()
{
    return InVehicle() ? m.Read<Vector3>(Vehicle + sdk.m_vecVehicleVelocity) : m.Read<Vector3>(address + sdk.m_vecVelocity);
}