#include "pch.h"
#include "Player.h"
#include "Controller.h"

#include "GameObjectManager.h"
#include "BitmapManager.h"
#include "SceneManager.h"
#include "CreateManager.h"
#include "Stage.h"
#include "Ground.h"
#include "Effect.h"

int CPlayer::m_iSlideCount = 0;

CPlayer::CPlayer()
	: m_tFrame({})

	, m_bRunning(true)

	, m_bSliding(false)

	, m_tJumpingTime(0)
	, m_bJumping(false)
	, m_fJumpSpeed(15.f)
	, m_iJumpCount(2)
	, m_iSpinCount(0)

	, m_tBlastTime(0)
	, m_bBlast(false)

	, m_tBiggestTime(0)
	, m_bBiggest(false)

	, m_tCollisionTime(0)
	, m_bCollision(false)
	,m_bInvincible(false)

{}

CPlayer::~CPlayer()
{
	Release_GameObject();
}

int CPlayer::Ready_GameObject()
{
	m_bIsDead = false;
	m_bIsActive = true;
	m_eType = STAGE::TYPE::NORMAL;

	// 플레이어 위치
	m_tInfo.fX = 200.f;
	m_tInfo.fY = 400.f;

	// 플레이어 스프라이트 크기
	m_tInfo.iCX = 360;
	m_tInfo.iCY = 360;

	// 플레이어 충돌체 위치;
	m_tCollisionInfo.fX = m_tInfo.fX - 50;
	m_tCollisionInfo.fY = m_tInfo.fY + 100;

	// 플레이어 충돌체 크기
	m_tCollisionInfo.iCX = 75;
	m_tCollisionInfo.iCY = 160;

	// Collision 온오프
	m_bCollisionView = false;

	// 플레이어 속도
	m_fSpeed = PLAYER_SPEED;

	// 플레이어 상태
	m_bRunning = true;
	m_bSliding = false;
	m_bJumping = false;

	// 더블점프 렌더 변수
	m_iJumpCount = 2;
	m_iSpinCount = 0;

	// 프레임 기본 설정
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;

	// 상태(enum) = RUN, DASH, JUMP, D_JUMP, SLIDE
	m_tFrame.iFrameState = PLAYER::STATE::RUN;

	// 프레임 속도
	m_tFrame.dwFrameSpeed = (int)(250 / m_fSpeed);
	m_tFrame.dwFrameTime = GetTickCount64();

	// 전력질주
	m_bBlast = false;
	m_tBlastTime = GetTickCount64();

	// 거대화
	m_bBiggest = false;
	m_tBiggestTime = GetTickCount64();

	// 충돌
	m_bCollision = false;
	m_tCollisionTime = GetTickCount64();
	
	m_bDive = false;

	m_bExhaustTime = GetTickCount64();

	m_tFireDelay = GetTickCount64();
	m_bFinish = false;

	return READY_OK;
}

