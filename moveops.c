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

void AddMove(Listnode *ml, int sourcex, int sourcey, int destx, int desty, short mtype)
{

int i;
unsigned char val[4] = { KNIGHT, BISHOP, ROOK, QUEEN };
Move *m;

m = &(ml->moves[ml->num]);

if(mtype == PROMOTE || mtype == PROMCAP)
  for(i = 0; i < 4; i++)  {
    m->source = (short)(12*sourcex + sourcey);
    m->dest = (short)(12*destx + desty);
    m->mtype = mtype;
    m->promval = val[i];
    ml->num++;
    m->oldpiece = 0;
    m->oldcastle = 0;
    m->oldep = 0;
    m = &(ml->moves[ml->num]);
    }
else  {
    m->source = (short)(12*sourcex + sourcey);
    m->dest = (short)(12*destx + desty);
    m->mtype = mtype;
    m->promval = 0;
    m->oldpiece = 0;
    m->oldcastle = 0;
    m->oldep = 0;
    ml->num++;
    }

}


void ClearMoves(Listnode *l)
{

int i;

for(i = 0; i < MAXLEGAL; i++)
  l->moves[i].mtype = EMPTY;

l->num = 0;

}


void PrintListnode(Position *p, Listnode *ml)
{

int i, count = 0;
Move *tmp;

printf("\n**%2d moves in list node**\n", ml->num);
for(i = 0; i < MAXLEGAL; i++)  {
  if(count == ml->num) break;
  tmp = &(ml->moves[i]);
  if(tmp->mtype == EMPTY) continue; 
  count++;
  PrintMove(p, tmp);  }
printf("\n\n");
}


void PrintMove(Position *p, Move *tmp)
{

short sx, sy, dx, dy;
char mstring[12];
unsigned char xside;

if(p->side == WHITE) xside = BLACK;
else xside = WHITE;

MakeMove(p, tmp);
sx = (short)(tmp->source/12) - 2;
sy = (short)(tmp->source%12) - 2;
dx = (short)(tmp->dest/12) - 2;
dy = (short)(tmp->dest%12) - 2;
mstring[0] = 'a' + sx;
mstring[1] = '1' + sy;
if(tmp->mtype == PROMCAP || tmp->mtype == CAPTURE) mstring[2] = 'x';
else mstring[2] = '-';
mstring[3] = 'a' + dx;
mstring[4] = '1' + dy;
if(tmp->mtype == PROMCAP || tmp->mtype == PROMOTE)  {
  mstring[5] = '=';
  if(tmp->promval == KNIGHT)  mstring[6] = 'N';
  else if(tmp->promval == BISHOP)  mstring[6] = 'B';
  else if(tmp->promval == ROOK)  mstring[6] = 'R';
  else mstring[6] = 'Q';
  if(IsCheck(p, xside) == 1)  {
    mstring[7] = '+';
    if(IsMate(p, xside) == 1) { mstring[8] = '+'; mstring[9] = '\0'; }
    else mstring[8] = '\0';   }
  else mstring[7] = '\0';
  }
else  {
  if(IsCheck(p, xside) == 1)  {
    mstring[5] = '+';
    if(IsMate(p, xside) == 1) { mstring[6] = '+'; mstring[7] = '\0'; }
    else mstring[6] = '\0';   }
  else mstring[5] = '\0';
  }
  printf("%s ", mstring);
UnmakeMove(p, tmp);

}
 
void PrintMoveBrief(Move *tmp)
{

short sx, sy, dx, dy;
char mstring[12];

sx = (short)(tmp->source/12) - 2;
sy = (short)(tmp->source%12) - 2;
dx = (short)(tmp->dest/12) - 2;
dy = (short)(tmp->dest%12) - 2;
mstring[0] = 'a' + sx;
mstring[1] = '1' + sy;
if(tmp->mtype == PROMCAP || tmp->mtype == CAPTURE) mstring[2] = 'x';
else mstring[2] = '-';
mstring[3] = 'a' + dx;
mstring[4] = '1' + dy;
if(tmp->mtype == PROMCAP || tmp->mtype == PROMOTE)  {
  mstring[5] = '=';
  if(tmp->promval == KNIGHT)  mstring[6] = 'N';
  else if(tmp->promval == BISHOP)  mstring[6] = 'B';
  else if(tmp->promval == ROOK)  mstring[6] = 'R';
  else mstring[6] = 'Q';
  mstring[7] = '\0';
  }
else mstring[5] = '\0';
printf("%s ", mstring);
}


