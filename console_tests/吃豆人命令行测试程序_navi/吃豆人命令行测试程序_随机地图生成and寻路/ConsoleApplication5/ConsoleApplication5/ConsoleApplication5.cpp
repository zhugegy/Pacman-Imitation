// ConsoleApplication5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DoNotUse.h"
#include "GameMap.h"
#include "CommonMacros.h"
#include "SingleInstanceMacro.h"
#include <stdlib.h>
#include <time.h>
#include "GYAStarNavi.h"
#include "GYEasyStack.h"

#define MAZE_WIDTH 32
#define MAZE_HEIGHT 32
//地形标示符
#define MAZE_PATH 0
#define MAZE_WALL 1
//int makemap(char *pStartpoint, int Height, int Width);

CDoNotUse obj;

int print_maze(char chMaze[][MAZE_WIDTH])
{
  int i = 0;
  int j = 0;

  

  for (i = 0; i < MAZE_HEIGHT; i++)
  {
    for (j = 0; j < MAZE_WIDTH; j++)
    {
      switch (chMaze[i][j])
      {
      case MAZE_PATH:
        printf("  ");
        break;
      case MAZE_WALL:
        printf("■");
        break;
        /*case MAZE_ENTRANCE:
        printf("→");
        break;
        case MAZE_EXIT:
        printf("☆");
        break;*/
      case 6:
        printf("☆");
        break;
      case 'g':
        printf("→");
        break;
      case 7:
        printf("¤");
      default:
        break;
      }
    }

    printf("\r\n");
  }

  printf("\r\n\r\n");

  return 0;
}

int main()
{
  char chMaze[32][32] = { 0 };
  srand((unsigned int)time(0));

  //makemap((char *)chMaze, 16, 16);
  
  CGameMap *p = GET_SINGLE(CGameMap);

  CGYAStarNavi *p2 = GET_SINGLE(CGYAStarNavi);

  CGYEasyStack mystack;
  int nTmpCoordX = 0;
  int nTmpCoordY = 0;

  while (true)
  {
    p->generate_random_map();
    p->get_map((char *)chMaze);

    print_maze(chMaze);

    printf("\r\n\r\n");

    p2->load_maze();
    p2->get_path(1, 16, 30, 16, mystack);

    while (mystack.is_empty() == false)
    {
      nTmpCoordY = mystack.pop();
      nTmpCoordX = mystack.pop();
      chMaze[nTmpCoordY][nTmpCoordX] = 7;
    }
    print_maze(chMaze);

    system("pause");
    system("cls");
  }

 

  return 0;
}

/*
int makemap(char *pStartpoint, int Height, int Width)
{
  for (int i = 0; i < Width; i++)
  {
    for (int j = 0; j < Height; j++)
    {
      pStartpoint[j * Width + i] = j * Width + i;
    }
  }


  return 0;
}*/