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

#define VERSION "1.0.0"

#define MAXGAMESIZE	200
#define MAXPLY	30
#define MAXLEGAL	100
#define HASHON		1
/* 40 meg hash table */
/*  #define HASHSIZE	833347   */
/* 240 meg hash table */
#define HASHSIZE	5000011 
#define GHOSTON		1
#define NULLPRUNE	0

typedef unsigned char SQUARE;
typedef struct _position {
  unsigned char side;
  unsigned char castle;
  unsigned short epstatus;
  short wkingx;
  short wkingy;
  short bkingx;
  short bkingy;
  SQUARE board[12][12];
} Position;
typedef struct _move {
  short source;
  short dest;
  short mtype;
  unsigned char promval;
  unsigned char oldpiece;
  unsigned char oldcastle;
  unsigned short oldep;
} Move;
typedef struct _listnode {
  int num;
  int start;
  int end;
  Move moves[MAXLEGAL];
} Listnode;
typedef struct _hashentry {
  unsigned long rank[8];
  short depth;
  short count;
  int value;
} Hashentry;
typedef struct _eline {
  int val;
  int jump;
  Move best;
  struct _eline *next;
} Eline;
typedef struct _gamerec {
  Move moves[MAXGAMESIZE];
} Gamerec;



#define DEBUG	0
#define DEBUGEVAL	0


#define PAWN	(1uL << 0)
#define KNIGHT	(1uL << 1)
#define BISHOP	(1uL << 2)
#define ROOK	(1uL << 3)
#define QUEEN	(1uL << 4)
#define KING	(1uL << 5)
#define WHITE	(1uL << 6)
#define BLACK	(1uL << 7)

#define OFFBOARD 255

#define NORMAL	0
#define CAPTURE	1
#define PROMOTE	2
#define PROMCAP	3
#define TWOMOVE	4
#define ENPASSANT	5
#define KSCASTLE	6
#define QSCASTLE	7
#define EMPTY	8

#define WKC	(1uL << 0)
#define WQC	(1uL << 1)
#define BKC	(1uL << 2)
#define BQC	(1uL << 3)

/* starting files for pieces in fischer random */
/* valid range is 2-9 (2 = a-file, 9 the h-file)        */
/* for regular chess, leave these values alone          */

#define QRFILE	2
#define KRFILE	9
#define QNFILE	3
#define KNFILE	8
#define QBFILE	4
#define KBFILE	7
#define KFILE 	6
#define QFILE	5

extern void GenerateMoves(Listnode *, Position *);
extern void GenerateLegalandIllegal(Listnode *, Position *);
extern void PrintListnode(Position *, Listnode *);
extern void PrintMove(Position *, Move *);
extern void AddMove(Listnode *, int, int, int, int, short);
extern void MakeMove(Position *, Move *);
extern void UnmakeMove(Position *, Move *);
extern void ClearMoves(Listnode *);
extern int Attacked(Position *, int, int, unsigned char);
extern void StartPosition(Position *);
extern void Make_Move(Position *, Move *);
extern int IsCheck(Position *, unsigned char);
extern int IsMate(Position *, unsigned char);
extern int IsEpSquare(short, short, unsigned short);
extern unsigned short MakeEpSquare(short, short, unsigned short);
extern unsigned short RemoveEpSquare(short, short, unsigned short);
extern int TimeP(int,int,int,int);
extern int Score(Position *, int, int);
extern int Binary(unsigned char);
extern short Minval(short, short);
extern short Maxval(short, short);
extern int AnyLegalMoves(Position *);
extern void PrintPosFlags(Position *, int, int);
extern void PrintBoard(Position *);
extern void PrintRecord(Gamerec *);
extern void ClearRecord(Gamerec *);
extern void AddtoRecord(Move *, Gamerec *, int, int);
extern void PopfromRecord(Gamerec *, Position *, int, int);
extern int MoveCompare(Move *, Move *);
extern int InputMove(Move *, Listnode *);
extern void Evaluate(Position *, int, int);
extern Eline *Minmax(Position *, int, int, int, int, int, int, int);
extern Move CurrLine[MAXPLY], Best[MAXPLY], MoreLine[MAXPLY];
extern void ElFree(Eline *);
extern void PrintMoveBrief(Move *);
extern void ShowCurrentLine(Eline *, Move *, int, int);
extern unsigned long HashPosition(Position *);
extern void SortMoves(Listnode *, Position *, int, int, int, int, int);
extern Hashentry TransTable[];
extern void AddtoTable(Position *, int, int, int);
extern int ResolveCollision(Position *p, unsigned long, int, int);
extern int totalc;
extern int dummyc;
extern void Solve(Position *);
extern int IsTransposition(Position *, int);
extern void AddtoTransTable(Position *, int);
extern int MateSolve(Position *, int, int, int);
extern int scoreflag;
extern unsigned char scoreside;
extern void Find(Position *);
extern void DumpPosition(Position *);
extern int FindSeries(Position *, int, int, int, int, int, int, int, int, int, int, int, int);
extern int IsMatedGhosted(Position *, int, int);
extern int IsStalemate(Position *, unsigned char);
