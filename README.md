# DOS-C-Space-Invaders

4 days challenge..... 
(i have lost the screenshot i take while the development of the game but i have some videos i have registered because i have send this video to my classmate/dawg Peppe5liters (he is on github, chek his profile))
## 1 day(SPRITE LOADING AND INITGRAPH ERROR):
every sprite of the game is create from the txt file, so in the first day i have write the txt file
this is the sprite of the player:
<img src="https://github.com/L-amichevoleprogrammatorediquartiere/DOS-C-Space-Invaders/assets/64810680/1475017c-2542-4bac-b625-c05319226909" width="300" height="300">
<img src="https://github.com/L-amichevoleprogrammatorediquartiere/DOS-C-Space-Invaders/assets/64810680/0d350e86-7e57-4673-9e80-af011a9c8d0e" width="250" height="150">

from the txt to pixel on screen i have write the module SPRT_LOA (sprite loader) it open every txt file and save in
```
struct sprite{
    char value[13][9];    
};
```
so the first day i don't have do a lot of work because after the moudle SPRT_LOAD i have started write the moudle DRAW but when i called the function initgraph, it return error, so the remain part of the day i wasted it trying to fix it (spolier after the closure of the program still return error BGI but if it work i don'touch it anymore......
## 2 day(DRAWING ON THE SCREEN WITH PUTPIXEL BUT IS TOO SLOW)
i have used the library
```
#include <graphics.h>
```
knowing that is old and deprecated, but why i have choice it?? Because back in the time when i start learn C (my first language, and my first love hahahaha) i used a old book that for the graphics part use this library, so i used this like a good ending, like a good bye, for the book. Returning to the module DRAW i write in it the function

```
void draw(int a, int c, int x, int y,int SIZE);
```
where a is the type of sprite to draw (0 alien1, 1 alien2, 2 alien3, 3 ply, 4 explode, 5 b1, 6 b2, 7 b3, 8 b4, 9 b5, 10 ammo), c is the color (0 black, 2 green, 15 white), (x, y) the coordinates of the pixel and SIZE the zoom of the pixel es.(1 pixel of the sprite = SIZE * SIZE in the screen).
After this the foundation of the program are ready, now in the last part of the second day i have to finds the rights coordinates for all the alien on the screen, so in the main i write something like this

```
int main(){
  draw(0,15,100,150,2);
  ecc...

  return 0;
}
```
so the result of the 2 day is(imagine there are all the alien on the screen):


<img src="https://github.com/L-amichevoleprogrammatorediquartiere/DOS-C-Space-Invaders/assets/64810680/3db60c81-0931-46a4-81c5-6fd0c28a8753" width="410" height="300">


## 3 day(START MAKING THE MAIN MODULE "GAME"):
In the module GAME i have initialized the struct:

```
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
```
in every struct i have xp,yp (that rappresents the top left corner of the object) and xa,ya (that rappresents the bottom right corner). In the struct alien stato can be 0 or 1 (0 when alien is destroyed and 1 when is good) and tipo rappresent the alien type that can be 1,2 or 3. The sctruct base have an int array stato that rappresent the state of every 16 pieces of the base (yes, the base is divided in block and don't be destroyed realistically like the original). And the struct object that i use for the player and ammo/bullet. After the creation of the structs, i writed the main function for GAME game(), basically for every loop of the game i do:

```
while(1){
        copy_in_old(); 
        alien_move();
        alien_shoot();
        player_move();
        player_shoot();
        ammo_move();  //move every bullet on the screen
        controlla_conflitti(); //check if bullet touch another object
        print_object(old_alieni,alieni,DCOLOR,&old_player,&player,DCOLOR,old_basi,basi,DCOLOR,old_ammop,DCOLOR,old_ammoa,DCOLOR);
        print_object(alieni,old_alieni,ACOLOR,&player,&old_player,PCOLOR,basi,old_basi,BCOLOR,ammop,ACOLOR,ammoa,ACOLOR);

        if(key_pressed()==4 || player_win()==1 || alien_win()==1){
            break;
        }
        //delay(50);
    }
```
I try do delete for every loop all the screen and then rewrite but the function print_object is too slow so don't write all the screen in time, to solve this problem i dont't rewrite all the object but only the object who have moved but for do this i have initialized a copy of every array struct with the prefix "old_". Go better than rewrite all the object but still slow, i think that the function of graphics.h:
```
putpixel()
```
is too slow for this, but it's to late to change. At the end of day 3 i write all the module GAME but the function controlla_conflitti() give me some problem(start of the war with turbo c++).

## 4 day(LAST WAR AND TURBO C++ HAVE LOST....)

the problem was that Turbo C++ don't check index of an array(for solve this stupid probelm i have spent 4 hour of my life)
<img src="https://github.com/L-amichevoleprogrammatorediquartiere/DOS-C-Space-Invaders/assets/64810680/639f15a6-4714-427a-a637-48853d0340b8">


me after 4 hour just to find is a simple index error....

btw the final result is:

<img src="https://github.com/L-amichevoleprogrammatorediquartiere/DOS-C-Space-Invaders/assets/64810680/ef7a234a-eb28-47f3-ae22-db9b52892143">


