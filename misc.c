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

int IsCheck(Position *p, unsigned char side)  {

if(side == WHITE && Attacked(p, p->wkingx, p->wkingy, BLACK) == 1) return 1;
if(side == BLACK && Attacked(p, p->bkingx, p->bkingy, WHITE) == 1) return 1;
return 0;

}

int IsMate(Position *p, unsigned char side)  {

int x;
unsigned char tempside;

tempside = p->side;
if(IsCheck(p, side) == 0) return 0;

p->side = side;

x = AnyLegalMoves(p);
p->side = tempside;
if(x == 1) return 0;
else  {
 return 1;  }

}


int IsStalemate(Position *p, unsigned char side)  {

int x;
unsigned char tempside = p->side;

if(IsCheck(p, side) == 1) return 0;

p->side = side;
x = AnyLegalMoves(p);
p->side = tempside;
if(x == 1) return 0;
else return 1;

}

         
/* en passant packing/unpacking functions */

int IsEpSquare(short x, short y, unsigned short epstatus)
{

if(y == 5 && ((1 << (x-2)) & epstatus)) return 1;
else if(y == 6 && ((1 << (x+6)) & epstatus)) return 1;
else return 0;

}

unsigned short MakeEpSquare(short x, short y, unsigned short epstatus)
{

if(y == 6 && !(IsEpSquare(x,y,epstatus)))
  return epstatus + (1 << (x+6));
else if(y == 5 && !(IsEpSquare(x,y,epstatus)))
  return epstatus + (1 << (x-2));
else return epstatus;

}
  
unsigned short RemoveEpSquare(short x, short y, unsigned short epstatus)
{

if(y == 6 && IsEpSquare(x,y,epstatus))
  return epstatus - (1 << (x+6));
else if(y == 5 && IsEpSquare(x,y,epstatus))
  return epstatus - (1 << (x-2));
else return epstatus;

}

short Minval(short a, short b)  {
  if(a < b) return a;
  else return b;
}
 
short Maxval(short a, short b)  {
  if(a > b) return a;
  else return b;
}

int Binary(unsigned char c)  {
  if(c == 0) return 0;
  else return 1;  }

int ResolveCollision(Position *p, unsigned long key, int depth, int count)
{

unsigned long i, j, shift;
unsigned long prank[8];

for(i = 0; i < 8; i++) prank[i] = 0;

for(i = 2; i < 10; i++)  {
  for(j = 2; j < 10; j++)  {
    shift = (j-2) * 4;
    switch(p->board[i][j])  {
      case EMPTY :
        break;
      case WHITE + PAWN :
        prank[i-2] += (1uL << shift);
        break;
      case WHITE + KNIGHT :
        prank[i-2] += (1uL << (++shift));
        break;
      case WHITE + BISHOP :
        prank[i-2] += (1uL << shift);
        prank[i-2] += (1uL << (++shift));
        break;
      case WHITE + ROOK :
        shift += 2;
        prank[i-2] += (1uL << shift);
        break;
      case WHITE + QUEEN :
        prank[i-2] += (1uL << shift);
        shift += 2;
        prank[i-2] += (1uL << shift);
        break;
      case WHITE + KING :
        prank[i-2] += (1uL << (++shift));
        prank[i-2] += (1uL << (++shift));
        break;
      case BLACK + PAWN :
        prank[i-2] += (1uL << shift);
        prank[i-2] += (1uL << (++shift));
        prank[i-2] += (1uL << (++shift));
        break;
      case BLACK + KNIGHT :
        shift += 3;
        prank[i-2] += (1uL << shift);
        break;
      case BLACK + BISHOP :
        prank[i-2] += (1uL << shift);
        shift += 3;
        prank[i-2] += (1uL << shift);
        break;
      case BLACK + ROOK :
        prank[i-2] += (1uL << (++shift));
        shift += 2;
        prank[i-2] += (1uL << shift);
        break;
      case BLACK + QUEEN :
        prank[i-2] += (1uL << shift);
        prank[i-2] += (1uL << (++shift));
        shift += 2;
        prank[i-2] += (1uL << shift);
        break;
      case BLACK + KING :
        shift += 2;
        prank[i-2] += (1uL << shift);
        prank[i-2] += (1uL << (++shift));
      break;
      }
    }
  }

if(prank[0] == TransTable[key].rank[0])
if(prank[1] == TransTable[key].rank[1])
if(prank[2] == TransTable[key].rank[2])
if(prank[3] == TransTable[key].rank[3])
if(prank[4] == TransTable[key].rank[4])
if(prank[5] == TransTable[key].rank[5])
if(prank[6] == TransTable[key].rank[6])
if(prank[7] == TransTable[key].rank[7])
{
  if(depth - count <= TransTable[key].depth - TransTable[key].count) {
    return 1;   }}

return 0;

}


