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
#include <string.h>
#include "chess.h"

void StartPosition(Position *p)
{

int i, j;

p->side = WHITE;
p->epstatus = 0;
p->castle = WKC + WQC + BKC + BQC;
// p->castle = 0;
p->wkingx = KFILE; p->bkingx = KFILE;
p->wkingy = 2; p->bkingy = 9;

for(i = 0; i < 12; i++)
  for(j = 0; j < 12; j++)
    if(i < 2 || i > 9 || j < 2 || j > 9) p->board[i][j] = OFFBOARD;
    else p->board[i][j] = 0;

for(i = 2; i < 10; i++)  {
  p->board[i][3] = WHITE + PAWN;
  p->board[i][8] = BLACK + PAWN;
  }

p->board[KRFILE][2] = WHITE + ROOK;
p->board[QRFILE][2] = WHITE + ROOK;
p->board[KNFILE][2] = WHITE + KNIGHT;
p->board[QNFILE][2] = WHITE + KNIGHT;
p->board[KBFILE][2] = WHITE + BISHOP;
p->board[QBFILE][2] = WHITE + BISHOP;
p->board[QFILE][2] = WHITE + QUEEN;
p->board[KFILE][2] = WHITE + KING;
p->board[KRFILE][9] = BLACK + ROOK;
p->board[QRFILE][9] = BLACK + ROOK;
p->board[KNFILE][9] = BLACK + KNIGHT;
p->board[QNFILE][9] = BLACK + KNIGHT;
p->board[KBFILE][9] = BLACK + BISHOP;
p->board[QBFILE][9] = BLACK + BISHOP;
p->board[QFILE][9] = BLACK + QUEEN;
p->board[KFILE][9] = BLACK + KING;

}


void Enter_Position(Position *p)
{

int i, j;
char line[500];

p->castle = 0;
p->epstatus = 0;

for(i = 0; i < 12; i++)
  for(j = 0; j < 12; j++)  {
    if(i < 2 || i > 9 || j < 2 || j > 9) p->board[i][j] = OFFBOARD;
    else p->board[i][j] = 0;  }

for(i = 9; i > 1; i--)  {
  printf("\nInput rank %d:\n", i-1);
  fgets(line, 200, stdin);
  for(j = 0; j < 8; j++)  {
    switch(line[j])  {
      case 'x' :
        p->board[j+2][i] = 0;
        break;
      case 'p' :
        p->board[j+2][i] = WHITE + PAWN;
        break;
      case 'P' :
        p->board[j+2][i] = BLACK + PAWN;
        break;
      case 'n' :
        p->board[j+2][i] = WHITE + KNIGHT;
        break;
      case 'N' :
        p->board[j+2][i] = BLACK + KNIGHT;
        break;
      case 'b' :
        p->board[j+2][i] = WHITE + BISHOP;
        break;
      case 'B' :
        p->board[j+2][i] = BLACK + BISHOP;
        break;
      case 'r' :
        p->board[j+2][i] = WHITE + ROOK;
        break;
      case 'R' :
        p->board[j+2][i] = BLACK + ROOK;
        break;
      case 'q' :
        p->board[j+2][i] = WHITE + QUEEN;
        break;
      case 'Q' :
        p->board[j+2][i] = BLACK + QUEEN;
        break;
      case 'k' :
        p->board[j+2][i] = WHITE + KING;
        p->wkingx = j+2;
        p->wkingy = i;
        break;
      case 'K' :
        p->board[j+2][i] = BLACK + KING;
        p->bkingx = j+2;
        p->bkingy = i;
        break;
      }
    }
  }

printf("\nSide to move (w/b) ? ");
fgets(line, 200, stdin);

if(line[0] == 'b') p->side = BLACK;
else p->side = WHITE;

printf("\nWhite king side castle ok (y/n) ? ");
fgets(line, 200, stdin);
if(line[0] == 'y') p->castle += WKC;
printf("\nWhite queen side castle ok (y/n) ? ");
fgets(line, 200, stdin);
if(line[0] == 'y') p->castle += WQC;
printf("\nBlack king side castle ok (y/n) ? ");
fgets(line, 200, stdin);
if(line[0] == 'y') p->castle += BKC;
printf("\nBlack queen side castle ok (y/n) ? ");
fgets(line, 200, stdin);
if(line[0] == 'y') p->castle += BQC;

printf("\nWhite En passant map entry (E = epsquare)\n");
fgets(line, 200, stdin);
for(j = 0; j < 8; j++)
  if(line[j] == 'E')  {
    p->epstatus = MakeEpSquare(j+2, 5, p->epstatus);  }

printf("\nBlack En passant map entry (E = epsquare)\n");
fgets(line, 200, stdin);
for(j = 0; j < 8; j++)
  if(line[j] == 'E')  {
    p->epstatus = MakeEpSquare(j+2, 6, p->epstatus);  }
}


