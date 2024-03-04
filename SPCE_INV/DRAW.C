#include "DRAW.H"
#include "SPRT_LOA.H"
#include <graphics.h>

void start(){
    int gdriver = DETECT, graphic_mode;
    load_sprite();
    initgraph(&gdriver, &graphic_mode, "c:\\turboc3\\bgi");
    setbkcolor(0); //colore sfondo nero
}

void draw(int a, int c, int x, int y,int SIZE){
    int rows=sizeof(sprites[a])/ sizeof(sprites[a].value[0]);
    int column=sizeof(sprites[a].value[0])/ sizeof(sprites[a].value[0][0]);
    int i,j,v,h;
    for(j=0; j<column; j++){
	    for(i=0; i<rows; i++){
		    if(sprites[a].value[i][j]=='1'){
		        for(v=0;v<SIZE; v++){
		            for(h=0;h<SIZE;h++){
			            putpixel(x+(i*SIZE)+h,y+(j*SIZE)+v,c);
		            }
		        }
		    }
	    }
    }
}

void close(){
    closegraph();
}