void MakeMove(Position *p, Move *m)
{

int direc, i;
short sx, sy, dx, dy;
unsigned char side = p->side, orig;

if(side == WHITE) direc = 1;
else direc = -1;

sx = (short)(m->source/12);
sy = (short)(m->source%12);
dx = (short)(m->dest/12);
dy = (short)(m->dest%12);

m->oldcastle = p->castle;
m->oldep = p->epstatus;
m->oldpiece = p->board[dx][dy];

orig = p->board[sx][sy];
p->board[dx][dy] = p->board[sx][sy];
if(sx != dx || sy != dy) p->board[sx][sy] = 0;

/* keep track of the king */

if((orig & KING) && (orig & WHITE))  {
  p->wkingx = dx;  p->wkingy = dy;  }
if((orig & KING) && (orig & BLACK))  {
  p->bkingx = dx;  p->bkingy = dy;  }

/* deal with castling and moves that make castling illegal */

if(m->mtype == KSCASTLE)  {
  if(p->side == WHITE)  {
    p->board[7][2] = WHITE + ROOK;
    if(KRFILE != 7 && KFILE != 7) p->board[KRFILE][2] = 0;  }
  else {
    p->board[7][9] = BLACK + ROOK;
    if(KRFILE != 7 && KFILE != 7) p->board[KRFILE][9] = 0;  }}
if(m->mtype == QSCASTLE)  {
  if(p->side == WHITE)  {
    p->board[5][2] = WHITE + ROOK;
    if(QRFILE != 5 && KFILE != 5) p->board[QRFILE][2] = 0;  }
  else {
    p->board[5][9] = BLACK + ROOK;
    if(QRFILE != 5 && KFILE != 5) p->board[QRFILE][9] = 0;  }}

if(p->castle & WKC)  {
  if(sx == KFILE && sy == 2) p->castle -= WKC;
  if(sx == KRFILE && sy == 2) p->castle -= WKC;
  if(dx == KRFILE && dy == 2 && (side & BLACK)) p->castle -= WKC;
  }

if(p->castle & WQC)  {
  if(sx == KFILE && sy == 2) p->castle -= WQC;  
  if(sx == QRFILE && sy == 2) p->castle -= WQC;
  if(dx == QRFILE && dy == 2 && (side & BLACK)) p->castle -= WQC;
  }

if(p->castle & BKC)  {
  if(sx == KFILE && sy == 9) p->castle -= BKC;
  if(sx == KRFILE && sy == 9) p->castle -= BKC;
  if(dx == KRFILE && dy == 9 && (side & WHITE)) p->castle -= BKC;
  }

if(p->castle & BQC)  {
  if(sx == KFILE && sy == 9) p->castle -= BQC;
  if(sx == QRFILE && sy == 9) p->castle -= BQC;
  if(dx == QRFILE && dy == 9 && (side & WHITE)) p->castle -= BQC;
  }

/* change pawn on 8th rank to its new piece type */

if(m->mtype == PROMOTE || m->mtype == PROMCAP)
  p->board[dx][dy] = p->board[dx][dy] - PAWN + m->promval;

/* handle all the en passant mess */

if(m->mtype == ENPASSANT)  
  p->board[dx][dy-direc] = 0;

if((orig & PAWN) && direc == 1 && IsEpSquare(sx, sy, p->epstatus) == 1)  {
  p->epstatus = RemoveEpSquare(sx, sy, p->epstatus);  }

if((orig & PAWN) && direc == -1 && IsEpSquare(sx, sy, p->epstatus) == 1)  {
  p->epstatus = RemoveEpSquare(sx, sy, p->epstatus);  }

if(direc == -1 && p->epstatus > 0 && p->epstatus < 256) {
  for(i = 2; i < 10; i++)  
    p->epstatus = RemoveEpSquare(i, 5, p->epstatus);  }

if(p->epstatus > 255 && direc == 1) {
  for(i = 2; i < 10; i++)  
    p->epstatus = RemoveEpSquare(i, 6, p->epstatus);  }

if(m->mtype == TWOMOVE) { p->epstatus = MakeEpSquare(dx, dy, p->epstatus);  }

}


