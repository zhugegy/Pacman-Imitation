#pragma once

#define C_GY_EASY_STACK_SIZE 2048   //����32*32�Թ���Ѱ·����ʱ��Ϊ2048

//һ���򵥵�ջ�ࡣ
class CGYEasyStack
{
public:
  CGYEasyStack();
  virtual ~CGYEasyStack();
private:

private:
  int m_nBuf[C_GY_EASY_STACK_SIZE];
  int m_nTop;
public:
  int push(int nNum);
  int pop();
  bool is_empty();
  int init_stack();

};