int CPlayer::Update_GameObject()
{
	m_tCollisionRect.left   = LONG(m_tCollisionInfo.fX - (m_tCollisionInfo.iCX >> 1));
	m_tCollisionRect.top    = LONG(m_tCollisionInfo.fY - (m_tCollisionInfo.iCY >> 1));
	m_tCollisionRect.right  = LONG(m_tCollisionInfo.fX + (m_tCollisionInfo.iCX >> 1));
	m_tCollisionRect.bottom = LONG(m_tCollisionInfo.fY + (m_tCollisionInfo.iCY >> 1));	

	// Collision 영역 온오프 //////////////////////////
	if (CKeyManager::Get_Instance()->Key_Down(KEY_F1))
	{
		if (m_bCollisionView == false)
		{
			m_bCollisionView = true;
		}
		else
		{
			m_bCollisionView = false;
		}
	}

	if (m_bFinish == true)
	{
		m_tInfo.fX += m_fSpeed;
		if (m_bJumping == true)
		{
			m_tInfo.fY -= 0 - (2 * G * (GetTickCount64() - m_tJumpingTime) / 1000.f);
		}

		if (m_tRect.left >= CONSTANT::RES_X)
		{
			static_cast<CStage*>(CSceneManager::Get_Instance()->Get_Scene())->IsPlayerDead();
		}
	}
	else if (m_bFinish == false)
	{
		if (true == m_bDive)
		{
			m_fSpeed = 0;
		}
		else if (true == m_bExhaust)
		{
			m_bSliding = false;
			m_bRunning = false;
			m_bCollision = false;
			m_tFrame.dwFrameSpeed = 150;

			if (m_bJumping == true)
			{
				m_tInfo.fY -= 0 - (3 * G * (GetTickCount64() - m_tJumpingTime) / 1000.f);
			}
		}
		else if (true == m_bCollision)
		{
			//장애물과 충돌했을 경우
			m_fSpeed = PLAYER_SPEED / 2;
			m_tFrame.dwFrameSpeed = 100;

			if (m_bJumping == true)
			{
				m_tInfo.fY -= 0 - (5 * G * (GetTickCount64() - m_tJumpingTime) / 1000.f);
			}
		}
		else
		{
			if (m_eType == STAGE::TYPE::NORMAL)
			{
				// 슬라이딩 ///////////////////////////////////////     
				if (CKeyManager::Get_Instance()->Key_Pressing(KEY_DOWN) && m_bJumping == false)
				{
					if (m_iSlideCount == 0)
					{
						CSoundManager::Get_Instance()->PlaySound(L"Slide.mp3", CSoundManager::CHANNELID::SLIDE);
						++m_iSlideCount;
					}
					m_bSliding = true;
					m_tFrame.iFrameState = PLAYER::STATE::SLIDE;
					m_tFrame.iFrameEnd = 1;

					if (m_bBiggest == true)
					{
						m_tCollisionInfo.fY = m_tInfo.fY + 270;
						m_tCollisionInfo.iCY = m_tInfo.iCY - 530;
					}
					else
					{
						m_tCollisionInfo.fY = m_tInfo.fY + 140;
						m_tCollisionInfo.iCY = m_tInfo.iCY - 280;
					}

					m_bRunning = false;

				}
				// 기본 상태 //////////////////////////////////////
				else
				{
					m_iSlideCount = 0;
					m_bSliding = false;
					m_bRunning = true;
					m_bExhaust = false;

					m_tFrame.iFrameState = PLAYER::STATE::RUN;
					m_tFrame.dwFrameSpeed = (int)(250 / m_fSpeed);
					m_tFrame.iFrameEnd = 3;

					m_tCollisionInfo.fX = m_tInfo.fX + 20;
					m_tCollisionInfo.fY = m_tInfo.fY + 100;
					m_tCollisionInfo.iCY = m_tInfo.iCY - 200;

					// Blast Jelly를 먹었을 경우
					if (m_bBlast == true)
					{
						m_fSpeed = PLAYER_SPEED * 2;
						m_tFrame.iFrameState = PLAYER::STATE::DASH;
						m_tFrame.iFrameEnd = 3;

						if (m_bBlast == true && m_tFireDelay + 50 < GetTickCount64())
						{
							CGameObject* pFire = CEffect::Create(ITEM::TYPE::BLAST_EFFECT, m_tCollisionRect.left - 50, m_tCollisionRect.bottom - 100);
							CGameObjectManager::Get_Instance()->Register_GameObject(OBJECT::TYPE::EFFECT, pFire);
							m_tFireDelay = GetTickCount64();
						}

					}
					else
					{
						m_fSpeed = PLAYER_SPEED;
					}

					// 거대화 했을 경우
					if (m_bBiggest == true && m_tInfo.iCX < 730)
					{
						m_tRect.left += 15;
						m_tRect.top += 15;
						m_tInfo.iCX += 15;
						m_tInfo.iCY += 15;
					}
					else if (m_bBiggest == false && m_tInfo.iCX > 360)
					{
						m_tRect.left += 12;
						m_tRect.top += 12;
						m_tInfo.iCX -= 12;
						m_tInfo.iCY -= 12;

						m_tInfo.fY += 10;
					}
				}

				// 점프 ///////////////////////////////////////////
				if (CKeyManager::Get_Instance()->Key_Down(KEY_SPACE) && m_iJumpCount > 0)
				{
					CSoundManager::Get_Instance()->PlaySound(L"Jump.mp3", CSoundManager::CHANNELID::JUMP);
					m_bJumping = true;
					--m_iJumpCount;

					m_bRunning = false;
					m_bSliding = false;

					m_fJumpSpeed = 15.f;
					m_tJumpingTime = GetTickCount64();
				}

				// 점프중 좌표이동
				if (m_bJumping == true)
				{
					m_tInfo.fY -= m_fJumpSpeed - (5 * G * (GetTickCount64() - m_tJumpingTime) / 1000.f);
				}

				// 이단점프 프레임 설정
				if (m_iJumpCount == 1)
				{
					m_tFrame.iFrameState = PLAYER::STATE::JUMP;
					m_tFrame.iFrameEnd = 1;
				}
				else if (m_iJumpCount == 0)
				{
					m_tFrame.iFrameState = PLAYER::STATE::D_JUMP;
					m_tFrame.iFrameEnd = 9;
					//m_tFrame.dwFrameSpeed = 500;
				}
			}
			else if (m_eType == STAGE::TYPE::BT_IN)
			{
				m_tCollisionInfo.fX = m_tInfo.fX + 20;
				m_tCollisionInfo.fY = m_tInfo.fY + 100;
				m_tCollisionInfo.iCY = m_tInfo.iCY - 200;

				m_tFrame.iFrameState = PLAYER::STATE::INTRO;
				m_tFrame.dwFrameSpeed = 95;
				m_tFrame.iFrameEnd = 20;

				m_fJumpSpeed = 15.f;
				m_tInfo.fY -= m_fJumpSpeed - (1.5 * G * (GetTickCount64() - m_tIntroTime) / 1000.f);
			}
			else if (m_eType == STAGE::TYPE::BONUSTIME)
			{
				m_tCollisionInfo.fX = m_tInfo.fX + 20;
				m_tCollisionInfo.fY = m_tInfo.fY + 100;
				m_tCollisionInfo.iCY = m_tInfo.iCY - 200;

				if (CKeyManager::Get_Instance()->Key_Pressing(KEY_SPACE))
				{
					m_tFrame.iFrameState = PLAYER::STATE::UP;
					m_tFrame.iFrameEnd = 2;

					if (m_bBiggest == true)
					{
						if (m_tRect.top > -350)
						{
							m_tInfo.fY -= 8;
						}
					}
					else
					{
						if (m_tRect.top > -100)
						{
							m_tInfo.fY -= 8;
						}
					}
				}
				else
				{
					if (m_tRect.bottom < 580)
					{
						m_tInfo.fY += 8;
					}

					m_tFrame.iFrameState = PLAYER::STATE::FLYING;
					m_tFrame.dwFrameSpeed = (int)(250 / m_fSpeed);
					m_tFrame.iFrameEnd = 8;

					// Blast Jelly를 먹었을 경우
					if (m_bBlast == true)
					{
						m_fSpeed = PLAYER_SPEED * 2;
					}
					else
					{
						m_fSpeed = PLAYER_SPEED;
					}

					// 거대화 했을 경우
					if (m_bBiggest == true && m_tInfo.iCX < 730)
					{
						m_tRect.left += 15;
						m_tRect.top += 15;
						m_tInfo.iCX += 15;
						m_tInfo.iCY += 15;
					}
					else if (m_bBiggest == false && m_tInfo.iCX > 360)
					{
						m_tRect.left += 12;
						m_tRect.top += 12;
						m_tInfo.iCX -= 12;
						m_tInfo.iCY -= 12;

						m_tInfo.fY += 10;
					}
				}
			}
			else if (m_eType == STAGE::TYPE::BT_OUT_1)
			{
				m_bBlast = false;
				m_bBiggest = false;
				m_fSpeed = PLAYER_SPEED;

				m_tCollisionInfo.fX = m_tInfo.fX + 20;
				m_tCollisionInfo.fY = m_tInfo.fY + 100;
				m_tCollisionInfo.iCY = m_tInfo.iCY - 200;

				m_tFrame.iFrameState = PLAYER::STATE::DOWN;
				m_tFrame.iFrameEnd = 0;

				m_tInfo.fY += 12;
			}
			else if (m_eType == STAGE::TYPE::BT_OUT_2)
			{
				m_tCollisionInfo.fX = m_tInfo.fX + 20;
				m_tCollisionInfo.fY = m_tInfo.fY + 100;
				m_tCollisionInfo.iCY = m_tInfo.iCY - 200;

				if (m_bSetPosition == true)
				{
					m_tInfo.fX = 200.f;
					m_tInfo.fY = -400.f;
					m_bSetPosition = false;
				}

				m_tFrame.iFrameState = PLAYER::STATE::DOWN;
				m_tFrame.iFrameEnd = 0;

				m_tInfo.fY += 12;
			}
		}
	}

	return OBJ_NO_EVENT;

}

