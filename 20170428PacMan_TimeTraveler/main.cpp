
//-----------------------------------������˵����--------------------------------
//  ��������:20170428PacMan_TimeTraveler
//  �������Զ��ˣ�ʱ�������ߣ���һ��������Ŀ����
//------------------------------------------------------------------------------

#include <windows.h>
#include <tchar.h>
#include <stdlib.h>

#include "SingleInstanceMacro.h"
#include "GameController.h"
#include "DoNotUse.h"

#pragma comment(lib,"winmm.lib")			//����PlaySound����������ļ�
#pragma  comment(lib,"Msimg32.lib")		//���ʹ��TransparentBlt��������Ŀ��ļ�

#define WINDOW_WIDTH	((1024)	+ (20))				//���ڿ�� + offset
#define WINDOW_HEIGHT	((768) + (40))						//���ڸ߶� + offset
#define WINDOW_TITLE		L"�Զ��ˣ�ʱ��������"	//���ڱ���

//ȫ�ֱ���������						
//DWORD		g_tPre = 0, g_tNow = 0;					//��¼ʱ�䣬���CGameController��
CDoNotUse objDoNotUse;   //ȫ�ֶ��󣬸���main()����֮ǰ�Ĳ����������������õĲ��ù���

//���ڹ��̺���
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

                                  
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
  LPSTR lpCmdLine, int nShowCmd)
{
  //���ڹ����Ĳ������ġ������ọ́�Windows��Ϸ���֮���㿪ʼ����ë���Ʊ���
  //��1���������
  WNDCLASSEX wndClass = { 0 };					    		//��WINDCLASSEX������һ��������
  wndClass.cbSize = sizeof(WNDCLASSEX);	    		//���ýṹ����ֽ�����С
  wndClass.style = CS_HREDRAW | CS_VREDRAW;   	//���ô��ڵ���ʽ
  wndClass.lpfnWndProc = WndProc;				      	//����ָ�򴰿ڹ��̺�����ָ��
  wndClass.cbClsExtra = 0;						  	    	//������ĸ����ڴ�
  wndClass.cbWndExtra = 0;				    		    	//���ڵĸ����ڴ�
  wndClass.hInstance = hInstance;					     	//ָ���������ڹ��̵ĳ����ʵ�����
  wndClass.hIcon = (HICON)::LoadImage(NULL, L"game_resource\\icon.ico", IMAGE_ICON, 0, 0, 
    LR_DEFAULTSIZE | LR_LOADFROMFILE);          //���ؼ����Զ���icoͼ��
  //ָ��������Ĺ����
  wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);    
  //ΪhbrBackground��Աָ��һ����ɫ��ˢ���	
  wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  
  //ָ���˵���Դ�����֡�
  wndClass.lpszMenuName = NULL;				
  //ָ������������֡�
  wndClass.lpszClassName = L"ForTheDreamOfGameDevelop";		

  //��2������ע��
  if (!RegisterClassEx(&wndClass))
  {
    return -1;
  }

  //��3�����ڴ���
  HWND hwnd = CreateWindow(L"ForTheDreamOfGameDevelop", WINDOW_TITLE,				
    WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
    WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

  //��4�����ڸ���
  MoveWindow(hwnd, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, true);		
  ShowWindow(hwnd, nShowCmd);                   //����ShowWindow��������ʾ����
  UpdateWindow(hwnd);						                //�Դ��ڽ��и���

  PlaySound(L"game_resource\\Approaching_Nirvana_You.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); //ѭ�����ű������� 

  //��Ϸ��ʼ��
  CGameController *p = GET_SINGLE(CGameController);
  p->init(hwnd);
  
  //g_tPre = GetTickCount();

  //��Ϸ��ѭ��
  MSG msg = { 0 };				                      //���岢��ʼ��msg
  while (msg.message != WM_QUIT)
  {
    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))  //�鿴Ӧ�ó�����Ϣ����
    {
      TranslateMessage(&msg);		                //���������Ϣת��Ϊ�ַ���Ϣ
      DispatchMessage(&msg);			              //�ַ�һ����Ϣ�����ڳ���
    }

    //���˴�ѭ���������ϴλ�ͼʱ�����0.05��ʱ�ٽ����ػ����
    /*g_tNow = GetTickCount();
    if (g_tNow != g_tPre)
    {
      g_tPre = g_tNow;
      p->loop();
    }*/
    p->loop();
  }

  //��Z������ע��
  UnregisterClass(L"ForTheDreamOfGameDevelop", wndClass.hInstance);
  return 0;
}

//���ڹ��̺���
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  CGameController *pGameController = GET_SINGLE(CGameController);
  CGamePlayer *pGamePlayer = pGameController->get_GamePlayer();

  switch (message)						
  {
  case WM_KEYDOWN:	          //���¼�����Ϣ
                     
    switch (wParam)
    {
    case VK_ESCAPE:           //���¡�Esc����
      DestroyWindow(hwnd);    // ���ٴ���
      PostQuitMessage(0);     //��������
      break;
    case VK_UP:				        //���¡�������
      //pGameController->m_pGamePlayer->give_change_direction_attemption(CGameElement::UP);
      pGamePlayer->give_change_direction_attemption(CGameElement::UP);
      break;
    case VK_DOWN:			        //���¡�������
      //pGameController->m_pGamePlayer->give_change_direction_attemption(CGameElement::DOWN);
      pGamePlayer->give_change_direction_attemption(CGameElement::DOWN);
      break;
    case VK_LEFT:			        //���¡�������			
      //pGameController->m_pGamePlayer->give_change_direction_attemption(CGameElement::LEFT);
      pGamePlayer->give_change_direction_attemption(CGameElement::LEFT);
      break;
    case VK_RIGHT:			      //���¡�������
      //pGameController->m_pGamePlayer->give_change_direction_attemption(CGameElement::RIGHT);
      pGamePlayer->give_change_direction_attemption(CGameElement::RIGHT);
      break;
    case VK_SPACE:           ////���¿ո��
      pGameController->run_game();
      break;
    }
    break;								

  case WM_DESTROY:					  //����������Ϣ
    {
    CGameController *p = GET_SINGLE(CGameController);
    p->terminate(hwnd);
    PostQuitMessage(0);			
    break;									
    }

  default:		
    //����ȱʡ�Ĵ��ڹ���
    return DefWindowProc(hwnd, message, wParam, lParam);		
  }

  return 0;
}
