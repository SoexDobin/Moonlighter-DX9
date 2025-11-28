#include "TagLayerMask.h"
#include "CLayerHelper.h"

BEGIN(Engine)

void TagLayerMask::Add_Mask(const wstring& wsLayerTag)
{
    LAYERID eID = CLayerHelper::GetInstance()->GetLayerIDByName(wsLayerTag);

    if ((iMask & eID) == 0)
        iMask += eID;
}

void TagLayerMask::Sub_Mask(const wstring& wsLayerTag)
{
    LAYERID eID = CLayerHelper::GetInstance()->GetLayerIDByName(wsLayerTag);

    if ((iMask & eID) != 0)
        iMask -= eID;
}

END
