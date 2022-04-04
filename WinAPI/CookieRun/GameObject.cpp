#include "pch.h"
#include "GameObject.h"

CGameObject::CGameObject()
{}

CGameObject::~CGameObject()
{}

void CGameObject::Update_Rect_GameObject()
{
    m_tRect.left   = LONG(m_tInfo.fX - (m_tInfo.iCX >> 1));
    m_tRect.top    = LONG(m_tInfo.fY - (m_tInfo.iCY >> 1));
    m_tRect.right  = LONG(m_tInfo.fX + (m_tInfo.iCX >> 1));
    m_tRect.bottom = LONG(m_tInfo.fY + (m_tInfo.iCY >> 1));
}
