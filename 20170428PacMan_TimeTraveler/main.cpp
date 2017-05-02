
//-----------------------------------【程序说明】--------------------------------
//  程序名称:20170428PacMan_TimeTraveler
//  描述：吃豆人：时光旅行者（五一四天乐项目）。
//------------------------------------------------------------------------------

#include <windows.h>
#include <tchar.h>
#include <stdlib.h>

#include "SingleInstanceMacro.h"
#include "GameController.h"
#include "DoNotUse.h"

#pragma comment(lib,"winmm.lib")			//调用PlaySound函数所需库文件
#pragma  comment(lib,"Msimg32.lib")		//添加使用TransparentBlt函数所需的库文件

#define WINDOW_WIDTH	((1024)	+ (20))				//窗口宽度 + offset
#define WINDOW_HEIGHT	((768) + (40))						//窗口高度 + offset
#define WINDOW_TITLE		L"吃豆人：时光旅行者"	//窗口标题

//全局变量的声明						
//DWORD		g_tPre = 0, g_tNow = 0;					//记录时间，配合CGameController类
CDoNotUse objDoNotUse;   //全局对象，负责main()函数之前的操作。辅助单例类用的不用管它

//窗口过程函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

                                  
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
  LPSTR lpCmdLine, int nShowCmd)
{
  //窗口过程四部曲抄的《逐梦旅程：Windows游戏编程之从零开始》，毛星云编著
  //【1】窗口设计
  WNDCLASSEX wndClass = { 0 };					    		//用WINDCLASSEX定义了一个窗口类
  wndClass.cbSize = sizeof(WNDCLASSEX);	    		//设置结构体的字节数大小
  wndClass.style = CS_HREDRAW | CS_VREDRAW;   	//设置窗口的样式
  wndClass.lpfnWndProc = WndProc;				      	//设置指向窗口过程函数的指针
  wndClass.cbClsExtra = 0;						  	    	//窗口类的附加内存
  wndClass.cbWndExtra = 0;				    		    	//窗口的附加内存
  wndClass.hInstance = hInstance;					     	//指定包含窗口过程的程序的实例句柄
  wndClass.hIcon = (HICON)::LoadImage(NULL, L"game_resource\\icon.ico", IMAGE_ICON, 0, 0, 
    LR_DEFAULTSIZE | LR_LOADFROMFILE);          //本地加载自定义ico图标
  //指定窗口类的光标句柄
  wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);    
  //为hbrBackground成员指定一个白色画刷句柄	
  wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  
  //指定菜单资源的名字。
  wndClass.lpszMenuName = NULL;				
  //指定窗口类的名字。
  wndClass.lpszClassName = L"ForTheDreamOfGameDevelop";		

  //【2】窗口注册
  if (!RegisterClassEx(&wndClass))
  {
    return -1;
  }

  //【3】窗口创建
  HWND hwnd = CreateWindow(L"ForTheDreamOfGameDevelop", WINDOW_TITLE,				
    WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
    WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

  //【4】窗口更新
  MoveWindow(hwnd, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, true);		
  ShowWindow(hwnd, nShowCmd);                   //调用ShowWindow函数来显示窗口
  UpdateWindow(hwnd);						                //对窗口进行更新

  PlaySound(L"game_resource\\Approaching_Nirvana_You.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); //循环播放背景音乐 

  //游戏初始化
  CGameController *p = GET_SINGLE(CGameController);
  p->init(hwnd);
  
  //g_tPre = GetTickCount();

  //游戏主循环
  MSG msg = { 0 };				                      //定义并初始化msg
  while (msg.message != WM_QUIT)
  {
    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))  //查看应用程序消息队列
    {
      TranslateMessage(&msg);		                //将虚拟键消息转换为字符消息
      DispatchMessage(&msg);			              //分发一个消息给窗口程序
    }

    //当此次循环运行与上次绘图时间相差0.05秒时再进行重绘操作
    /*g_tNow = GetTickCount();
    if (g_tNow != g_tPre)
    {
      g_tPre = g_tNow;
      p->loop();
    }*/
    p->loop();
  }

  //【Z】窗口注销
  UnregisterClass(L"ForTheDreamOfGameDevelop", wndClass.hInstance);
  return 0;
}

//窗口过程函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  CGameController *pGameController = GET_SINGLE(CGameController);
  CGamePlayer *pGamePlayer = pGameController->get_GamePlayer();

  switch (message)						
  {
  case WM_KEYDOWN:	          //按下键盘消息
                     
    switch (wParam)
    {
    case VK_ESCAPE:           //按下【Esc】键
      DestroyWindow(hwnd);    // 销毁窗口
      PostQuitMessage(0);     //结束程序
      break;
    case VK_UP:				        //按下【↑】键
      //pGameController->m_pGamePlayer->give_change_direction_attemption(CGameElement::UP);
      pGamePlayer->give_change_direction_attemption(CGameElement::UP);
      break;
    case VK_DOWN:			        //按下【↓】键
      //pGameController->m_pGamePlayer->give_change_direction_attemption(CGameElement::DOWN);
      pGamePlayer->give_change_direction_attemption(CGameElement::DOWN);
      break;
    case VK_LEFT:			        //按下【←】键			
      //pGameController->m_pGamePlayer->give_change_direction_attemption(CGameElement::LEFT);
      pGamePlayer->give_change_direction_attemption(CGameElement::LEFT);
      break;
    case VK_RIGHT:			      //按下【→】键
      //pGameController->m_pGamePlayer->give_change_direction_attemption(CGameElement::RIGHT);
      pGamePlayer->give_change_direction_attemption(CGameElement::RIGHT);
      break;
    case VK_SPACE:           ////按下空格键
      pGameController->run_game();
      break;
    }
    break;								

  case WM_DESTROY:					  //窗口销毁消息
    {
    CGameController *p = GET_SINGLE(CGameController);
    p->terminate(hwnd);
    PostQuitMessage(0);			
    break;									
    }

  default:		
    //调用缺省的窗口过程
    return DefWindowProc(hwnd, message, wParam, lParam);		
  }

  return 0;
}
