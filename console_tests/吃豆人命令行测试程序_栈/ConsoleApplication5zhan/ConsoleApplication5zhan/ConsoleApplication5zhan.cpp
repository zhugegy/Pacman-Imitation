// ConsoleApplication5zhan.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <iostream>
#include <conio.h>
using namespace std;

class CStack
{
public:
  CStack()
  {
    nTop = -1;
    memset(buf, 0, sizeof(buf));
  }
  bool empty()
  {
    return nTop < 0 ? true : false;
  }
  int Pop()
  {
    if (nTop < 0)
    {
      cout << "Stack empty" << endl;
      _getch();
      exit(0);
    }
    else
    {
      return buf[nTop--];
    }
  }
  void Push(int n)
  {
    if (nTop >= 9)
    {
      cout << "Stack overflow" << endl;
      _getch();
      exit(0);
    }
    else
    {
      buf[++nTop] = n;
    }
  }

private:
  int buf[10];
  int nTop;
};

int _tmain(int argc, _TCHAR* argv[])
{
  CStack stack;
  stack.Push(54);
  stack.Push(14);
  stack.Push(56);

  cout << stack.Pop() << endl;
  while (!stack.empty())
    cout << stack.Pop() << endl;
  return 0;
}



