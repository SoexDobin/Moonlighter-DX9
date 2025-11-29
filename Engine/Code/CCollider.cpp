#include "CCollider.h"
#include "CGameObject.h"

CCollider::CCollider()
    : CComponent(), m_bIsCol(false), m_eState(COL_STATE_END), m_fScale(1.f)
{
    ZeroMemory(&m_tCollision, sizeof(Collision));
    ZeroMemory(&m_vOffset, sizeof(_vec3));
}

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
    : CComponent(pGraphicDev), m_bIsCol(false), m_eState(COL_STATE_END), m_fScale(1.f)
{
    ZeroMemory(&m_tCollision, sizeof(Collision));
    ZeroMemory(&m_vOffset, sizeof(_vec3));
}

CCollider::CCollider(const CCollider& rhs)
    : CComponent(rhs), m_bIsCol(false), m_eState(COL_STATE_END),
    m_vOffset(rhs.m_vOffset), m_fScale(rhs.m_fScale)
{
    ZeroMemory(&m_tCollision, sizeof(Collision));
}

CCollider::~CCollider()
{
}

_bool CCollider::Is_Overlapped(CCollider* pOverlap)
{
     return (m_usetOverlapCol.find(pOverlap) != m_usetOverlapCol.end());
}

void CCollider::Add_OverlapMember(CCollider* pOverlap)
{
    if (m_usetOverlapCol.find(pOverlap) == m_usetOverlapCol.end())
    {
        m_usetOverlapCol.insert(pOverlap);
        pOverlap->AddRef();
    }
}

void CCollider::Release_OverlapMember(CCollider* pOverlap)
{
    if (m_usetOverlapCol.find(pOverlap) != m_usetOverlapCol.end())
    {
        m_usetOverlapCol.erase(pOverlap);
        Safe_Release(pOverlap);
    }
}

void CCollider::Free()
{
    for_each(m_usetOverlapCol.begin(), m_usetOverlapCol.end(),
        [](CCollider* pCol) -> void {
            Safe_Release(pCol);
        });

    CComponent::Free();
}

#pragma region Editor
#ifdef _DEBUG

void CCollider::Display_Editor(const char* pObjTag)
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(15, 15, 45, 255));
    const _float lineH = ImGui::GetFrameHeightWithSpacing();

    ImGui::Text("Scale");
    if (ImGui::BeginChild("ScaleBox", ImVec2(0, lineH * 3.f), true))
    {
        ImGui::PushItemWidth(100.0f);
        ImGui::SliderFloat("##Scale", &m_fScale, 1.0f, 20.0f, "%.2f");
        ImGui::PopItemWidth();
    }
    ImGui::EndChild();

    ImGui::Spacing();

    ImGui::Text("Offset");
    if (ImGui::BeginChild("OffsetBox", ImVec2(0, lineH * 2.0f), true))
    {
        ImGui::PushItemWidth(40.0f);

        ImGui::InputFloat("X", &m_vOffset.x, 0.f, 0.f, "%.2f"); ImGui::SameLine();
        ImGui::InputFloat("Y", &m_vOffset.y, 0.f, 0.f, "%.2f"); ImGui::SameLine();
        ImGui::InputFloat("Z", &m_vOffset.z, 0.f, 0.f, "%.2f");

        ImGui::PopItemWidth();
    }
    ImGui::EndChild();

    ImGui::Spacing();

    ImGui::Text("Collider");
    // 타입 표시
    const char* typeStr = "";
    switch (Get_ColType())
    {
    case SPHERE_COL: typeStr = "Sphere"; break;
    case RECT_COL:   typeStr = "Rect";   break;
    default:         typeStr = "Unknown"; break;
    }

    ImGui::Text("Type       : %s", typeStr);
    ImGui::Text("Owner      : %p", m_pOwner);

    // 오프셋 / 스케일 편집 (원하면)
    ImGui::Text("Offset");
    ImGui::SameLine();
    ImGui::PushItemWidth(40);
    ImGui::InputFloat("o X", &m_vOffset.x, 0.f, 0.f, "%.2f"); ImGui::SameLine();
    ImGui::InputFloat("o Y", &m_vOffset.y, 0.f, 0.f, "%.2f"); ImGui::SameLine();
    ImGui::InputFloat("o Z", &m_vOffset.z, 0.f, 0.f, "%.2f");
    ImGui::PopItemWidth();

    ImGui::Text("Scale");
    ImGui::SameLine();
    ImGui::PushItemWidth(60);
    ImGui::InputFloat("##ColScale", &m_fScale, 0.f, 0.f, "%.2f");
    ImGui::PopItemWidth();

    const auto& overlap = m_usetOverlapCol;
    _int overlapCount = static_cast<_int>(overlap.size());
    ImGui::Text("OverlapCnt : %d", overlapCount);

    // ✅ 체크박스로 바로 on/off
    ImGui::Checkbox("Show Overlaps", &m_bShowOverlapsInEditor);

    // 체크된 경우에만 목록을 그려줌 (에디터 인라인)
    if (m_bShowOverlapsInEditor)
    {
        if (overlapCount > 0)
        {
            if (ImGui::TreeNode("Overlaps"))
            {
                _int idx = 0;
                for (CCollider* pOther : overlap)
                {
                    CGameObject* pOtherOwner = pOther ? pOther->Get_Owner() : nullptr;
                    const char* otherName =
                        pOtherOwner ? (const char*)pOtherOwner->Get_DisplayName() : "Unknown";

                    ImGui::Text("[%d] %s (Col Object :%s)",
                        idx++, otherName, pOtherOwner->Get_DisplayName());
                }
                ImGui::TreePop();
            }
        }
        else
        {
            ImGui::TextDisabled("No overlaps");
        }
    }
    ImGui::PopStyleColor();
}

#endif
#pragma endregion
