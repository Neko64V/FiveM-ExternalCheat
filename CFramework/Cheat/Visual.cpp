#include "FrameCore.h"

void CFramework::RenderInfo()
{
    // ウォーターマーク
    std::string text = std::to_string((int)ImGui::GetIO().Framerate) + "FPS";
    StringEx(ImVec2(8.f, 8.f), ImColor(1.f, 1.f, 1.f, 1.f), ImGui::GetFontSize(), text.c_str());

    ImGui::GetStyle().AntiAliasedLines = false;

    // FovCircle
    if (g.AimBot && g.Aim_DrawFov)
        ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(g.GameRect.right / 2.f, g.GameRect.bottom / 2.f), g.Aim_Fov, FOV_User);

    // Crosshair
    if (g.Crosshair)
    {
        switch (g.CrosshairType)
        {
        case 0: {
            ImVec2 Center = ImVec2(g.GameRect.right / 2, g.GameRect.bottom / 2);
            ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Center.x - g.CrosshairSize, Center.y), ImVec2((Center.x + g.CrosshairSize) + 1, Center.y), CrosshairColor, 1);
            ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Center.x, Center.y - g.CrosshairSize), ImVec2(Center.x, (Center.y + g.CrosshairSize) + 1), CrosshairColor, 1);
        }   break;
        case 1:
            ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2((float)g.GameRect.right / 2.f, (float)g.GameRect.bottom / 2.f), g.CrosshairSize + 1, ImColor(0.f, 0.f, 0.f, 1.f), NULL);
            ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2((float)g.GameRect.right / 2.f, (float)g.GameRect.bottom / 2.f), g.CrosshairSize, CrosshairColor, NULL);
            break;
        }
    }

    ImGui::GetStyle().AntiAliasedLines = true;
}