void AddtoTable(Position *p, int depth, int count, int val)
{

unsigned long key, i, j, shift;
unsigned long prank[8];

key = HashPosition(p);

for(i = 0; i < 8; i++) prank[i] = 0;

for(i = 2; i < 10; i++)  {
  for(j = 2; j < 10; j++)  {
    shift = (j-2) * 4;
    switch(p->board[i][j])  {
      case EMPTY :
        break;
      case WHITE + PAWN :
        prank[i-2] += (1uL << shift);
        break;
      case WHITE + KNIGHT :
        prank[i-2] += (1uL << (++shift));
        break;
      case WHITE + BISHOP :
        prank[i-2] += (1uL << shift);
        prank[i-2] += (1uL << (++shift));
        break;
      case WHITE + ROOK :
        shift += 2;
        prank[i-2] += (1uL << shift);
        break;
      case WHITE + QUEEN :
        prank[i-2] += (1uL << shift);
        shift += 2;
        prank[i-2] += (1uL << shift);
        break;
      case WHITE + KING :
        prank[i-2] += (1uL << (++shift));
        prank[i-2] += (1uL << (++shift));
        break;
      case BLACK + PAWN :
        prank[i-2] += (1uL << shift);
        prank[i-2] += (1uL << (++shift));
        prank[i-2] += (1uL << (++shift));
        break;
      case BLACK + KNIGHT :
        shift += 3;
        prank[i-2] += (1uL << shift);
        break;
      case BLACK + BISHOP :
        prank[i-2] += (1uL << shift);
        shift += 3;
        prank[i-2] += (1uL << shift);
        break;
      case BLACK + ROOK :
        prank[i-2] += (1uL << (++shift));
        shift += 2;
        prank[i-2] += (1uL << shift);
        break;
      case BLACK + QUEEN :
        prank[i-2] += (1uL << shift);
        prank[i-2] += (1uL << (++shift));
        shift += 2;
        prank[i-2] += (1uL << shift);
        break;
      case BLACK + KING :
        shift += 2;
        prank[i-2] += (1uL << shift);
        prank[i-2] += (1uL << (++shift));
      break;
      }
    }
  }

if(TransTable[key].depth != -1 && TransTable[key].depth >= depth) {
if(prank[0] == TransTable[key].rank[0])
if(prank[1] == TransTable[key].rank[1])
if(prank[2] == TransTable[key].rank[2])
if(prank[3] == TransTable[key].rank[3])
if(prank[4] == TransTable[key].rank[4])
if(prank[5] == TransTable[key].rank[5])
if(prank[6] == TransTable[key].rank[6])
if(prank[7] == TransTable[key].rank[7])
  return;
if(TransTable[key].depth - TransTable[key].count >= depth - count)
  return;
}
 
TransTable[key].depth = depth;
TransTable[key].count = count;
for(i = 0; i < 8; i++) TransTable[key].rank[i] = prank[i];
TransTable[key].value = val;

} 

