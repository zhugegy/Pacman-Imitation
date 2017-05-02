#pragma once
#include "SingleInstanceMacro.h"
#include <windows.h>

const int constnWindowWidth = 1024;
const int constnWindowHeight = 768;

//此类用于绘图。单例类。
class CGameDrawer
{


public:

  SINGLE_INSTANCE(CGameDrawer)

  static HDC m_hdc;
  static HDC m_mdc;
  static HDC m_bufdc;

  

private:
  CGameDrawer();
  virtual ~CGameDrawer();
public:
  int init(HWND hwnd);
  int display();
  int get_window_width();
  int get_window_height();

private:
  int m_nWidowWidth;
  int m_nWidowHeight;
  
public:
  static int coordY_to_display_transfer(int nCoordY);
  static int coordX_to_display_transfer(int nCoordX);
  int clean_dc(HWND hwnd);
};

