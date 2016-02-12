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

void GenerateMoves(Listnode *ml, Position *pos)
{

int i, j, x, y, direc, cancastle;
unsigned char side = pos->side, xside;
SQUARE board[12][12];

if(side == WHITE)  {
  xside = BLACK;  direc = 1;  }
else {
  xside = WHITE;  direc = -1;  }

ClearMoves(ml);
for(i = 0; i < MAXLEGAL; i++) {
  ml->moves[i].oldep = pos->epstatus;
  ml->moves[i].oldcastle = pos->castle;  }

memcpy(&(board[0][0]), &(pos->board[0][0]), sizeof(SQUARE) * 144);

for(i = 2; i < 10; i++)  {
  for(j = 2; j < 10; j++)  {
    if(board[i][j] != 0 && (board[i][j] & side))  {
      if(board[i][j] & KNIGHT)  {
        x = -1; y = -2;
	if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
          if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
          else AddMove(ml, i, j, i+x, j+y, NORMAL);
        }
        x = -1; y = 2;
        if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
          if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
          else AddMove(ml, i, j, i+x, j+y, NORMAL);
        }
        x = 1; y = -2;
	if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
          if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
          else AddMove(ml, i, j, i+x, j+y, NORMAL);
        }
      	x = 1; y = 2;
	if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
          if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
          else AddMove(ml, i, j, i+x, j+y, NORMAL);
        }
       	x = -2; y = -1;
	if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
          if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
          else AddMove(ml, i, j, i+x, j+y, NORMAL);
        }
        x = -2; y = 1;
	if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
          if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
          else AddMove(ml, i, j, i+x, j+y, NORMAL);
        }
        x = 2; y = -1;
	if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
          if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
          else AddMove(ml, i, j, i+x, j+y, NORMAL);
        }
        x = 2; y = 1;
	if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
          if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
          else AddMove(ml, i, j, i+x, j+y, NORMAL);
        }
      }
      if((board[i][j] & BISHOP) || (board[i][j] & QUEEN))  {
        for(x = 1; (board[i-x][j-x] != OFFBOARD) && (!(board[i-x][j-x] & side)); x++)
          if(board[i-x][j-x] & xside)  {  
            AddMove(ml, i, j, i-x, j-x, CAPTURE);
            break;  }
	  else AddMove(ml, i, j, i-x, j-x, NORMAL);
        for(x = 1; (board[i-x][j+x] != OFFBOARD) && (!(board[i-x][j+x] & side)); x++)
          if(board[i-x][j+x] & xside)  {  
            AddMove(ml, i, j, i-x, j+x, CAPTURE);
            break;  }
	  else AddMove(ml, i, j, i-x, j+x, NORMAL);
        for(x = 1; (board[i+x][j-x] != OFFBOARD) && (!(board[i+x][j-x] & side)); x++)
          if(board[i+x][j-x] & xside)  {  
            AddMove(ml, i, j, i+x, j-x, CAPTURE);
            break;  }
	  else AddMove(ml, i, j, i+x, j-x, NORMAL);
        for(x = 1; (board[i+x][j+x] != OFFBOARD) && (!(board[i+x][j+x] & side)); x++)
          if(board[i+x][j+x] & xside)  {  
            AddMove(ml, i, j, i+x, j+x, CAPTURE);
            break;  }
	  else AddMove(ml, i, j, i+x, j+x, NORMAL);
        }
      if((board[i][j] & ROOK) || (board[i][j] & QUEEN))  {
        for(x = 1; (board[i-x][j] != OFFBOARD) && (!(board[i-x][j] & side)); x++)
          if(board[i-x][j] & xside) {
            AddMove(ml, i, j, i-x, j, CAPTURE);
            break;  }
          else AddMove(ml, i, j, i-x, j, NORMAL);
        for(x = 1; (board[i+x][j] != OFFBOARD) && (!(board[i+x][j] & side)); x++)
          if(board[i+x][j] & xside) {
            AddMove(ml, i, j, i+x, j, CAPTURE);
            break;  }
          else AddMove(ml, i, j, i+x, j, NORMAL);
        for(x = 1; (board[i][j-x] != OFFBOARD) && (!(board[i][j-x] & side)); x++)
          if(board[i][j-x] & xside) {
            AddMove(ml, i, j, i, j-x, CAPTURE);
            break;  }
          else AddMove(ml, i, j, i, j-x, NORMAL);
        for(x = 1; (board[i][j+x] != OFFBOARD) && (!(board[i][j+x] & side)); x++)
          if(board[i][j+x] & xside) {
            AddMove(ml, i, j, i, j+x, CAPTURE);
            break;  }
          else AddMove(ml, i, j, i, j+x, NORMAL);
        }
      if(board[i][j] & KING)  {
        for(x = -1; x <= 1; x++)
          for(y = -1; y <= 1; y++)  {
            if(x == 0 && y == 0) continue;
            if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
              if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
              else AddMove(ml, i, j, i+x, j+y, NORMAL);
            }
          }
        }
      if(board[i][j] & PAWN)  {
        if(board[i][j+direc] == 0) {
          if((direc == -1 && j == 3) || (direc == 1 && j == 8))
            AddMove(ml, i, j, i, j+direc, PROMOTE);
          else AddMove(ml, i, j, i, j+direc, NORMAL);
        }
        if((direc == -1 && j == 8) || (direc == 1 && j == 3))
          if(board[i][j+direc+direc] == 0 && board[i][j+direc] == 0) 
            AddMove(ml, i, j, i, j+direc+direc, TWOMOVE);
        if((board[i-1][j+direc] != OFFBOARD) && (board[i-1][j+direc] & xside)) {
          if((direc == -1 && j == 3) || (direc == 1 && j == 8))
            AddMove(ml, i, j, i-1, j+direc, PROMCAP);
          else AddMove(ml, i, j, i-1, j+direc, CAPTURE);      
        }
        if((board[i+1][j+direc] != OFFBOARD) && (board[i+1][j+direc] & xside)) {
          if((direc == -1 && j == 3) || (direc == 1 && j == 8))
            AddMove(ml, i, j, i+1, j+direc, PROMCAP);
          else AddMove(ml, i, j, i+1, j+direc, CAPTURE); 
        }
        /* there is at least one ep square */  
        if(pos->epstatus > 0 && direc == 1) {
          if(j == 6 && IsEpSquare(i-1, 6, pos->epstatus) == 1 && board[i-1][j+direc] == 0)
            AddMove(ml, i, j, i-1, j+direc, ENPASSANT);
          if(j == 6 && IsEpSquare(i+1, 6, pos->epstatus) == 1 && board[i+1][j+direc] == 0)
            AddMove(ml, i, j, i+1, j+direc, ENPASSANT);
        }

        if(pos->epstatus > 0 && direc == -1)  {
          if(j == 5 && IsEpSquare(i-1, 5, pos->epstatus) == 1 && board[i-1][j+direc] == 0)
            AddMove(ml, i, j, i-1, j+direc, ENPASSANT);
          if(j == 5 && IsEpSquare(i+1, 5, pos->epstatus) == 1 && board[i+1][j+direc] == 0)
            AddMove(ml, i, j, i+1, j+direc, ENPASSANT);   
        }
      }
    }
  }
}

