#include "GameElement.h"

//初始化静态常量
int CGameElement::m_nGameElementCount = 0;
CGameElement* CGameElement::pGameElement[constMaxElementNum] = { 0 };

/*
CGameElement::CGameElement()
{
  
}*/


CGameElement::~CGameElement()
{
  DeleteObject(hDrawHandel);
}


int CGameElement::GetCoordX()
{
  return m_nCoordX;
}


int CGameElement::GetCoordY()
{
  return m_nCoordY;
}


CGameElement::CGameElement(int nCoordX, int nCoordY)
{
  pGameElement[m_nGameElementCount] = this;
  m_nGameElementCount++;

  m_nCoordX = nCoordX;
  m_nCoordY = nCoordY;
  m_bIsVisble = false;
  m_bIsActive = false;
  m_nCurrentFrame = 0;
  m_nMaxFrameNum = 0;
  m_nActPoint = 0;
  m_nActPointGoal = 0;
  m_nDrawPoint = 0;
  m_nDrawPointGoal = 0;

  m_nDrawOffsetX = 0;
  m_nDrawOffsetY = 0;

  hDrawHandel = NULL;
}


int CGameElement::update()
{
  return 0;
}
