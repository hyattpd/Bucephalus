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
#include <string.h>
#include <sys/time.h>
#include "chess.h"

int Score(Position *p, int len, int count)  {

short x, y;
int i, j;
int material, mobility, kfield, pval = 0, ghost = 0;
float fval;
unsigned char side = p->side, xside;
Listnode ml;
Move *tmp;

if(p->side == WHITE) xside = BLACK;
else xside = WHITE;

material = 0; mobility = 0; kfield = 0;

if(IsMate(p, p->side) == 1) {
  if(p->side == WHITE) return -1000000;
  else return 1000000;
}

/* check for 'galvin's ghost' tactic */

if(GHOSTON && count == 1)  {
  ClearMoves(&ml);
  GenerateMoves(&ml, p);
  i = 0; j = 0; ghost = 1;
  for(i = 0; i < MAXLEGAL; i++)  {
    if(j == ml.num) break;
    if(ghost == 0) break;
    tmp = &(ml.moves[i]);
    if(tmp->mtype == EMPTY) continue;
    MakeMove(p, tmp);
    if(IsCheck(p, xside) == 0) ghost = 0;
    UnmakeMove(p, tmp);
  }}
if(GHOSTON && ghost == 1)  {
  if(p->side == WHITE) return -900000;
  else return 900000;
}

for(i = 2; i < 10; i++)
  for(j = 2; j < 10; j++)  {
    if(p->board[i][j] != 0)  {
      if(p->board[i][j] & KNIGHT) pval = 300;
      else if(p->board[i][j] & BISHOP) pval = 350;
      else if(p->board[i][j] & ROOK) pval = 500;
      else if(p->board[i][j] & QUEEN) pval = 900;
      else if(p->board[i][j] & KING) pval = 0;
      else if((p->board[i][j] & PAWN) && (p->board[i][j] & WHITE)) pval = 100 + 10*(j-3);
      else if((p->board[i][j] & PAWN) && (p->board[i][j] & BLACK)) pval = 100 + 10*(8-j);
      if(p->board[i][j] & BLACK) pval *= -1;
      if(scoreflag == 1 && !(p->board[i][j] & scoreside)) material += pval;
      if(scoreflag == 0) material += pval;
    }}

ClearMoves(&ml);
p->side = WHITE;
GenerateLegalandIllegal(&ml, p);
for(i = 0; i < ml.num; i++)  {
  tmp = &(ml.moves[i]);
  x = tmp->source/12; y = tmp->source%12;
  mobility += 5;  }
ClearMoves(&ml);
p->side = BLACK;
GenerateLegalandIllegal(&ml, p);
for(i = 0; i < ml.num; i++)  {
  tmp = &(ml.moves[i]);
  x = tmp->source/12; y = tmp->source%12;
  mobility -= 5;  }

p->side = side;

/* king field assessment */

if(p->side == WHITE)  {
  for(i = -1; i <= 1; i++)
    for(j = -1; j <= 1; j++)  {
      if(i == 0 && j == 0) continue;  
      if(p->board[p->bkingx+i][p->bkingy+j] & BLACK)  {
        kfield += 5*len;
        continue;  }
      if(Attacked(p, p->bkingx+i, p->bkingy+j, p->side))  {
        kfield += 5*len;
        continue;  }
      if(p->board[p->bkingx+i][p->bkingy+j] == OFFBOARD) {
        kfield += 5*len;
        continue;  }
  }}
else  {
  for(i = -1; i <= 1; i++)
    for(j = -1; j <= 1; j++)  {
      if(i == 0 && j == 0) continue;
      if(p->board[p->wkingx+i][p->wkingy+j] & WHITE)  {
        kfield -= 5*len;
        continue;  }
      if(Attacked(p, p->wkingx+i, p->wkingy+j, p->side))  {
        kfield -= 5*len;
        continue;  }
      if(p->board[p->wkingx+i][p->wkingy+j] == OFFBOARD) {
        kfield -= 5*len;
        continue;  }
  }}

if(count==-1)   {
  printf("\nScore Breakdown:  Material %d  Mobility %d  Kingfield %d", material, mobility, kfield);
  fval = (material+mobility+kfield)/100.0;
  printf("\nTotal score = %.2f\n", fval);  }

return (kfield+material+mobility);

}



