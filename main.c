/*******************************************************************************
    BUCEPHALUS Progressive Chess Analysis Tool
    Copyright (C) 2016 Doug Hyatt

    Code Author:  Doug Hyatt

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "chess.h"

int totalc = 0, dummyc = 0, scoreflag = 0;
unsigned char scoreside;
Move Best[MAXPLY], CurrLine[MAXPLY], MoreLine[MAXPLY];
Hashentry TransTable[HASHSIZE];

int main(int argc, char *argv[])
{

int i, len, count;
char line[500], sure[500], side[2][10] = {"W", "B"};
unsigned char xside;
Listnode Mlist[MAXPLY], temp;
Move PMove;
Gamerec CurrGame;
Position Current;

StartPosition(&Current);
len = 1;
count = 1;
for(i = 0; i < MAXPLY; i++)  {
  ClearMoves(&(Mlist[i]));
  Best[i].mtype = EMPTY;
  CurrLine[i].mtype = EMPTY;  }
for(i = 0; i < HASHSIZE; i++)  {
  TransTable[i].depth = -1;  }
ClearRecord(&CurrGame);
printf("\nBucephalus v%s\n\n", VERSION);

PrintBoard(&Current);

while(1)  {

  if(Current.side == WHITE) xside = BLACK;
  else xside = WHITE;
  GenerateMoves(&temp, &Current);
  printf("\n[%sS%dM%d]>  ", side[Binary(Current.side & BLACK)], len, count);
  fgets(line, 200, stdin);
  switch(line[0])  {
    case '?' : 		
    case 'h' :
      printf("\n?/h:\tPrint this help info\n");
      printf("b:\tGo back one move\n");
      printf("c:\tShow current score\n");
      printf("d:\tShow current position\n");
      printf("e:\tMinmax evaluator\n");
      printf("f:\tSeries finder\n");
      printf("l:\tShow legal moves\n");
      printf("m:\tMake a move\n");
      printf("n:\tNew game\n");
      printf("p:\tShow position status\n");
      printf("q/x:\tExit Buce\n");
      printf("r:\tShow game moves\n");
      printf("s:\tSeriesmate solver\n");
      printf("w:\tWrite position to file\n");
      printf("v:\tShow version information\n");
      break;
    case 'b' :
      if(len == 1 && count == 1) break;
      count--; 
      if(count == 0)  {
        if(Current.side == WHITE) Current.side = BLACK;
        else Current.side = WHITE;
        len--;
        count = len;
        if(len > 1)  {
          while(CurrGame.moves[len*(len-1)/2+count-1].mtype == EMPTY)
            count--;    
        }
      }
      PopfromRecord(&CurrGame, &Current, len, count);  
      break;
    case 'd' :
      PrintBoard(&Current);
      break;
    case 'e' :
      Evaluate(&Current, len, count);
      break;
    case 'f' :
      Find(&Current);
      break;
    case 'l' :
      PrintListnode(&Current, &temp);
      break;
    case 'm' :
      if(InputMove(&PMove, &temp) == -1) break;
      MakeMove(&Current, &PMove);
      AddtoRecord(&PMove, &CurrGame, len, count);
      count++;
      if(count > len || IsCheck(&Current, xside))  { 
        if(Current.side == WHITE) Current.side = BLACK;
        else Current.side = WHITE;
        count = 1;
        len++;    
      } 
      PrintBoard(&Current);
      break;
    case 'n' :
      printf("\n\nARE YOU SURE?:  ");
      fgets(sure, 200, stdin);
      if(sure[0] != 'y') break;
      StartPosition(&Current);
      len = 1;
      count = 1;
      for(i = 0; i < MAXPLY; i++)  {
        ClearMoves(&(Mlist[i]));
        Best[i].mtype = EMPTY;
        CurrLine[i].mtype = EMPTY;  }
      ClearRecord(&CurrGame);
      PrintBoard(&Current);
      break;
    case 'p' :
      PrintPosFlags(&Current, len, count);
      break;
    case 'q' :
    case 'x' :
      exit(0);
    case 'r' :
      PrintRecord(&CurrGame);
      break;
    case 's' :
      Solve(&Current);
      break;
    case 'c' :
      Score(&Current, len, -1);
      break;
    case 'v' :
      printf("\nBucephalus Version v%s\n", VERSION);
      break;
    case 'w' :
      DumpPosition(&Current);
      break;
  }


}

} 
