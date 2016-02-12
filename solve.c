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
#include <sys/time.h>
#include "chess.h"

void Solve(Position *p)  {

int len, ghost, i, min, sec;
char line[500];
struct timeval t1, t2;
Position temp;

memcpy(&temp, p, sizeof(Position));

printf("\nEnter number of moves to search: ");
fgets(line, 200, stdin); sscanf(line, "%d", &len); 
if(len < 1 || len > 20)  {
  printf("\nInvalid number of moves entered.\n");
  return;  }

printf("Enter side to move (w/b): ");
fgets(line, 200, stdin);
if(line[0] == 'w' || line[0] == 'W') temp.side = WHITE;
else temp.side = BLACK;

printf("Look for ghosts? (y/n): ");
fgets(line, 200, stdin);
if(line[0] == 'y' || line[0] == 'Y') ghost = 1;
else ghost = 0;

printf("Clearing transposition table...\n");
for(i = 0; i < HASHSIZE; i++) TransTable[i].depth = -1;

printf("Solving...\n");

gettimeofday(&t1, NULL);
MateSolve(&temp, len, ghost, 0);
gettimeofday(&t2, NULL);
min = (t2.tv_sec - t1.tv_sec)/60;
sec = (t2.tv_sec - t1.tv_sec) - (min*60);
sec += (int)((t2.tv_usec - t1.tv_usec)/1000000.0);
printf("\nTime elapsed = %d min %d sec\n", min, sec); 

return;

} 



int MateSolve(Position *p, int len, int ghost, int count)
{

int i, j, k, val, isghost;
unsigned char xside, tmpside;
char line[500];
Listnode ml;
Move *tmp;

if(HASHON) AddtoTransTable(p, count);

if(p->side == WHITE) xside = BLACK;
else xside = WHITE;

if(IsCheck(p, xside))  {
  tmpside = p->side;
  p->side = xside;
  GenerateMoves(&ml, p);
  p->side = tmpside;
  if(ml.num == 0) return 1;  /* mate */
  else if(ghost == 1)  {
    i = 0; j = 0; isghost = 1;
    for(i = 0; i < MAXLEGAL; i++)  {
      if(j == ml.num) break;
      if(ghost == 0) break;
      tmp = &(ml.moves[i]);
      if(tmp->mtype == EMPTY) continue;
      MakeMove(p, tmp);
      if(IsCheck(p, p->side) == 0) isghost = 0;
      UnmakeMove(p, tmp);
    }
    if(isghost == 1) return 2;  /* ghost */
  }
return 0;  /* leaf with no ghost or mate */  
}

if(len == 0) return 0;  /* leaf with no check */

GenerateMoves(&ml, p);
j = 0;
for(i = 0; i < MAXLEGAL; i++)  {
  if(j == ml.num) break;
  tmp = &(ml.moves[i]);
  if(tmp->mtype == EMPTY) continue;
  memcpy(&CurrLine[count], tmp, sizeof(Move));
  MakeMove(p, tmp);
  if(HASHON && IsTransposition(p, count+1) == 1)  {
/*    for(k = 0; k < count + 1; k++) PrintMoveBrief(&CurrLine[k]);
    printf("[%d]", IsTransposition(p, count+1));fflush(stdout);  */
    UnmakeMove(p, tmp);
    j++;  continue;    } 
  val = MateSolve(p, len - 1, ghost, count + 1);
  if(val == 3) {
    UnmakeMove(p, tmp);
    return 3;  }
  if(val == 1 || val == 2)  {
    if(val == 1) printf("\nMATE FOUND--> ");
    else printf("\nGHOST FOUND--> ");
    for(k = 0; k < count+1; k++)
      PrintMoveBrief(&CurrLine[k]);
    printf("\nContinue the search? (y/n) ");
    fgets(line, 200, stdin);
    if(line[0] == 'n' || line[0] == 'N') {
      UnmakeMove(p, tmp);
      return 3;   }
       }
  UnmakeMove(p, tmp);
  j++;
}

return 0;

} 

   

