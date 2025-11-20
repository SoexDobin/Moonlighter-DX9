#include "CTransform.h"

CTransform::CTransform()
    : m_vScale(1.f, 1.f, 1.f), m_vAngle(0.f, 0.f, 0.f)
{
    ZeroMemory(m_vInfo, sizeof(m_vInfo));
    D3DXMatrixIdentity(&m_matWorld);
}

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
    :CComponent(pGraphicDev), m_vScale(1.f, 1.f, 1.f), m_vAngle(0.f, 0.f, 0.f)
{
    ZeroMemory(m_vInfo, sizeof(m_vInfo));
    D3DXMatrixIdentity(&m_matWorld);
}

CTransform::CTransform(const CTransform& rhs)
    : CComponent(rhs), m_vScale(rhs.m_vScale), m_vAngle(rhs.m_vAngle)
{
    for (_uint i = 0; i < INFO_END; ++i)
    {
        m_vInfo[i] = rhs.m_vInfo[i];
    }
    m_matWorld = rhs.m_matWorld;
}

CTransform::~CTransform()
{
}

HRESULT CTransform::Ready_Transform()
{
    D3DXMatrixIdentity(&m_matWorld);

    for (int i = 0; i < INFO_END; ++i)
    {
        memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));
    }

    return S_OK;
}

_int CTransform::Update_Component(const _float fTimeDelta)
{
    D3DXMatrixIdentity(&m_matWorld);

    for (int i = 0; i < INFO_POS; ++i)
    {
        memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));
    }

    for (int i = 0; i < INFO_POS; ++i)
    {
        m_vInfo[i] *= *((_float*)m_vScale + i);
    }

    _matrix matRot[ROT_END];

    D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
    D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
    D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

    for (_uint i = 0; i < INFO_POS; ++i)
    {
        for (_uint j = 0; j < ROT_END; ++j)
        {
            D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
        }
    }

    for (_uint i = 0; i < INFO_END; ++i)
    {
        memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));
    }

    return 0;
}

void CTransform::LateUpdate_Component()
{
}

CTransform* CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CTransform* pTransform = new CTransform(pGraphicDev);

    if (FAILED(pTransform->Ready_Transform()))
    {
        Safe_Release(pTransform);
        MSG_BOX("CTransform Create Failed");
        return  nullptr;
    }

    return pTransform;
}

CComponent* CTransform::Clone()
{
    return new CTransform(*this);
}

void CTransform::Free()
{
    Engine::CComponent::Free();
}