void AddtoTransTable(Position *p, int count)
{

unsigned long key, i, j, shift;
unsigned long prank[8];

key = HashPosition(p);

for(i = 0; i < 8; i++) prank[i] = 0;

for(i = 2; i < 10; i++)  {
  for(j = 2; j < 10; j++)  {
    shift = (j-2) * 4;
    switch(p->board[i][j])  {
      case EMPTY :
        break;
      case WHITE + PAWN :
        prank[i-2] += (1uL << shift);
        break;
      case WHITE + KNIGHT :
        prank[i-2] += (1uL << (++shift));
        break;
      case WHITE + BISHOP :
        prank[i-2] += (1uL << shift);
        prank[i-2] += (1uL << (++shift));
        break;
      case WHITE + ROOK :
        shift += 2;
        prank[i-2] += (1uL << shift);
        break;
      case WHITE + QUEEN :
        prank[i-2] += (1uL << shift);
        shift += 2;
        prank[i-2] += (1uL << shift);
        break;
      case WHITE + KING :
        prank[i-2] += (1uL << (++shift));
        prank[i-2] += (1uL << (++shift));
        break;
      case BLACK + PAWN :
        prank[i-2] += (1uL << shift);
        prank[i-2] += (1uL << (++shift));
        prank[i-2] += (1uL << (++shift));
        break;
      case BLACK + KNIGHT :
        shift += 3;
        prank[i-2] += (1uL << shift);
        break;
      case BLACK + BISHOP :
        prank[i-2] += (1uL << shift);
        shift += 3;
        prank[i-2] += (1uL << shift);
        break;
      case BLACK + ROOK :
        prank[i-2] += (1uL << (++shift));
        shift += 2;
        prank[i-2] += (1uL << shift);
        break;
      case BLACK + QUEEN :
        prank[i-2] += (1uL << shift);
        prank[i-2] += (1uL << (++shift));
        shift += 2;
        prank[i-2] += (1uL << shift);
        break;
      case BLACK + KING :
        shift += 2;
        prank[i-2] += (1uL << shift);
        prank[i-2] += (1uL << (++shift));
      break;
      }
    }
  }

if(TransTable[key].depth != -1) {
if(prank[0] == TransTable[key].rank[0])
if(prank[1] == TransTable[key].rank[1])
if(prank[2] == TransTable[key].rank[2])
if(prank[3] == TransTable[key].rank[3])
if(prank[4] == TransTable[key].rank[4])
if(prank[5] == TransTable[key].rank[5])
if(prank[6] == TransTable[key].rank[6])
if(prank[7] == TransTable[key].rank[7]) {
  TransTable[key].depth = Minval(TransTable[key].depth, count);
  return;  }
if(count > TransTable[key].depth) return;
}
  
TransTable[key].depth = count;
TransTable[key].count = 0;
for(i = 0; i < 8; i++) TransTable[key].rank[i] = prank[i];
TransTable[key].value = 0;

}


int IsTransposition(Position *p, int count)
{

unsigned long i, j, shift, key;
unsigned long prank[8];

key = HashPosition(p);
if(TransTable[key].depth == -1) return 0;

for(i = 0; i < 8; i++) prank[i] = 0;

for(i = 2; i < 10; i++)  {
  for(j = 2; j < 10; j++)  {
    shift = (j-2) * 4;
    switch(p->board[i][j])  {
      case EMPTY :
        break;
      case WHITE + PAWN :
        prank[i-2] += (1uL << shift);
        break;
      case WHITE + KNIGHT :
        prank[i-2] += (1uL << (++shift));
        break;
      case WHITE + BISHOP :
        prank[i-2] += (1uL << shift);
        prank[i-2] += (1uL << (++shift));
        break;
      case WHITE + ROOK :
        shift += 2;
        prank[i-2] += (1uL << shift);
        break;
      case WHITE + QUEEN :
        prank[i-2] += (1uL << shift);
        shift += 2;
        prank[i-2] += (1uL << shift);
        break;
      case WHITE + KING :
        prank[i-2] += (1uL << (++shift));
        prank[i-2] += (1uL << (++shift));
        break;
      case BLACK + PAWN :
        prank[i-2] += (1uL << shift);
        prank[i-2] += (1uL << (++shift));
        prank[i-2] += (1uL << (++shift));
        break;
      case BLACK + KNIGHT :
        shift += 3;
        prank[i-2] += (1uL << shift);
        break;
      case BLACK + BISHOP :
        prank[i-2] += (1uL << shift);
        shift += 3;
        prank[i-2] += (1uL << shift);
        break;
      case BLACK + ROOK :
        prank[i-2] += (1uL << (++shift));
        shift += 2;
        prank[i-2] += (1uL << shift);
        break;
      case BLACK + QUEEN :
        prank[i-2] += (1uL << shift);
        prank[i-2] += (1uL << (++shift));
        shift += 2;
        prank[i-2] += (1uL << shift);
        break;
      case BLACK + KING :
        shift += 2;
        prank[i-2] += (1uL << shift);
        prank[i-2] += (1uL << (++shift));
      break;
      }
    }
  }

if(prank[0] == TransTable[key].rank[0])
if(prank[1] == TransTable[key].rank[1])
if(prank[2] == TransTable[key].rank[2])
if(prank[3] == TransTable[key].rank[3])
if(prank[4] == TransTable[key].rank[4])
if(prank[5] == TransTable[key].rank[5])
if(prank[6] == TransTable[key].rank[6])
if(prank[7] == TransTable[key].rank[7])
{
  if(count >= TransTable[key].depth) return 1;
  else return 2;
}

return 3;

} 