void CPlayer::Late_Update_GameObject()
{
	if (m_tInvincibleTime + 2000 < GetTickCount64())
	{
		m_bInvincible = false;
		m_tInvincibleTime = GetTickCount64();
	}

	if (m_tCollisionRect.top > CONSTANT::RES_Y && m_eType != STAGE::TYPE::BT_OUT_1 && m_bDive == false && m_bDiveSound == false)
	{
		m_bDiveTime = GetTickCount64();
		CSoundManager::Get_Instance()->PlaySound(L"Exhaust.mp3", CSoundManager::CHANNELID::EXHAUST);
		m_bDiveSound = true;
		m_bDive = true;
	}

	if (m_eType == STAGE::TYPE::BT_OUT_1)
	{
		if (m_tRect.top > CONSTANT::RES_Y)
		{
			m_tInfo.fX = -200.f;
			m_tInfo.fY = -200.f;
		}
	}

	list<CGameObject*>* plistGround = CGameObjectManager::Get_Instance()->Get_Ground();

	if (m_eType != STAGE::TYPE::BT_OUT_1)
	{
		for (auto pGround : (*plistGround))
		{
			if (m_tCollisionRect.right >= pGround->Get_Rect().left && m_tCollisionRect.left <= pGround->Get_Rect().right)
			{
				if (static_cast<CGround*>(pGround)->Get_GroundType() == GROUND::TYPE::NORMAL)
				{
					if (m_tRect.bottom > pGround->Get_Rect().top)
					{
						if (m_bFinish == true)
						{
							m_tFrame.iFrameState = 0;
							m_tFrame.iFrameEnd = 3;
						}
						if (m_eType == STAGE::TYPE::BT_OUT_2)
						{
							m_tInfo.fY = (float)(pGround->Get_Rect().top - m_tInfo.iCY / 2);
							m_eType = STAGE::TYPE::NORMAL;

							m_bRunning = true;
							m_bJumping = false;

							m_iJumpCount = 2;
							m_iSpinCount = 0;
						}
						else
						{
							m_tInfo.fY = (float)(pGround->Get_Rect().top - m_tInfo.iCY / 2);

							m_bRunning = true;
							m_bJumping = false;

							m_iJumpCount = 2;
							m_iSpinCount = 0;
						}
					}
				}
				else if (static_cast<CGround*>(pGround)->Get_GroundType() == GROUND::TYPE::_VOID)
				{
					if (true == m_bBlast || true == m_bBiggest)
					{
						if (m_tRect.bottom > pGround->Get_Rect().top)
						{
							m_tInfo.fY = (float)(pGround->Get_Rect().top - m_tInfo.iCY / 2);
						}
					}
					else
					{
						if (false == m_bJumping)
						{
							m_bJumping = true;
							m_fJumpSpeed = -10.f;
							m_tJumpingTime = GetTickCount64();
						}
					}									
				}
				else if (static_cast<CGround*>(pGround)->Get_GroundType() == GROUND::TYPE::_VOID2)
				{
					if (true == m_bBlast || true == m_bBiggest)
					{
						if (m_tRect.bottom > pGround->Get_Rect().top)
						{
							m_tInfo.fY = (float)(pGround->Get_Rect().top - m_tInfo.iCY / 2);
						}
					}
					else
					{
						if (false == m_bJumping)
						{
							m_bJumping = true;
							m_fJumpSpeed = -10.f;
							m_tJumpingTime = GetTickCount64();
						}
					}
				}
			}
		}
	}

	MoveFrame();

	if (m_bBlast == true && m_tBlastTime + BLAST < GetTickCount64())
	{
		m_bBlast = false;
		m_tBlastTime = GetTickCount64();
	}

	if (m_bBiggest == true && m_tBiggestTime + BIGGEST < GetTickCount64())
	{
		m_bBiggest = false;
		m_tBiggestTime = GetTickCount64();
	}

	if (m_bCollision == true && m_tCollisionTime + 400 < GetTickCount64())
	{
		m_bCollision = false;
		m_tCollisionTime = GetTickCount64();
	}

	if (m_bExhaust == true && m_bExhaustTime + 200 < GetTickCount64())
	{
		if (0 < m_fSpeed)
		{
			m_fSpeed -= 1.f;
		}
		else
		{
			
			m_fSpeed = 0;
		}
	
		if (m_bExhaustSound == false && m_bExhaustTime + 700 < GetTickCount64())
		{
			CSoundManager::Get_Instance()->PlaySound(L"Exhaust.mp3", CSoundManager::CHANNELID::EXHAUST);
			m_bExhaustSound = true;
		}
		if (m_bExhaustTime + 2000 < GetTickCount64())
		{
			
			static_cast<CStage*>(CSceneManager::Get_Instance()->Get_Scene())->IsPlayerDead();
			m_bExhaustTime = GetTickCount64();
		}				
	}

	if (m_bDive == true && m_bDiveTime + 1000 < GetTickCount64())
	{
		static_cast<CStage*>(CSceneManager::Get_Instance()->Get_Scene())->IsPlayerDead();
		m_bDiveTime = GetTickCount64();
	}
}

