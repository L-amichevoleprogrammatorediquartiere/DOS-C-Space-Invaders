#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

struct alien{
    int xp,yp;
    int xa,ya;
    int stato,tipo;
};

struct object{
    int xp,yp;
    int xa,ya;
    int stato;
};

struct base{
    int xp,yp;
    int xa,ya;
    int stato[4][4];
};
//stato 1 buono stato 0 esploso (2- esplodendo;

int game();
//ritorna 1 se vince il player 0 gli alieni.....

#endif