if((pos->castle & WKC) && side == WHITE)  {
  cancastle = 1;
  for(i = KFILE; i <= 8; i++) 
    if(Attacked(pos, i, 2, xside) == 1) cancastle = 0;
  for(i = Minval(7, KFILE); i <= 8; i++)
    if(i != KFILE && i != KRFILE && board[i][2] != 0) cancastle = 0;
 if(cancastle == 1)
    AddMove(ml, KFILE, 2, 8, 2, KSCASTLE);  }
if((pos->castle & BKC) && side == BLACK)  {
  cancastle = 1;
  for(i = KFILE; i <= 8; i++) 
    if(Attacked(pos, i, 9, xside) == 1) cancastle = 0;
  for(i = Minval(7, KFILE); i <= 8; i++)
    if(i != KFILE && i != KRFILE && board[i][9] != 0) cancastle = 0;
 if(cancastle == 1)
    AddMove(ml, KFILE, 9, 8, 9, KSCASTLE);  }
if((pos->castle & WQC) && side == WHITE)  {
  cancastle = 1;
  for(i = Minval(4, KFILE); i <= Maxval(4, KFILE); i++) 
    if(Attacked(pos, i, 2, xside) == 1) cancastle = 0;
  for(i = Minval(QRFILE + 1, 4); i <= Maxval(5, KFILE); i++)
    if(i != KFILE && i != QRFILE && board[i][2] != 0) cancastle = 0;
 if(cancastle == 1)
    AddMove(ml, KFILE, 2, 4, 2, QSCASTLE);  }