void CPlayer::Render_GameObject(HDC hDC)
{
	if (m_bInvincible==true)
	{
		if (m_bExhaust == false)
		{
			if (GetTickCount64() - m_tFrame.dwFrameTime < m_tFrame.dwFrameSpeed / 1.2f)
			{
				return;
			}
		}
		
	}

	HDC hMemDC = CBitmapManager::Get_Instance()->Find_Image(m_pCookieName);
	if (nullptr == hMemDC) { return; }
	GdiTransparentBlt
	(
		hDC,
		m_tRect.left,
		m_tRect.top,
		m_tInfo.iCX,
		m_tInfo.iCY,
		hMemDC,
		m_tFrame.iFrameStart * 360,
		m_tFrame.iFrameState * 360,
		360,
		360,
		RGB(255, 0, 255)
	);


	// Collision 영역
	if (m_bCollisionView == true)
	{
		m_tCollisionRect.left = LONG(m_tCollisionInfo.fX - (m_tCollisionInfo.iCX >> 1));
		m_tCollisionRect.top = LONG(m_tCollisionInfo.fY - (m_tCollisionInfo.iCY >> 1));
		m_tCollisionRect.right = LONG(m_tCollisionInfo.fX + (m_tCollisionInfo.iCX >> 1));
		m_tCollisionRect.bottom = LONG(m_tCollisionInfo.fY + (m_tCollisionInfo.iCY >> 1));

		Rectangle(hDC, m_tCollisionRect.left, m_tCollisionRect.top, m_tCollisionRect.right, m_tCollisionRect.bottom);

		HFONT	hFont, OldFont;

		hFont = CreateFont
		(
			50,
			0, 0, 0, 0, 0, 0, 0,
			HANGEUL_CHARSET,
			0, 0, 0,
			VARIABLE_PITCH | FF_ROMAN,
			TEXT("CookieRun Black")
		);
		OldFont = (HFONT)SelectObject(hDC, hFont);

		SetTextColor(hDC, RGB(255, 255, 255));
		SetBkMode(hDC, TRANSPARENT);

		TextOut(hDC, 20, 60, L"Collision ON", 12);

		SelectObject(hDC, OldFont);
		DeleteObject(hFont);
	}

	if (m_bCollisionView == true && m_bCollision == true)
	{
		HFONT	hFont, OldFont;

		hFont = CreateFont
		(
			50,
			0, 0, 0, 0, 0, 0, 0,
			HANGEUL_CHARSET,
			0, 0, 0,
			VARIABLE_PITCH | FF_ROMAN,
			TEXT("CookieRun Black")
		);
		OldFont = (HFONT)SelectObject(hDC, hFont);

		SetTextColor(hDC, RGB(255, 255, 255));
		SetBkMode(hDC, TRANSPARENT);

		TextOut(hDC, 1100, 60, L"Crash!", 6);

		SelectObject(hDC, OldFont);
		DeleteObject(hFont);
	}
}