void UnmakeMove(Position *p, Move *m)  
{

int direc;
short sx, sy, dx, dy;
unsigned char side = p->side, orig;

if(side == WHITE) direc = 1;
else direc = -1;

sx = (short)(m->source/12);
sy = (short)(m->source%12);
dx = (short)(m->dest/12);
dy = (short)(m->dest%12);

orig = p->board[dx][dy];
p->board[sx][sy] = p->board[dx][dy];
p->board[dx][dy] = m->oldpiece;
p->castle = m->oldcastle;
p->epstatus = m->oldep;

if(m->mtype == PROMOTE || m->mtype == PROMCAP) {
  if(p->board[sx][sy] & WHITE) p->board[sx][sy] = WHITE + PAWN;
  else p->board[sx][sy] = BLACK + PAWN;
}

if((orig & KING) && (orig & WHITE))  {
  p->wkingx = sx;  p->wkingy = sy;  }
if((orig & KING) && (orig & BLACK))  {
  p->bkingx = sx;  p->bkingy = sy;  }

/* deal with castling and moves that make castling illegal */

if(m->mtype == KSCASTLE)  {
  if(orig & WHITE)  {
    p->board[KRFILE][2] = WHITE + ROOK;
    if(KRFILE != 7 && KFILE != 7) p->board[7][2] = 0;  }
  else {
    p->board[KRFILE][9] = BLACK + ROOK;
    if(KRFILE != 7 && KFILE != 7) p->board[7][9] = 0;  }}
if(m->mtype == QSCASTLE)  {
  if(orig & WHITE)  {
    p->board[QRFILE][2] = WHITE + ROOK;
    if(QRFILE != 5 && KFILE != 5) p->board[5][2] = 0;  }
  else {
    p->board[QRFILE][9] = BLACK + ROOK;
    if(QRFILE != 5 && KFILE != 5) p->board[5][9] = 0;  }}

if(m->mtype == ENPASSANT)  {
  if(p->side == WHITE) p->board[dx][dy-direc] = BLACK + PAWN;
  else p->board[dx][dy-direc] = WHITE + PAWN;  }

}


int MoveCompare(Move *m1, Move *m2)
{

if(m2->mtype == EMPTY) return -1;
if(m1->source != m2->source) return -1;
if(m1->dest != m2->dest) return -1;
if(m1->promval != m2->promval) return -1; 
return 0;

}