void PrintPosFlags(Position *p, int len, int count)
{

char yes[2] = {'n', 'y'}, side[2] = {'W', 'B'};

printf("\nW:  [K %c%d] [KC %c] [QC %c] [EP %c%c%c%c%c%c%c%c]\n", 
  'a'+p->wkingx-2, p->wkingy-1, yes[Binary(p->castle & WKC)], yes[Binary(p->castle & WQC)],
  yes[IsEpSquare(2,5,p->epstatus)], yes[IsEpSquare(3,5, p->epstatus)], 
  yes[IsEpSquare(4,5,p->epstatus)], yes[IsEpSquare(5,5, p->epstatus)], 
  yes[IsEpSquare(6,5,p->epstatus)], yes[IsEpSquare(7,5, p->epstatus)], 
  yes[IsEpSquare(8,5,p->epstatus)], yes[IsEpSquare(9,5, p->epstatus)] );
printf("B:  [K %c%d] [KC %c] [QC %c] [EP %c%c%c%c%c%c%c%c]\n",
  'a'+p->bkingx-2, p->bkingy-1, yes[Binary(p->castle & BKC)], yes[Binary(p->castle & BQC)],
  yes[IsEpSquare(2,6,p->epstatus)], yes[IsEpSquare(3,6, p->epstatus)], 
  yes[IsEpSquare(4,6,p->epstatus)], yes[IsEpSquare(5,6, p->epstatus)], 
  yes[IsEpSquare(6,6,p->epstatus)], yes[IsEpSquare(7,6, p->epstatus)], 
  yes[IsEpSquare(8,6,p->epstatus)], yes[IsEpSquare(9,6, p->epstatus)] );
printf("Side to move: %c  Length of Series: %d  Count in Series: %d\n",
  side[Binary(p->side & BLACK)], len, count);

}


void PrintBoard(Position *p)
{

int i, j;
printf("\n\n        ---------------------------------\n");

for(j = 9; j > 1; j--)  {
  printf("     %1d  |", j - 1);
  for(i = 2; i < 10; i++)  {
    if(p->board[i][j] & BLACK) printf(" *");
    else printf(" ");
    if(p->board[i][j] & PAWN) printf("P");
    else if(p->board[i][j] & KNIGHT) printf("N");
    else if(p->board[i][j] & BISHOP) printf("B");
    else if(p->board[i][j] & ROOK) printf("R");
    else if(p->board[i][j] & QUEEN) printf("Q");
    else if(p->board[i][j] & KING) printf("K");
    else printf(" ");
    if(!(p->board[i][j] & BLACK)) printf(" ");
    printf("|");  }
  if(j > 2) printf("\n        |---+---+---+---+---+---+---+---|\n");
  }
printf("\n        ---------------------------------\n");
printf("          a   b   c   d   e   f   g   h\n\n");
}


void ClearRecord(Gamerec *g)
{

int i;

for(i = 0; i < MAXGAMESIZE; i++) 
  g->moves[i].mtype = EMPTY;

}