void CFramework::RenderESP()
{
    CPed* pLocal = &local;
    static CPed target = CPed();

    float MinFov = 9999.f;
    float MinDistance = 9999.f;
    Vector2 Center = Vector2(g.GameRect.right / 2.f, g.GameRect.bottom / 2.f);
    Matrix ViewMatrix = m.Read<Matrix>(ViewPort + 0x24C);

    if (!pLocal->Update())
        return;

    for (auto& entity : EntityList)
    {
        CPed* pEntity = &entity;

        // Checks
        if (!pEntity->Update())
            continue;

        float pDistance = GetDistance(pEntity->m_pVecLocation, pLocal->m_pVecLocation);

        if (pDistance > g.ESP_MaxDistance)
            continue;
        else if (!g.ESP_NPC && !pEntity->IsPlayer())
            continue;

        // Bone W2S
        Vector2 pBase{};
        if (!WorldToScreen(ViewMatrix, pEntity->m_pVecLocation, pBase))
            continue;

        std::vector<Vector2> bScreen;
        std::vector<Vector3> BoneList = pEntity->GetBoneList();

        for (int j = 0; j < BoneList.size(); j++)
        {
            Vector2 vOut{};

            if (Vec3_Empty(BoneList[j]))
                continue;
            else if (!WorldToScreen(ViewMatrix, BoneList[j], vOut))
                continue;

            bScreen.push_back(vOut);
        }

        if (BoneList.size() != bScreen.size())
            continue;

        // ESP Resource
        float HeadToNeck = bScreen[NECK].y - bScreen[HEAD].y;
        float pTop = bScreen[HEAD].y - (HeadToNeck * 2.5f);
        float pBottom = bScreen[LEFTFOOT].y > bScreen[RIGHTFOOT].y ? bScreen[LEFTFOOT].y : bScreen[RIGHTFOOT].y;
        float pHeight = pBottom - pTop;
        float pWidth = pHeight / 3.5f;
        float bScale = pWidth / 1.5f;
        ImColor color = pEntity->IsPlayer() ? ESP_PLAYER : ESP_NPC;

        // GodMode Checker
        if (pEntity->IsGod())
            color = ESP_GOD;

            // Line
        if (g.ESP_Line)
            DrawLine(ImVec2(g.GameRect.right / 2.f, g.GameRect.bottom), ImVec2(pBase.x, pBottom), color, 1.f);

        // Box
        if (g.ESP_Box)
        {
            if (g.ESP_BoxFilled)
                ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(pBase.x - pWidth, pTop), ImVec2(pBase.x + pWidth, pBottom), ESP_Filled);

            switch (g.ESP_BoxType)
            {
            case 0: // simple
                DrawLine(ImVec2(pBase.x - pWidth, pTop), ImVec2(pBase.x + pWidth, pTop), color, 1.f);
                DrawLine(ImVec2(pBase.x - pWidth, pTop), ImVec2(pBase.x - pWidth, pBottom), color, 1.f);
                DrawLine(ImVec2(pBase.x + pWidth, pTop), ImVec2(pBase.x + pWidth, pBottom), color, 1.f);
                DrawLine(ImVec2(pBase.x - pWidth, pBottom), ImVec2(pBase.x + pWidth, pBottom), color, 1.f);
                break;
            case 1: // cornerd
                DrawLine(ImVec2((pBase.x - pWidth), pTop), ImVec2((pBase.x - pWidth) + bScale, pTop), color, 1.f); // top
                DrawLine(ImVec2((pBase.x + pWidth), pTop), ImVec2((pBase.x + pWidth) - bScale, pTop), color, 1.f);
                DrawLine(ImVec2(pBase.x - pWidth, pTop), ImVec2(pBase.x - pWidth, pTop + bScale), color, 1.f); // left
                DrawLine(ImVec2(pBase.x - pWidth, pBottom), ImVec2(pBase.x - pWidth, pBottom - bScale), color, 1.f);
                DrawLine(ImVec2(pBase.x + pWidth, pTop), ImVec2(pBase.x + pWidth, pTop + bScale), color, 1.f); // right
                DrawLine(ImVec2(pBase.x + pWidth, pBottom), ImVec2(pBase.x + pWidth, pBottom - bScale), color, 1.f);
                DrawLine(ImVec2((pBase.x - pWidth), pBottom), ImVec2((pBase.x - pWidth) + bScale, pBottom), color, 1.f); // bottom
                DrawLine(ImVec2((pBase.x + pWidth), pBottom), ImVec2((pBase.x + pWidth) - bScale, pBottom), color, 1.f);
                break;
            default:
                break;
            }
        }

        // Skeleton
        if (g.ESP_Skeleton)
        {
            // Head
            Circle(ImVec2(bScreen[HEAD].x, bScreen[HEAD].y), HeadToNeck, ESP_Skeleton);

            Vector3 bList[][2] = { { BoneList[NECK], BoneList[HIP] }, { BoneList[NECK], BoneList[LEFTHAND] }, { BoneList[NECK], BoneList[RIGHTHAND] }, 
                { BoneList[HIP], BoneList[LEFTANKLE] }, { BoneList[HIP], BoneList[RIGHTANKLE] } };

            // Body
            for (int j = 0; j < 5; j++)
            {
                Vector2 ScreenB1{}, ScreenB2{};
                if (Vec3_Empty(bList[j][0]) || Vec3_Empty(bList[j][1]))
                    break;

                if (!WorldToScreen(ViewMatrix, bList[j][0], ScreenB1) || !WorldToScreen(ViewMatrix, bList[j][1], ScreenB2))
                    break;

                DrawLine(ImVec2(ScreenB1.x, ScreenB1.y), ImVec2(ScreenB2.x, ScreenB2.y), ESP_Skeleton, 1.f);
            }
        }

        // Healthbar
        if (g.ESP_HealthBar)
        {
            HealthBar((pBase.x - pWidth) - 5.f, pBottom, 2.f, -pHeight, pEntity->m_fHealth, pEntity->m_fMaxHealth);

            if (pEntity->m_fArmor > 0.f)
                ArmorBar((pBase.x + pWidth) + 3.f, pBottom, 2.f, -pHeight, pEntity->m_fArmor, 100);
        }

        // Distance
        if (g.ESP_Distance)
        {
            std::string DistStr = std::to_string((int)pDistance) + "m";
            StringEx(ImVec2(pBase.x - ImGui::CalcTextSize(DistStr.c_str()).x / 2.f, pBottom), ImColor(1.f, 1.f, 1.f, 1.f), ImGui::GetFontSize(), DistStr.c_str());
        }

        // Name
        if (g.ESP_Name)
        {
            /* - GTA Online
            std::string pName = m.ReadString(pEntity->PlayerInfo + 0xFC);
            StringEx(ImVec2(pBase.x - ImGui::CalcTextSize(pName.c_str()).x / 2.f, pTop - 15.f), ImColor(1.f, 1.f, 1.f, 1.f), ImGui::GetFontSize(), pName.c_str());
            */
        }

        // AimBot
        if (g.AimBot)
        {
            if (IsKeyDown(g.AimKey0) || IsKeyDown(g.AimKey1))
            {
                if (g.Aim_MaxDistance < pDistance)
                    continue;
                else if (!g.Aim_NPC && !pEntity->IsPlayer())
                    continue;

                for (int j = 0; j < 9; j++)
                {
                    Vector2 fov_check{};
                    if (!WorldToScreen(ViewMatrix, BoneList[j], fov_check))
                        continue;

                    float FOV = abs((Center - fov_check).Length());

                    if (FOV < g.Aim_Fov)
                    {
                        switch (g.Aim_Type)
                        {
                        case 0:
                            if (FOV < MinFov)
                            {
                                target = entity;
                                MinFov = FOV;

                                break;
                            }
                            break;
                        case 1:
                            if (pDistance < MinDistance)
                            {
                                target = entity;
                                MinDistance = pDistance;

                                break;
                            }
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
        }
    }

    // AimBot
    if (g.AimBot)
    {
        if (target.address != NULL)
            AimBot(target);
    }
}