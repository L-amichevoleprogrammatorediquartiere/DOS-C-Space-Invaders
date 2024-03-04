#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED

void start();
void draw(int a, int c, int x, int y,int SIZE);
/*c=    0 nero
        2 verde
        15 bianco

  a=    0 alien1
        1 alien2
        2 alien3
        3 ply
        4 explode
        5 b1
        6 b2
        7 b3
        8 b4
        9 b5
        10 ammo*/

void close();

#endif