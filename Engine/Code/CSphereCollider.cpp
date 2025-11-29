#include "CSphereCollider.h"
#include "CRectCollider.h"
#include "CCollisionManager.h"

#ifdef _DEBUG
#include "CTransform.h"
#endif

CSphereCollider::CSphereCollider()
    : m_fRadius(1.f)
{
    strcpy_s(m_szDisplayName, "Sphere Collider");
}

CSphereCollider::CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev)
    : CCollider(pGraphicDev), m_fRadius(1.f)
{
    strcpy_s(m_szDisplayName, "Sphere Collider");
}

CSphereCollider::CSphereCollider(const CSphereCollider& rhs)
    : CCollider(rhs), m_fRadius(rhs.m_fRadius)
{
    strcpy_s(m_szDisplayName, "Sphere Collider");
}

CSphereCollider::~CSphereCollider()
{
}

HRESULT CSphereCollider::Ready_SphereCollider()
{
    CCollider::Ready_Collider();
    return S_OK;
}

_bool CSphereCollider::Check_Collision(CCollider* pCol)
{
    switch (pCol->Get_ColType())
    {
    case SPHERE_COL:
        return CCollisionManager::GetInstance()->SphereCollision(this, static_cast<CSphereCollider*>(pCol));
    case RECT_COL:
        return CCollisionManager::GetInstance()->SphereRectCollision(this, static_cast<CRectCollider*>(pCol));
    default:
        return false;
    }
}

_int CSphereCollider::Update_Component(const _float fTimeDelta)
{
    _int iExit = CCollider::Update_Component(fTimeDelta);

    return iExit;
}

void CSphereCollider::LateUpdate_Component()
{
    CCollider::LateUpdate_Component();
}

#ifdef _DEBUG
void CSphereCollider::Render_DebugCollider()
{
    if (m_pTrans == nullptr || m_pGraphicDevice == nullptr)
        return;

    struct DebugVtx { _float x, y, z; DWORD color; };

    
    DWORD prevFillMode = 0;
    m_pGraphicDevice->GetRenderState(D3DRS_FILLMODE, &prevFillMode);
    BOOL prevLighting = FALSE;
    m_pGraphicDevice->GetRenderState(D3DRS_LIGHTING, (DWORD*)&prevLighting);
    D3DXMATRIX prevWorld;
    m_pGraphicDevice->GetTransform(D3DTS_WORLD, &prevWorld);
    // 월드 행렬은 단위행렬로 두고, 월드좌표로 직접 찍는다
    D3DXMATRIX matIdentity;
    D3DXMatrixIdentity(&matIdentity);
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, &matIdentity);

    DWORD color(0);
    if (m_eState == ENTER_COL || m_eState == STAY_COL)
        color = D3DCOLOR_ARGB(255, 255, 0, 0);
    else
        color = D3DCOLOR_ARGB(255, 0, 255, 0);

    const _int   segments = 32;   // 원 세그먼트 수

    _vec3 center = m_pTrans->Get_Pos() + m_vOffset;
    float radius = m_fRadius * m_fScale; // 필요하면 충돌 공식에 맞게 조정

    m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    m_pGraphicDevice->SetTexture(0, nullptr);
    m_pGraphicDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

    auto DrawCircle = [&](_int plane)
        {
            // plane: 0 = XZ, 1 = XY, 2 = YZ
            DebugVtx v[segments + 1];

            for (int i = 0; i <= segments; ++i)
            {
                float t = (2.f * D3DX_PI * i) / segments;
                float ct = cosf(t);
                float st = sinf(t);

                switch (plane)
                {
                case 0: // XZ 평면 (바닥면)
                    v[i].x = center.x + ct * radius;
                    v[i].y = center.y;
                    v[i].z = center.z + st * radius;
                    break;

                case 1: // XY 평면 (정면)
                    v[i].x = center.x + ct * radius;
                    v[i].y = center.y + st * radius;
                    v[i].z = center.z;
                    break;

                case 2: // YZ 평면 (측면)
                    v[i].x = center.x;
                    v[i].y = center.y + ct * radius;
                    v[i].z = center.z + st * radius;
                    break;
                }

                v[i].color = color;
            }

            // LINESTRIP은 정점개수 - 1 이 primitiveCount
            m_pGraphicDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, segments, v, sizeof(DebugVtx));
        };

    DrawCircle(0); // XZ
    DrawCircle(1); // XY
    DrawCircle(2); // YZ

    // 상태 복원
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, &prevWorld);
    m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, prevFillMode);
    m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, prevLighting);
}
#endif

CSphereCollider* CSphereCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CSphereCollider* pInstance = new CSphereCollider(pGraphicDev);

    if (FAILED(pInstance->Ready_SphereCollider()))
    {
        MSG_BOX("CCollier Create Failed");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

CComponent* CSphereCollider::Clone()
{
    CCollider* pClone = new CSphereCollider(*this);
    CCollisionManager::GetInstance()->Add_Collider(pClone);

    return pClone;
}

void CSphereCollider::Free()
{
    CCollider::Free();
}

#pragma region Editor
#ifdef _DEBUG

void CSphereCollider::Display_Editor(const char* pObjTag)
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(15, 15, 45, 255));
    const _float lineH = ImGui::GetFrameHeightWithSpacing();

    ImGui::Text("Radius");
    if (ImGui::BeginChild("Radius", ImVec2(0, lineH * 2.0f), true))
    {
        ImGui::PushItemWidth(40);
        ImGui::InputFloat("s Radius", &m_fRadius, 0.f, 0.f, "%.2f", ImGuiInputTextFlags_None);
        ImGui::PopItemWidth();
    }
    ImGui::EndChild();

    ImGui::Spacing();

    CCollider::Display_Editor(pObjTag);
    ImGui::PopStyleColor();
}

#endif
#pragma endregion