void CPlayer::Release_GameObject()
{}

void CPlayer::MoveFrame()
{
	if (m_tFrame.iFrameState == PLAYER::STATE::D_JUMP)
	{
		if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed*1.5 < GetTickCount64())
		{
			if (m_tFrame.iFrameStart < m_tFrame.iFrameEnd)
			{
				++m_tFrame.iFrameStart;
			}
			else
			{
				m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
			}
			m_tFrame.dwFrameTime = GetTickCount64();
		}
	}
	else if(true == m_bExhaust || true == m_bCollision)
	{
		if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount64())
		{
			if (m_tFrame.iFrameStart < m_tFrame.iFrameEnd)
			{
				++m_tFrame.iFrameStart;
			}	
			else
			{
				m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
			}
			m_tFrame.dwFrameTime = GetTickCount64();
		}
	}
	else
	{
		if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount64())
		{
			++m_tFrame.iFrameStart;
			m_tFrame.dwFrameTime = GetTickCount64();
		}

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			m_tFrame.iFrameStart = 0;
		}
	}
}

CGameObject* CPlayer::Create(const TCHAR* pCookieName)
{
	CGameObject* pInstance = new CPlayer;

	if (0 > pInstance->Ready_GameObject()) { Safe_Delete(pInstance); }

	static_cast<CPlayer*>(pInstance)->Set_Cookie(pCookieName);

	return pInstance;
}
