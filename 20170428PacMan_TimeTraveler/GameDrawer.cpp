#include "GameDrawer.h"

HDC CGameDrawer::m_hdc = NULL;
HDC CGameDrawer::m_mdc = NULL;
HDC CGameDrawer::m_bufdc = NULL;

CGameDrawer::CGameDrawer()
{
  m_nWidowWidth = constnWindowWidth;
  m_nWidowHeight = constnWindowHeight;
}


CGameDrawer::~CGameDrawer()
{

}


int CGameDrawer::init(HWND hwnd)
{
  HBITMAP bmp;
  m_hdc = GetDC(hwnd);
  m_mdc = CreateCompatibleDC(m_hdc);  //创建一个和hdc兼容的内存DC
  m_bufdc = CreateCompatibleDC(m_hdc);//再创建一个和hdc兼容的缓冲DC
  bmp = CreateCompatibleBitmap(m_hdc, m_nWidowWidth, m_nWidowHeight);

  SelectObject(m_mdc, bmp);
  return 0;
}


int CGameDrawer::get_window_width()
{
  return m_nWidowWidth;
}


int CGameDrawer::get_window_height()
{
  return m_nWidowHeight;
}


int CGameDrawer::display()
{
  BitBlt(m_hdc, 0, 0, m_nWidowWidth, m_nWidowHeight, m_mdc, 0, 0, SRCCOPY);
  return 0;
}


int CGameDrawer::coordY_to_display_transfer(int nCoordY)
{
  return nCoordY * 24;
}


int CGameDrawer::coordX_to_display_transfer(int nCoordX)
{
  return 128 + nCoordX * 24;
}


int CGameDrawer::clean_dc(HWND hwnd)
{
  DeleteDC(m_bufdc);
  DeleteDC(m_mdc);
  ReleaseDC(hwnd, m_hdc);

  return 0;
}
