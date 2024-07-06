/*
 *    SPDX-FileCopyrightText: 2021 Monaco F. J. <monaco@usp.br>
 *    SPDX-FileCopyrightText: 2024 joao <j.otavio.s@usp.br>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  This file is a derivative work from SYSeg (https://gitlab.com/monaco/syseg)
 *  and contains modifications carried out by the following author(s):
 *  joao <j.otavio.s@usp.br>
 */

#include <stdio.h>

int foo();
char bar();

int main()
{
  foo();
  return 0;
}

int foo(int n)
{
  int a;
  a = bar(n+1);
  return a;
}

char bar(int m)
{
  int b;
  b = m+1;
  return b;
}