void Find(Position *p)
{

int len, i, min, sec;
int minmaterialreq, maxmaterialreq, mustendwithcheck, mustntgetmated;
int valpawn = 1, valknight = 3, valbishop = 3, valrook = 5, valqueen = 9;
char line[500];
struct timeval t1, t2;
Position temp;

memcpy(&temp, p, sizeof(Position));

printf("\nEnter number of moves to search: ");
fgets(line, 200, stdin); sscanf(line, "%d", &len); 
if(len < 1 || len > 20)  {
  printf("\nInvalid number of moves entered.\n");
  return;  }

printf("Enter side to move (w/b): ");
fgets(line, 200,stdin); 
if(line[0] == 'w' || line[0] == 'W') temp.side = WHITE;
else temp.side = BLACK;

printf("----*Requirements For Series*----\n");

printf("Must eat material (y/n)?: ");
fgets(line, 200, stdin); 
if(line[0] == 'y' || line[0] == 'Y')  {
  printf("Change the default piece values (y/n)?: ");
  fgets(line, 200, stdin);
  if(line[0] == 'y' || line[0] == 'Y')  {
    printf("Value for pawn (Default = 1) : ");
    fgets(line, 200, stdin); sscanf(line, "%d", &valpawn); 
    if(valpawn < 0 || valpawn > 1000) valpawn = 1;
    printf("Value for knight (Default = 3) : ");
    fgets(line, 200, stdin); sscanf(line, "%d", &valknight); 
    if(valknight < 0 || valknight > 1000) valknight = 1;
    printf("Value for bishop (Default = 3) : ");
    fgets(line, 200, stdin); sscanf(line, "%d", &valbishop); 
    if(valbishop < 0 || valbishop > 1000) valbishop = 1;
    printf("Value for rook (Default = 5) : ");
    fgets(line, 200, stdin); sscanf(line, "%d", &valrook); 
    if(valrook < 0 || valrook > 1000) valrook = 1;
    printf("Value for queen (Default = 9) : ");
    fgets(line, 200, stdin); sscanf(line, "%d", &valqueen); 
    if(valqueen < 0 || valqueen > 1000) valqueen = 1;  }
  printf("Min material req(%d=P/%d=N/%d=B/%d=R/%d=Q/Enter 0 for none) ? : ",
    valpawn, valknight, valbishop, valrook, valqueen);
  fgets(line, 200, stdin); sscanf(line, "%d", &minmaterialreq); 
  if(minmaterialreq == 0) minmaterialreq = -1;
  printf("Max material req(%d=P/%d=N/%d=B/%d=R/%d=Q/Enter 0 for none) ? : ",
    valpawn, valknight, valbishop, valrook, valqueen);
  fgets(line, 200, stdin); sscanf(line, "%d", &maxmaterialreq); 
  if(maxmaterialreq == 0) maxmaterialreq = 10001;
  }
else { 
  minmaterialreq = -1;
  maxmaterialreq = 10001;  }

printf("Must end with check (y/n): ");
fgets(line, 200, stdin); 
if(line[0] == 'y' || line[0] == 'Y') mustendwithcheck = 1;
else mustendwithcheck = 0;

printf("Must NOT get mated or ghosted (y/n): ");
fgets(line, 200, stdin); 
if(line[0] == 'y' || line[0] == 'Y') {
  printf("Length to check for mate/ghost : ");
  fgets(line, 200, stdin); sscanf(line, "%d", &mustntgetmated); 
  if(mustntgetmated < 0 || mustntgetmated > MAXPLY)
    mustntgetmated = 0;  }
else mustntgetmated = 0;

printf("Clearing transposition table...\n");
for(i = 0; i < HASHSIZE; i++) TransTable[i].depth = -1;

printf("Solving...\n");

gettimeofday(&t1, NULL);
FindSeries(&temp, len, 0, minmaterialreq, maxmaterialreq, mustendwithcheck, mustntgetmated, 0, 
  valpawn, valknight, valbishop, valrook, valqueen);
gettimeofday(&t2, NULL);
min = (t2.tv_sec - t1.tv_sec)/60;
sec = (t2.tv_sec - t1.tv_sec) - (min*60);
sec += (int)((t2.tv_usec - t1.tv_usec)/1000000.0);
printf("\nTime elapsed = %d min %d sec\n", min, sec); 

return;

}
 