void PrintRecord(Gamerec *g)
{

int i, len, count;
unsigned char xside;
Position temp;

if(g->moves[0].mtype == EMPTY)  {
  printf("\nGame record is empty.\n");
  return;  }

StartPosition(&temp);

printf("\n***** GAME RECORD *****\n");

for(len = 1; len < MAXGAMESIZE; len++)  {
  if(g->moves[(len*(len-1)/2)].mtype != EMPTY) printf("\n     %2d. ", len);
  count = 0;
  if(temp.side == WHITE) xside = BLACK;
  else xside = WHITE;
  while(count < len && !IsCheck(&temp, xside))  {
    i = len * (len - 1) / 2 + count;
    if(g->moves[i].mtype != EMPTY)   {
      PrintMove(&temp, &(g->moves[i]));
      MakeMove(&temp, &(g->moves[i]));  }
    else  {
      printf("\n");
      return;  }
    count++;  
    }
  if(temp.side == WHITE) temp.side = BLACK;
  else temp.side = WHITE;
  }
}


void AddtoRecord(Move *m, Gamerec *g, int len, int count)
{

int i = 0;

i = len * (len-1) / 2;
i += count - 1;

memcpy(&(g->moves[i]), m, sizeof(Move));

}


void PopfromRecord(Gamerec *g, Position *p, int len, int count)
{

int i;

i = len * (len-1) / 2;
i += count - 1;

UnmakeMove(p, &(g->moves[i]));

g->moves[i].mtype = EMPTY;

}


unsigned long HashPosition(Position *p)
{

unsigned long i, j;
unsigned long val=0, shiftval;

for(i = 2; i < 10; i++)  
  for(j = 2; j < 10; j++)  {
    shiftval = (i-2) * 8 + j-2;
    if(p->board[i][j] > 0) val+= (1uL << shiftval);  }

for(i = 2; i < 10; i++)  
  for(j = 2; j < 10; j++)  {
    if(p->board[i][j] & BLACK) val += i * j;
    if(p->board[i][j] & WHITE) val += (i+7) * j + 1;
    if(p->board[i][j] & PAWN) val +=( i+13) * j + 3;
    if(p->board[i][j] & KNIGHT) val += i * (j + 11);
    if(p->board[i][j] & BISHOP) val += i * j + 19;
    if(p->board[i][j] & ROOK) val += i * j + 97;
    if(p->board[i][j] & KING) val += i * (j + 133);
    if(p->board[i][j] & QUEEN) val += (i+5) * j;  }

val += (unsigned long)p->side;
val += (unsigned long)p->castle;
val += (unsigned long)p->epstatus;
val = val % HASHSIZE;

return val;

}


void DumpPosition(Position *p)
{

int i, j;
char line[500], img[200];
SQUARE board[12][12];
FILE *fhtml;

memcpy(&board[0][0], &(p->board[0][0]), sizeof(SQUARE) * 144);
strcpy(img, "><img src=\"/~hyatt/images/chs/xxx.gif\" width=17 height=17");

printf("\nEnter filename : ");
fgets(line, 200, stdin);

fhtml = fopen(line, "w"); 

fprintf(fhtml, "<TABLE border cellpadding=0 hspace=10><TR><TD\n");

for(j = 9; j >= 2; j--)  {
  fprintf(fhtml, "><nobr");
  for(i = 2; i < 10; i++)  {
    if((i+j)%2 == 0) img[32] = 'b';
    else img[32] = 'w';
  if(board[i][j] == 0) { img[30] = 's'; img[31] = 'q'; }
  else {
    if(board[i][j] & WHITE) img[30] = 'w';
    else img[30] = 'b';
    if(board[i][j] & KNIGHT) img[31] = 'n';
    else if(board[i][j] & BISHOP) img[31] = 'b';
    else if(board[i][j] & ROOK) img[31] = 'r';
    else if(board[i][j] & QUEEN) img[31] = 'q';
    else if(board[i][j] & KING) img[31] = 'k';
    else img[31] = 'p';  }
  if(i == 2) fprintf(fhtml, "%s", img);
  else fprintf(fhtml, "\n%s", img);
  }
  fprintf(fhtml, "</nobr><br\n");
  }
fprintf(fhtml, "></td></tr></table>");

fclose(fhtml);

}
    
