# Upsilon

## What is Upsilon ?

Upsilon is a C++ game engine, designed to make 2D puzzles. It uses the [SDL2](http://libsdl.org/) library and the [Luwra](https://github.com/vapourismo/luwra) Lua wrapper. It is distributed under the GNU GPL license.

Subgames are written in Lua. They have to be in the `subgames` folder.

## How to compile and run Upsilon ?

On a GNU/Linux operating system you can do:  
```
$ sudo apt-get install libsdl2-dev libsdl2-image-dev git
$ git clone https://github.com/UpsilonPuzzle/upsilon.git --recursive
$ cd upsilon
$ chmod +x compile.sh
$ ./compile.sh
```

I guess Upsilon is compilable on a Microsoft Windows operating system, but sorry, I don't know how. If someone succeded to do this, please contact me by e-mail or open an issue.


To run Upsilon, you may give one argument: the subgame name (which is the subgame folder name). If you don't, Upsilon will ask you the subgame you want to use.  
For example:
```
$ ./upsilon mario_sokoban
```

## How to make a subgame ?

To make a subgame (if you do, you'll make me very happy :D), first learn the Lua programming language (don't worry, it's really easy).

The Lua API documentation will come very soon. For the moment, you can look at the "mario sokoban" subgame (unfinished) and try to understand how it works.

## This game engine is in development !

Remember: for the moment, Upsilon is very unfinished. But you can make his development faster by helping to develop it by creating issues, sending pull requests, writing the wiki, etc!!