if((pos->castle & BQC) && side == BLACK)  {
  cancastle = 1;
  for(i = Minval(4, KFILE); i <= Maxval(4, KFILE); i++) 
    if(Attacked(pos, i, 9, xside) == 1) cancastle = 0;
  for(i = Minval(QRFILE + 1, 4); i <= Maxval(5, KFILE); i++)
    if(i != KFILE && i != QRFILE && board[i][9] != 0) cancastle = 0;
 if(cancastle == 1)
    AddMove(ml, KFILE, 9, 4, 9, QSCASTLE);  }

for(i = 0; ml->moves[i].mtype != EMPTY; i++)  {
  MakeMove(pos, &(ml->moves[i]));
  if(IsCheck(pos, side) == 1)  {
    UnmakeMove(pos, &(ml->moves[i]));
    ml->moves[i].mtype = EMPTY;
    ml->num--;  }
  else UnmakeMove(pos, &(ml->moves[i]));
  }

}






int Attacked(Position *pos, int x, int y, unsigned char side)
{

int i, j, direc;
SQUARE board[12][12];

memcpy(&(board[0][0]), &(pos->board[0][0]), sizeof(SQUARE) * 144);

if(side == WHITE)  {
  direc = 1;  }
else {
  direc = -1;  }

/* check pawns first */

if((board[x-1][y-direc] != OFFBOARD) && (board[x-1][y-direc] & side) && (board[x-1][y-direc] & PAWN))
  return 1;
if((board[x+1][y-direc] != OFFBOARD) && (board[x+1][y-direc] & side) && (board[x+1][y-direc] & PAWN))
  return 1;

/* check knight squares */

if((board[x-1][y-2] != OFFBOARD) && (board[x-1][y-2] & side) && (board[x-1][y-2] & KNIGHT))
  return 1;
if((board[x+1][y-2] != OFFBOARD) && (board[x+1][y-2] & side) && (board[x+1][y-2] & KNIGHT))
  return 1;
if((board[x-1][y+2] != OFFBOARD) && (board[x-1][y+2] & side) && (board[x-1][y+2] & KNIGHT))
  return 1;
if((board[x+1][y+2] != OFFBOARD) && (board[x+1][y+2] & side) && (board[x+1][y+2] & KNIGHT))
  return 1;
if((board[x-2][y-1] != OFFBOARD) && (board[x-2][y-1] & side) && (board[x-2][y-1] & KNIGHT))
  return 1;
if((board[x+2][y-1] != OFFBOARD) && (board[x+2][y-1] & side) && (board[x+2][y-1] & KNIGHT))
  return 1;
if((board[x-2][y+1] != OFFBOARD) && (board[x-2][y+1] & side) && (board[x-2][y+1] & KNIGHT))
  return 1;
if((board[x+2][y+1] != OFFBOARD) && (board[x+2][y+1] & side) && (board[x+2][y+1] & KNIGHT))
  return 1;

/* check king squares */

for(i = -1; i <=1; i++)
  for(j = -1; j <=1; j++)  {
    if(i == 0 && j == 0) continue;
    if((board[x+i][y+j] != OFFBOARD) && (board[x+i][y+j] & side) && (board[x+i][y+j] & KING))
      return 1;
    }

/* check diagonals */

for(i = 1; board[x-i][y-i] == 0; i++);
if((board[x-i][y-i] != OFFBOARD) && (board[x-i][y-i] & side) && (board[x-i][y-i] & (QUEEN | BISHOP)))
  return 1;
for(i = 1; board[x-i][y+i] == 0; i++);
if((board[x-i][y+i] != OFFBOARD) && (board[x-i][y+i] & side) && (board[x-i][y+i] & (QUEEN | BISHOP)))
  return 1;
for(i = 1; board[x+i][y-i] == 0; i++);
if((board[x+i][y-i] != OFFBOARD) && (board[x+i][y-i] & side) && (board[x+i][y-i] & (QUEEN | BISHOP)))
  return 1;
for(i = 1; board[x+i][y+i] == 0; i++);
if((board[x+i][y+i] != OFFBOARD) && (board[x+i][y+i] & side) && (board[x+i][y+i] & (QUEEN | BISHOP)))
  return 1;

/* check ranks and files */

for(i = 1; board[x-i][y] == 0; i++);
if((board[x-i][y] != OFFBOARD) && (board[x-i][y] & side) && (board[x-i][y] & (QUEEN | ROOK)))
  return 1;
for(i = 1; board[x+i][y] == 0; i++);
if((board[x+i][y] != OFFBOARD) && (board[x+i][y] & side) && (board[x+i][y] & (QUEEN | ROOK)))
  return 1;
for(i = 1; board[x][y-i] == 0; i++);
if((board[x][y-i] != OFFBOARD) && (board[x][y-i] & side) && (board[x][y-i] & (QUEEN | ROOK)))
  return 1;
for(i = 1; board[x][y+i] == 0; i++);
if((board[x][y+i] != OFFBOARD) && (board[x][y+i] & side) && (board[x][y+i] & (QUEEN | ROOK)))
  return 1;

return 0;

}

       
int AnyLegalMoves(Position *pos)  {

int i, j, x, y, direc;
unsigned char side = pos->side, xside;
SQUARE board[12][12];
Listnode l, *ml = &l;

ClearMoves(ml);
for(i = 0; i < MAXLEGAL; i++) {
  ml->moves[i].oldep = pos->epstatus;
  ml->moves[i].oldcastle = pos->castle;  }

if(side == WHITE)  {
  xside = BLACK;  direc = 1;  }
else {
  xside = WHITE;  direc = -1;  }

memcpy(&(board[0][0]), &(pos->board[0][0]), sizeof(SQUARE) * 144);

for(i = 2; i < 10; i++)
  for(j = 2; j < 10; j++)  {
    if(board[i][j] != 0 && (board[i][j] & side))  {
      if(board[i][j] & KNIGHT)  {
        x = -1; y = -2;
	if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
          if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
          else AddMove(ml, i, j, i+x, j+y, NORMAL);
        }
        x = -1; y = 2;
        if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
          if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
          else AddMove(ml, i, j, i+x, j+y, NORMAL);
        }
        x = 1; y = -2;
	if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
          if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
          else AddMove(ml, i, j, i+x, j+y, NORMAL);
        }
      	x = 1; y = 2;
	if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
          if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
          else AddMove(ml, i, j, i+x, j+y, NORMAL);
        }
       	x = -2; y = -1;
	if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
          if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
          else AddMove(ml, i, j, i+x, j+y, NORMAL);
        }
        x = -2; y = 1;
	if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
          if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
          else AddMove(ml, i, j, i+x, j+y, NORMAL);
        }
        x = 2; y = -1;
	if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
          if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
          else AddMove(ml, i, j, i+x, j+y, NORMAL);
        }
        x = 2; y = 1;
	if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
          if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
          else AddMove(ml, i, j, i+x, j+y, NORMAL);
        }
      }
      if((board[i][j] & BISHOP) || (board[i][j] & QUEEN))  {
        for(x = 1; (board[i-x][j-x] != OFFBOARD) && (!(board[i-x][j-x] & side)); x++)
          if(board[i-x][j-x] & xside)  {  
            AddMove(ml, i, j, i-x, j-x, CAPTURE);
            break;  }
	  else AddMove(ml, i, j, i-x, j-x, NORMAL);
        for(x = 1; (board[i-x][j+x] != OFFBOARD) && (!(board[i-x][j+x] & side)); x++)
          if(board[i-x][j+x] & xside)  {  
            AddMove(ml, i, j, i-x, j+x, CAPTURE);
            break;  }
	  else AddMove(ml, i, j, i-x, j+x, NORMAL);
        for(x = 1; (board[i+x][j-x] != OFFBOARD) && (!(board[i+x][j-x] & side)); x++)
          if(board[i+x][j-x] & xside)  {  
            AddMove(ml, i, j, i+x, j-x, CAPTURE);
            break;  }
	  else AddMove(ml, i, j, i+x, j-x, NORMAL);
        for(x = 1; (board[i+x][j+x] != OFFBOARD) && (!(board[i+x][j+x] & side)); x++)
          if(board[i+x][j+x] & xside)  {  
            AddMove(ml, i, j, i+x, j+x, CAPTURE);
            break;  }
	  else AddMove(ml, i, j, i+x, j+x, NORMAL);
      }
      if((board[i][j] & ROOK) || (board[i][j] & QUEEN))  {
        for(x = 1; (board[i-x][j] != OFFBOARD) && (!(board[i-x][j] & side)); x++)
          if(board[i-x][j] & xside) {
            AddMove(ml, i, j, i-x, j, CAPTURE);
            break;  }
          else AddMove(ml, i, j, i-x, j, NORMAL);
        for(x = 1; (board[i+x][j] != OFFBOARD) && (!(board[i+x][j] & side)); x++)
          if(board[i+x][j] & xside) {
            AddMove(ml, i, j, i+x, j, CAPTURE);
            break;  }
          else AddMove(ml, i, j, i+x, j, NORMAL);
        for(x = 1; (board[i][j-x] != OFFBOARD) && (!(board[i][j-x] & side)); x++)
          if(board[i][j-x] & xside) {
            AddMove(ml, i, j, i, j-x, CAPTURE);
            break;  }
          else AddMove(ml, i, j, i, j-x, NORMAL);
        for(x = 1; (board[i][j+x] != OFFBOARD) && (!(board[i][j+x] & side)); x++)
          if(board[i][j+x] & xside) {
            AddMove(ml, i, j, i, j+x, CAPTURE);
            break;  }
          else AddMove(ml, i, j, i, j+x, NORMAL);
      }
      if(board[i][j] & KING)  {
        for(x = -1; x <= 1; x++)
          for(y = -1; y <= 1; y++)  {
            if(x == 0 && y == 0) continue;
            if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
              if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
              else AddMove(ml, i, j, i+x, j+y, NORMAL);
            }
          }
        }
      if(board[i][j] & PAWN)  {
        if(board[i][j+direc] == 0) {
          if((direc == -1 && j == 3) || (direc == 1 && j == 8))
            AddMove(ml, i, j, i, j+direc, PROMOTE);
          else AddMove(ml, i, j, i, j+direc, NORMAL);
        }
        if((direc == -1 && j == 8) || (direc == 1 && j == 3)) 
          if(board[i][j+direc+direc] == 0 && board[i][j+direc] == 0) 
            AddMove(ml, i, j, i, j+direc+direc, TWOMOVE);
        if((board[i-1][j+direc] != OFFBOARD) && (board[i-1][j+direc] & xside)) {
          if((direc == -1 && j == 3) || (direc == 1 && j == 8))
            AddMove(ml, i, j, i-1, j+direc, PROMCAP);
          else AddMove(ml, i, j, i-1, j+direc, CAPTURE);      
        }
        if((board[i+1][j+direc] != OFFBOARD) && (board[i+1][j+direc] & xside)) {
          if((direc == -1 && j == 3) || (direc == 1 && j == 8))
            AddMove(ml, i, j, i+1, j+direc, PROMCAP);
          else AddMove(ml, i, j, i+1, j+direc, CAPTURE); 
        }
        if(pos->epstatus > 0 && direc == 1)  /* there is at least one ep square */  {
          if(j == 6 && IsEpSquare(i-1, 6, pos->epstatus) == 1 && board[i-1][j+direc] == 0)
            AddMove(ml, i, j, i-1, j+direc, ENPASSANT);
          if(j == 6 && IsEpSquare(i+1, 6, pos->epstatus) == 1 && board[i+1][j+direc] == 0)
            AddMove(ml, i, j, i+1, j+direc, ENPASSANT);
        }

        if(pos->epstatus > 0 && direc == -1)  {
          if(j == 5 && IsEpSquare(i-1, 5, pos->epstatus) == 1 && board[i-1][j+direc] == 0)
            AddMove(ml, i, j, i-1, j+direc, ENPASSANT);
          if(j == 5 && IsEpSquare(i+1, 5, pos->epstatus) == 1 && board[i+1][j+direc] == 0)
            AddMove(ml, i, j, i+1, j+direc, ENPASSANT);   
         }

      }

      for(x = 0; ml->moves[x].mtype != EMPTY; x++)  {
        MakeMove(pos, &(ml->moves[x]));
        if(IsCheck(pos, side) == 1)  {
          UnmakeMove(pos, &(ml->moves[x]));
          ml->moves[x].mtype = EMPTY;
          ml->num--;  }
        else UnmakeMove(pos, &(ml->moves[x]));
        }

        if(ml->num > 0) {   return 1;  }

      }
    }

