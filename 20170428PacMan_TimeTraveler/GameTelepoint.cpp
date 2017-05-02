#include "GameTelepoint.h"

/*
CGameTelepoint::CGameTelepoint()
{
}*/
CGameTelepoint::CGameTelepoint(int nCoordX, int nCoordY, int nTeleDestCoordX, int nTeleDestCoordY)
  :CGameElement(nCoordX, nCoordY)
{
  //1. 设定Telepoint自己的参数
  m_nTeleDestCoordX = nTeleDestCoordX;
  m_nTeleDestCoordY = nTeleDestCoordY;
  
  //2. 重写一些Telepoint特定参数（继承自GameElement类）

  //3. 重写一些Telepoint特定参数（继承自GameElement类）:重写画图handle

}

CGameTelepoint::~CGameTelepoint()
{
}

int CGameTelepoint::update()
{
  //0. 检查自己状态（是否发生碰撞），只适用于player

  //1.action 执行动作（移动，看指令列表，变向，寻路等）
  if (m_bIsActive == true)
  {
    m_nActPoint++;
    if (m_nActPoint >= m_nActPointGoal)
    {
      m_nActPoint = 0;
    }
  }

  //2.draw 画图
  if (m_bIsVisble == true)
  {

  }
  return 0;
}


int CGameTelepoint::get_tele_dest_coordX()
{
  return m_nTeleDestCoordX;
}


int CGameTelepoint::get_tele_dest_coordY()
{
  return m_nTeleDestCoordY;
}