int InputMove(Move *m, Listnode *l)
{

int i;
short sx, sy, dx, dy;
char line[500];
unsigned char prom;

printf("\nEnter your move [? for help]--> ");

fgets(line, 200, stdin);
if(line[0] == '?')  {
  printf("\n\nMoves should be entered in coordinate notation, e.g.\n");
  printf("e2e4, d7d5, etc.  Always use source and destination squares,\n");
  printf("so castling is e1g1, en passant is d5e6, etc.  To promote,\n");
  printf("enter the piece value immediately after your move, e.g. c7c8N.\n");
  printf("B is bishop, R is rook, Q is queen.  Press m to enter your move.\n");
}
sx = (short)(line[0] - 'a' + 2);
dx = (short)(line[2] - 'a' + 2);
sy = (short)(line[1] - '1' + 2);
dy = (short)(line[3] - '1' + 2);
m->source = sx*12 + sy;
m->dest = dx*12 + dy;
if(line[4] == 'N') prom = KNIGHT;
else if(line[4] == 'B') prom = BISHOP;
else if(line[4] == 'R') prom = ROOK;
else if(line[4] == 'Q') prom = QUEEN;
else prom = 0;
m->promval = prom;
if(sx < 2 || dx < 2 || sy < 2 || dy < 2)  {
  printf("\nRange is from a to h and 1 to 8.\n");
  return -1;  }
if(sx > 9 || dx > 9 || sy > 9 || dy > 9)  {
  printf("\nRange is from a to h and 1 to 8.\n");
  return -1;  }

for(i = 0; i < MAXLEGAL; i++)  {
  if(MoveCompare(m, &(l->moves[i])) == 0)  {
    m->mtype = l->moves[i].mtype;
    m->source = l->moves[i].source;
    m->dest = l->moves[i].dest;
    m->promval = l->moves[i].promval;   
    return 0;  }}

printf("\nInvalid move entered!  Do 'l' to see legal moves.\n");
return -1; 

}



void SortMoves(Listnode *ml, Position *p, int depth, int count, int len, int lindex, int npval)
{

int i, j = 0, k = 0, flipflag, val[MAXLEGAL];
unsigned long hkey;
unsigned char xside, tside = p->side;
Move *tmp, m;
Listnode tln;

if(p->side == WHITE) xside = BLACK;
else xside = WHITE;

memcpy(&tln, ml, sizeof(Listnode));

for(i = 0; i < MAXLEGAL; i++)  {
  if(j == tln.num) break;
  tmp = &(tln.moves[i]);
  if(tmp->mtype == EMPTY) continue;
  MakeMove(p, tmp);
  if(count == len || IsCheck(p, xside)) flipflag = 1;
  else flipflag = 0;
  if(flipflag == 1) p->side = xside;
  hkey = HashPosition(p);
  if(TransTable[hkey].depth == -1)   {
    val[k] = 0;  }
  else if(ResolveCollision(p, hkey, depth, lindex+1) == 0) {
    val[k] = TransTable[hkey].value;  
    if(NULLPRUNE && npval != -1000001)  {
      if(!flipflag && p->side == WHITE && val[k] <= npval + 50)  {
        UnmakeMove(p, tmp);
        j++;
        continue;  }
      else if(!flipflag && p->side == BLACK && val[k] >= npval - 50)  {
        UnmakeMove(p, tmp);
        j++;
        continue;  }
      }}
  else {
    if(flipflag == 1) p->side = tside;
    UnmakeMove(p, tmp);
    j++;
    continue;
  }
  memcpy(&(ml->moves[k]), tmp, sizeof(Move));  
  k++;
  j++;
  if(flipflag == 1) p->side = tside;
  UnmakeMove(p, tmp);
  }
ml->num = k;
ml->moves[k].mtype = EMPTY;

if(p->side == BLACK)  {
for(i = 1; i < ml->num; i++)  {
  if(val[i] < val[i-1]) {
    memcpy(&m, &(ml->moves[i]), sizeof(Move));
    for(j = i-1; j >=0; j--)  {
      memcpy(&(ml->moves[j+1]), &(ml->moves[j]), sizeof(Move));
      if(j == 0 || val[j-1] < val[i]) break;  }
    memcpy(&(ml->moves[j]), &m, sizeof(Move));
    }
  }
  }
else  {
for(i = 1; i < ml->num; i++)  {
  if(val[i] > val[i-1]) {
    memcpy(&m, &(ml->moves[i]), sizeof(Move));
    for(j = i-1; j >=0; j--)  {
      memcpy(&(ml->moves[j+1]), &(ml->moves[j]), sizeof(Move));
      if(j == 0 || val[j-1] > val[i]) break;  }
    memcpy(&(ml->moves[j]), &m, sizeof(Move));
    }
  }
  }

if(val[97] == 12)  {
  PrintListnode(p, &tln);
  PrintListnode(p, ml);  }
}