return 0;

}



void GenerateLegalandIllegal(Listnode *ml, Position *pos)
{

int i, j, x, y, direc;
unsigned char side = pos->side, xside;
SQUARE board[12][12];

if(side == WHITE)  {
  xside = BLACK;  direc = 1;  }
else {
  xside = WHITE;  direc = -1;  }

ClearMoves(ml);
for(i = 0; i < MAXLEGAL; i++) {
  ml->moves[i].oldep = pos->epstatus;
  ml->moves[i].oldcastle = pos->castle;  }

memcpy(&(board[0][0]), &(pos->board[0][0]), sizeof(SQUARE) * 144);

for(i = 2; i < 10; i++)
  for(j = 2; j < 10; j++)  {
    if(board[i][j] != 0 && (board[i][j] & side))  {
      if(board[i][j] & KNIGHT)  {
        x = -1; y = -2;
	if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
          if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
          else AddMove(ml, i, j, i+x, j+y, NORMAL);
        }
        x = -1; y = 2;
        if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
          if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
          else AddMove(ml, i, j, i+x, j+y, NORMAL);
        }
        x = 1; y = -2;
	if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
          if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
          else AddMove(ml, i, j, i+x, j+y, NORMAL);
        }
      	x = 1; y = 2;
	if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
          if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
          else AddMove(ml, i, j, i+x, j+y, NORMAL);
        }
       	x = -2; y = -1;
	if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
          if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
          else AddMove(ml, i, j, i+x, j+y, NORMAL);
        }
        x = -2; y = 1;
	if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
          if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
          else AddMove(ml, i, j, i+x, j+y, NORMAL);
        }
        x = 2; y = -1;
	if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
          if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
          else AddMove(ml, i, j, i+x, j+y, NORMAL);
        }
        x = 2; y = 1;
	if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
          if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
          else AddMove(ml, i, j, i+x, j+y, NORMAL);
        }
      }
      if((board[i][j] & BISHOP) || (board[i][j] & QUEEN))  {
        for(x = 1; (board[i-x][j-x] != OFFBOARD) && (!(board[i-x][j-x] & side)); x++)
          if(board[i-x][j-x] & xside)  {  
            AddMove(ml, i, j, i-x, j-x, CAPTURE);
            break;  }
	  else AddMove(ml, i, j, i-x, j-x, NORMAL);
        for(x = 1; (board[i-x][j+x] != OFFBOARD) && (!(board[i-x][j+x] & side)); x++)
          if(board[i-x][j+x] & xside)  {  
            AddMove(ml, i, j, i-x, j+x, CAPTURE);
            break;  }
	  else AddMove(ml, i, j, i-x, j+x, NORMAL);
        for(x = 1; (board[i+x][j-x] != OFFBOARD) && (!(board[i+x][j-x] & side)); x++)
          if(board[i+x][j-x] & xside)  {  
            AddMove(ml, i, j, i+x, j-x, CAPTURE);
            break;  }
	  else AddMove(ml, i, j, i+x, j-x, NORMAL);
        for(x = 1; (board[i+x][j+x] != OFFBOARD) && (!(board[i+x][j+x] & side)); x++)
          if(board[i+x][j+x] & xside)  {  
            AddMove(ml, i, j, i+x, j+x, CAPTURE);
            break;  }
	  else AddMove(ml, i, j, i+x, j+x, NORMAL);
      }
      if((board[i][j] & ROOK) || (board[i][j] & QUEEN))  {
        for(x = 1; (board[i-x][j] != OFFBOARD) && (!(board[i-x][j] & side)); x++)
          if(board[i-x][j] & xside) {
            AddMove(ml, i, j, i-x, j, CAPTURE);
            break;  }
          else AddMove(ml, i, j, i-x, j, NORMAL);
        for(x = 1; (board[i+x][j] != OFFBOARD) && (!(board[i+x][j] & side)); x++)
          if(board[i+x][j] & xside) {
            AddMove(ml, i, j, i+x, j, CAPTURE);
            break;  }
          else AddMove(ml, i, j, i+x, j, NORMAL);
        for(x = 1; (board[i][j-x] != OFFBOARD) && (!(board[i][j-x] & side)); x++)
          if(board[i][j-x] & xside) {
            AddMove(ml, i, j, i, j-x, CAPTURE);
            break;  }
          else AddMove(ml, i, j, i, j-x, NORMAL);
        for(x = 1; (board[i][j+x] != OFFBOARD) && (!(board[i][j+x] & side)); x++)
          if(board[i][j+x] & xside) {
            AddMove(ml, i, j, i, j+x, CAPTURE);
            break;  }
          else AddMove(ml, i, j, i, j+x, NORMAL);
      }
      if(board[i][j] & KING)  {
        for(x = -1; x <= 1; x++)  {
          for(y = -1; y <= 1; y++)  {
            if(x == 0 && y == 0) continue;
            if((board[i+x][j+y] != OFFBOARD) && (!(board[i+x][j+y] & side))) {
              if(board[i+x][j+y] & xside)  AddMove(ml, i, j, i+x, j+y, CAPTURE);
              else AddMove(ml, i, j, i+x, j+y, NORMAL);
            }
          }
        }
      }
      if(board[i][j] & PAWN)  {
        if(board[i][j+direc] == 0) {
          if((direc == -1 && j == 3) || (direc == 1 && j == 8))
            AddMove(ml, i, j, i, j+direc, PROMOTE);
          else AddMove(ml, i, j, i, j+direc, NORMAL);
        }
        if((direc == -1 && j == 8) || (direc == 1 && j == 3))
          if(board[i][j+direc+direc] == 0 && board[i][j+direc] == 0) 
            AddMove(ml, i, j, i, j+direc+direc, TWOMOVE);
        if((board[i-1][j+direc] != OFFBOARD) && (board[i-1][j+direc] & xside)) {
          if((direc == -1 && j == 3) || (direc == 1 && j == 8))
            AddMove(ml, i, j, i-1, j+direc, PROMCAP);
          else AddMove(ml, i, j, i-1, j+direc, CAPTURE);      
        }
        if((board[i+1][j+direc] != OFFBOARD) && (board[i+1][j+direc] & xside)) {
          if((direc == -1 && j == 3) || (direc == 1 && j == 8))
            AddMove(ml, i, j, i+1, j+direc, PROMCAP);
          else AddMove(ml, i, j, i+1, j+direc, CAPTURE); 
        }
        if(pos->epstatus > 0 && direc == 1)  /* there is at least one ep square */  {
          if(j == 6 && IsEpSquare(i-1, 6, pos->epstatus) == 1 && board[i-1][j+direc] == 0)
            AddMove(ml, i, j, i-1, j+direc, ENPASSANT);
          if(j == 6 && IsEpSquare(i+1, 6, pos->epstatus) == 1 && board[i+1][j+direc] == 0)
            AddMove(ml, i, j, i+1, j+direc, ENPASSANT);
        }

        if(pos->epstatus > 0 && direc == -1)  {
          if(j == 5 && IsEpSquare(i-1, 5, pos->epstatus) == 1 && board[i-1][j+direc] == 0)
            AddMove(ml, i, j, i-1, j+direc, ENPASSANT);
          if(j == 5 && IsEpSquare(i+1, 5, pos->epstatus) == 1 && board[i+1][j+direc] == 0)
            AddMove(ml, i, j, i+1, j+direc, ENPASSANT);   
        }
      }
    }
  }

}
