## Bucephalus

  An analysis tool for the game of progressive chess.

### Getting Started

To install Bucephalus, just type:

```bash
$ make install
```

Bucephalus is an interactive program that runs inside a terminal window.  It prints an ASCII board followed by a prompt,
and the user types in single letter commands to make moves or evaluate/solve positions.  Type 'h' or '?' at the prompt to see a list of commands.

### Features

  * Understands the rules of Scottish Progressive Chess, described here: https://en.wikipedia.org/wiki/Progressive_chess
  * Can print legal moves from any position ('l' command).
  * Can show a game record ('r' command).
  * Can solve a seriesmate of a specified length ('s' command). This can also solve for *ghosts* (series that force the opponent to deliver check with the first move of their series).
  * Can evaluate a position given a specified ply depth ('e' command).  Has two evaluation functions: one based on material differential, and one solely based on minimizing opponent material.
  * Can run a series find with many criteria ('f' command) and combine this with the solve function to find series that consume a certain amount of material without getting mated or ghosted.

### Help Menu
```
?/h:	Print this help info
b:	Go back one move
c:	Show current score
d:	Show current position
e:	Minmax evaluator
f:	Series finder
l:	Show legal moves
m:	Make a move
n:	New game
p:	Show position status
q/x:	Exit Buce
r:	Show game moves
s:	Seriesmate solver
v:	Show version information
```

#### Contributors

 * Author: [Doug Hyatt](https://github.com/hyattpd/)

#### License

  [GPL](LICENSE)