Eline *Minmax(Position *p, int len, int count, int ply, int lindex, int a, int b, int npval)
{

int i, j = 0, beval, alpha = a, beta = b, useless = 0;
unsigned char xside, tside = p->side;
Listnode ml, testln;
Move *tmp;
Eline *el, *etmp, *ebest = NULL;

el = (Eline *)malloc(sizeof(Eline));
memset(el, 0, sizeof(Eline));
if(el==NULL) {printf("ERROR in malloc!");fflush(stdout);exit(0);}

if(HASHON && lindex == 0) AddtoTable(p, ply, ply, Score(p, len, count));

/* we're a leaf node -- return the position's score */

if(ply == 0 || IsMate(p, p->side) || IsStalemate(p, p->side))  {
  el->next = NULL;
  el->best.mtype = EMPTY;
  el->val = Score(p, len, count);
  if(HASHON) AddtoTable(p, lindex+ply, lindex, el->val);
  return el;  }

if(p->side == WHITE)  {
  xside = BLACK;
  beval = -1000000;  }
else  {
  xside = WHITE;
  beval = 1000000;  }

/* make each legal move, evaluate node's successors */

GenerateMoves(&ml, p);
memcpy(&testln, &ml, sizeof(Listnode));
if(HASHON)   {
  SortMoves(&ml, p, lindex+ply, count, len, lindex, npval);
  if(ml.num == 0)  {
    el->val = 0;
    el->jump = 2;
    el->next = NULL;
    return el;   }
  }
if(NULLPRUNE && ply > 1) npval = Score(p, len, count);

for(i = 0; i < MAXLEGAL; i++)  {
  if(j == ml.num) break;
  tmp = &(ml.moves[i]);
  if(tmp->mtype == EMPTY) continue;
  if(DEBUGEVAL) memcpy(&CurrLine[lindex], tmp, sizeof(Move));
  MakeMove(p, tmp);

/* if end of series, switch sides, else keep current side */

  if(count == len || IsCheck(p, xside))  {
    p->side = xside;
    etmp = Minmax(p, len+1, 1, ply-1, lindex+1, alpha, beta, -1000001); 
    p->side = tside;  }
  else   {
    etmp = Minmax(p, len, count+1, ply-1, lindex+1, alpha, beta, npval);  }
  if(DEBUGEVAL) ShowCurrentLine(etmp, tmp, ply, lindex);

/* transposition.. all moves were redundant */
  if(etmp->jump == 2)  {
    UnmakeMove(p, tmp);
    ElFree(etmp);
    useless++; 
    j++;
    continue;  }

/* we're a pruned branch or a mate...return up to top of series */

  if(etmp->jump == 1)  {
    UnmakeMove(p, tmp);
    el->val = etmp->val;
    el->next = etmp;
    if(count > 1 && lindex > 0) el->jump = 1;
    else {  
      el->jump = 0;  }
    ElFree(etmp);
    if(HASHON) AddtoTable(p, lindex+ply, lindex, el->val);
    return el;  }

/* we're a max node */

  if(p->side == WHITE && etmp->val > beval)  {
    ElFree(ebest);
    beval = etmp->val;
    if(count == 1 || lindex == 0) alpha = beval;
    ebest = etmp;
    memcpy(&(el->best), tmp, sizeof(Move));
    el->val = etmp->val;
    el->next = ebest;
    if((alpha >= beta) || (alpha == 1000000))   {
      UnmakeMove(p, tmp);
      if(count > 1 && lindex > 0) el->jump = 1;
      if(HASHON) AddtoTable(p, lindex+ply, lindex, el->val);
      return el;  }
  }

/* we're a min node */

  else if(p->side == BLACK && etmp->val < beval)  {
    ElFree(ebest); 
    beval = etmp->val;
    if(count == 1 || lindex == 0) beta = beval;
    ebest = etmp;
    memcpy(&(el->best), tmp, sizeof(Move));
    el->val = etmp->val;
    el->next = ebest;
    if((beta <= alpha) || (beta == -1000000))   {
      if(count > 1 && lindex > 0) el->jump = 1;
      UnmakeMove(p, tmp);
      if(HASHON) AddtoTable(p, lindex+ply, lindex, el->val);
      return el;  }
  }
  else ElFree(etmp); 
  UnmakeMove(p, tmp);
  j++;
}

if(useless == ml.num) el->jump = 2;
if(HASHON) AddtoTable(p, lindex+ply, lindex, el->val);
return el;

}
   

void Evaluate(Position *p, int len, int count)  {

int ply, i, min;
float sec;
char line[500];
struct timeval t1, t2;
Eline *etmp, *el;

printf("\nClearing hash table...\n");
for(i = 0; i < HASHSIZE; i++)  
  TransTable[i].depth = -1;

printf("\nPly to evaluate ? : ");

fgets(line, 200, stdin); sscanf(line, "%d", &ply); 
if(ply < 1 || ply > MAXPLY)  {
  printf("\nInvalid ply value!\n");
  return;  }

printf("Score based on total material or just opponent's? (t/o) ? : ");
fgets(line, 200, stdin); 
if(line[0] == 'o')  {
  scoreflag = 1;
  printf("Side to evaluate (w/b) ? : ");
  fgets(line, 200, stdin); 
  if(line[0] == 'w' || line[0] == 'W') scoreside = WHITE;
  else scoreside = BLACK;  }
else scoreflag = 0;

el = NULL;
printf("\n\n");
for(i = 1; i <= ply; i++)  {
  ElFree(el);
  gettimeofday(&t1, NULL);
  el = Minmax(p, len, count, i, 0, -1000000, 1000000, -1000001);
  gettimeofday(&t2, NULL);
  min = (t2.tv_sec - t1.tv_sec)/60;
  sec = (t2.tv_sec - t1.tv_sec) - (min*60);
  sec += (t2.tv_usec - t1.tv_usec)/1000000.0;
if(el->val == 1000000 || el->val == -1000000)
  printf("[PLY %2d][SCORE *MATE*][TIME %d m %.2f s][LINE: ", i, min, sec);
else if(el->val == 900000 || el->val == -900000)
  printf("[PLY %2d][SCORE *GHOST*][TIME %d m %.2f s][LINE: ", i, min, sec);
else 
  printf("[PLY %2d][SCORE %5.2f][TIME %d m %.2f s][LINE: ", i, (float)el->val/100.0, min, sec);
  for(etmp = el; etmp->best.mtype != EMPTY; etmp = etmp->next) 
    PrintMoveBrief(&(etmp->best));
  printf("]\n");
}    

}



void ElFree(Eline *el)
{

Eline *tmp1, *tmp2;

if(el == NULL) return;

tmp1 = el;

while(tmp1 != NULL && tmp1->next != tmp1)  {
  tmp2 = tmp1->next;
  free(tmp1); 
  tmp1 = tmp2;
}

}


void ShowCurrentLine(Eline *el, Move *m, int ply, int lindex)
{

int i;


printf("\n[Examining: ");
for(i = 0; i < DEBUGEVAL && i < lindex+1; i++) 
  PrintMoveBrief(&CurrLine[i]);
printf("][Score: %f]", (float)el->val/100.0);
fflush(stdout);

}