int FindSeries(Position *p, int len, int count, int lmrq, int umrq, int mec, int mgm, int eaten, 
  int valpawn, int valknight, int valbishop, int valrook, int valqueen)
{
 
int i, j, k, val, meetsreq;
unsigned char xside, tmpside;
Listnode ml;
Move *tmp;
 
if(HASHON) AddtoTransTable(p, count);
 
if(p->side == WHITE) xside = BLACK;
else xside = WHITE;
 
if(len == 0 || IsCheck(p, xside))  {  /* leaf node--evaluate */
  meetsreq = 1;
  if(eaten < lmrq) meetsreq = 0;
  if(eaten > umrq) meetsreq = 0;
  if(mec == 1 && !IsCheck(p, xside)) meetsreq = 0;
  if(meetsreq == 1 && mgm > 0)  {
    tmpside = p->side;
    p->side = xside;
/*    printf("\nChecking "); 
    for(i = 0; i < count; i++) PrintMoveBrief(&CurrLine[i]);
    printf("for mate/ghost... "); */
    if(IsMatedGhosted(p, mgm, 0) > 0) meetsreq = 0;  
    p->side = tmpside;   }
  return meetsreq; 
}
 
GenerateMoves(&ml, p);
j = 0;
for(i = 0; i < MAXLEGAL; i++)  {
  if(j == ml.num) break;
  tmp = &(ml.moves[i]);
  if(tmp->mtype == EMPTY) continue;
  memcpy(&CurrLine[count], tmp, sizeof(Move));
  MakeMove(p, tmp);
  if(HASHON && IsTransposition(p, count+1) == 1)  {
/*    for(k = 0; k < count + 1; k++) PrintMoveBrief(&CurrLine[k]);
    printf("[%d]", IsTransposition(p, count+1));fflush(stdout);  */
    UnmakeMove(p, tmp);
    j++;  continue;    }
  if(tmp->oldpiece & xside)  {
    if(tmp->oldpiece & QUEEN) eaten+=valqueen;
    if(tmp->oldpiece & ROOK) eaten+=valrook;
    if(tmp->oldpiece & BISHOP) eaten+=valbishop;
    if(tmp->oldpiece & KNIGHT) eaten+=valknight;
    if(tmp->oldpiece & PAWN) eaten+=valpawn;
    } 
  val = FindSeries(p, len - 1, count + 1, lmrq, umrq, mec, mgm, eaten
    , valpawn, valknight, valbishop, valrook, valqueen);
  if(val == 2) {
    UnmakeMove(p, tmp);
    return 2;  }
  if(val == 1)  {
    printf("\nSERIES FOUND--> ");
    for(k = 0; k < count+1; k++)
      PrintMoveBrief(&CurrLine[k]);
    fflush(stdout); 
/*    printf("\nContinue the search? (y/n) ");
    fgets(line, 200, stdin);
    if(line[0] == 'n' || line[0] == 'N') {
    if(tmp->oldpiece & xside)  {
      if(tmp->oldpiece & QUEEN) eaten-=valqueen;
      if(tmp->oldpiece & ROOK) eaten-=valrook;
      if(tmp->oldpiece & BISHOP) eaten-=valbishop;
      if(tmp->oldpiece & KNIGHT) eaten-=valknight;
      if(tmp->oldpiece & PAWN) eaten-=valpawn;
    } 
      UnmakeMove(p, tmp);
      return 2;   }   */
       }  

  if(tmp->oldpiece & xside)  {
    if(tmp->oldpiece & QUEEN) eaten-=valqueen;
    if(tmp->oldpiece & ROOK) eaten-=valrook;
    if(tmp->oldpiece & BISHOP) eaten-=valbishop;
    if(tmp->oldpiece & KNIGHT) eaten-=valknight;
    if(tmp->oldpiece & PAWN) eaten-=valpawn;
    } 

  UnmakeMove(p, tmp);
  j++;
}

  return 0; 
}




int IsMatedGhosted(Position *p, int len, int count)  {

int i, j, val, isghost;
unsigned char xside, tmpside;
Listnode ml;
Move *tmp;

if(p->side == WHITE) xside = BLACK;
else xside = WHITE;

if(IsCheck(p, xside))  {
  tmpside = p->side;
  p->side = xside;
  GenerateMoves(&ml, p);
  p->side = tmpside;
  if(ml.num == 0) return 1;  /* mate */
  i = 0; j = 0; isghost = 1;
  for(i = 0; i < MAXLEGAL; i++)  {
    if(j == ml.num) break;
    tmp = &(ml.moves[i]);
    if(tmp->mtype == EMPTY) continue;
    MakeMove(p, tmp);
    if(IsCheck(p, p->side) == 0) isghost = 0;
    UnmakeMove(p, tmp);
  }
  if(isghost == 1) return 2;  /* ghost */
return 0;  /* leaf with no ghost or mate */  
}

if(len == 0) return 0;  /* leaf with no check */

GenerateMoves(&ml, p);
j = 0;
for(i = 0; i < MAXLEGAL; i++)  {
  if(j == ml.num) break;
  tmp = &(ml.moves[i]);
  if(tmp->mtype == EMPTY) continue;
  memcpy(&MoreLine[count], tmp, sizeof(Move));
  MakeMove(p, tmp);
  val = IsMatedGhosted(p, len - 1, count + 1);
  if(val == 3) {
    UnmakeMove(p, tmp);
    return 3;  }
  if(val == 1 || val == 2)  {
/*    if(val == 1) printf("\n-->Gets mated by: ");
    else printf("\n-->Gets ghosted by: ");
    for(k = 0; k < count+1; k++)
      PrintMoveBrief(&MoreLine[k]);
    fflush(stdout);
*/
    UnmakeMove(p, tmp);
    return 3;   }
  UnmakeMove(p, tmp);
  j++;
}

return 0;

} 
