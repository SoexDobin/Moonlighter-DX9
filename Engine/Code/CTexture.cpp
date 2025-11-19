#include "CTexture.h"
#include "CResourceManager.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev, _float fSpeed)
    : CComponent(pGraphicDev),
    m_bLoop(true), m_bOneLoop(false), m_bStop(false),
    m_iCurTex(0), m_iCurFrame(0),
    m_fSpeed(fSpeed), m_fFrameAcc(0.f)
{
    strcpy_s(m_szDisplayName, "Texture");
}

CTexture::CTexture(const CTexture& rhs)
    : CComponent(rhs),
    m_bLoop(rhs.m_bLoop), m_bOneLoop(false), m_bStop(rhs.m_bStop),
    m_iCurTex(0), m_iCurFrame(0),
    m_fSpeed(rhs.m_fSpeed), m_fFrameAcc(0.f)
{
    size_t iTex = rhs.m_vecTexture.size();

    m_vecTexture.reserve(iTex);
    m_vecTexture = rhs.m_vecTexture;
    for (_uint i = 0; i < iTex; ++i)
    {
        size_t iFrame = rhs.m_vecTexture[i].size();
        m_vecTexture[i] = rhs.m_vecTexture[i];

        for (_uint j = 0; j < iFrame; ++j)
        {
            m_vecTexture[i][j]->AddRef();
        }
    }

    strcpy_s(m_szDisplayName, "Texture");
}

CTexture::~CTexture()
{
}

const _uint CTexture::Get_FrameCount(const _uint iTex) { 
    return (_uint)m_vecTexture[iTex].size(); 
}

HRESULT CTexture::Ready_Texture(const wstring wsKey)
{
    m_vecTexture.push_back(CResourceManager::GetInstance()->Get_Sprite(wsKey));

    return S_OK;
}

void CTexture::Set_Texture(const _uint iTex, const _uint iFrame)
{
    if (m_vecTexture.empty() || m_vecTexture.size() <= iTex)
        return;
    if (m_vecTexture[iTex].empty() || m_vecTexture[iTex].size() <= iFrame)
        return;

    m_bOneLoop = false;
    m_iCurTex = iTex;
    m_iCurFrame = iFrame;
    m_pGraphicDevice->SetTexture(0, m_vecTexture[m_iCurTex][m_iCurFrame]);
}

_int CTexture::Update_Component(const _float fTimeDelta)
{
    if (m_bStop || m_vecTexture.empty())
        return 0;

    if (m_fSpeed <= 0.f)
        return 0;

    m_fFrameAcc += m_fSpeed * fTimeDelta;

    if (m_fFrameAcc < 1.f)
        return 0;

    m_fFrameAcc -= 1.f;
    m_iCurFrame++;

    if (m_bLoop)
    {
        if (m_iCurFrame >= m_vecTexture[m_iCurTex].size())
            m_iCurFrame = 0;
    }
    else
    {
        if (m_iCurFrame >= m_vecTexture[m_iCurTex].size())
        {
            m_iCurFrame = static_cast<unsigned int>(m_vecTexture[m_iCurTex].size()) - 1;
            m_bOneLoop = true;
        }
    }

    return 0;
}

void CTexture::LateUpdate_Component()
{
    m_pGraphicDevice->SetTexture(0, m_vecTexture[m_iCurTex][m_iCurFrame]);
}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fSpeed)
{
    CTexture* pInstance(nullptr);

    if (pInstance = new CTexture(pGraphicDev, fSpeed))
    {
        return pInstance;       
    }

    MSG_BOX("Texture Create Failed");
    Safe_Release(pInstance);
    return nullptr;
}

CComponent* CTexture::Clone()
{
    return new CTexture(*this);
}

void CTexture::Free()
{
    Engine::CComponent::Free();

    for_each(m_vecTexture.begin(), m_vecTexture.end(), 
        [](vector<IDirect3DBaseTexture9*>& vec) -> void {
            for_each(vec.begin(), vec.end(), CDeleteObj());
    });

    m_vecTexture.clear();
}

void CTexture::Display_Editor(const char* pObjTag)
{
     ImGui::PushItemWidth(60);

     ImGui::Checkbox("stop", &m_bStop);

     ImGui::DragFloat("speed", &m_fSpeed, 0.1f, 0.f, 40.f, "%.2f");
     ImGui::DragInt("frame", (int*)(&m_iCurFrame), 1.f, 0, m_vecTexture[m_iCurTex].size() - 1);

     ImGui::Text("cur frame : %lu", m_iCurFrame);
     ImGui::Text("cur Tex : %lu", m_iCurTex);

     ImGui::PopItemWidth();
}
