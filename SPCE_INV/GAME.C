#include "GAME.H"
#include "DRAW.H"
#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int qammop=0,qammoa=0; //quantità di proiettili sullo schermo risp. del player e degli alinei
int latoxb,latoyb; //conterrano rispettivamente la lunghezza e l'altezza di ogni blocco della base

struct alien alieni[9][5];
struct base basi[4];
struct object player;
struct object *ammop;
struct object *ammoa;

struct alien old_alieni[9][5];
struct base old_basi[4];
struct object old_player;
struct object *old_ammop;
struct object *old_ammoa;

#define ACOLOR 15
#define PCOLOR 2
#define BCOLOR 2
#define DCOLOR 0
#define PLAYER_MOV 10 //numero di pixel movimento del player ad ogni loop
#define LIMIT_SX 90
#define LIMIT_DX 550
#define PROB_4C_ASHOOT 10 //probabilità ad ogni ciclo del gioco che un alieno spari
                           //10 = 1%
#define AMMO_MOV 10 //velocità proiettili

void copy_alien(struct alien *dest, struct alien *src){
    dest->xp=src->xp;
    dest->yp=src->yp;
    dest->xa=src->xa;
    dest->ya=src->ya;
    dest->tipo=src->tipo;
    dest->stato=src->stato;
}

void copy_object(struct object *dest, struct object *src){
    dest->xp=src->xp;
    dest->yp=src->yp;
    dest->xa=src->xa;
    dest->ya=src->ya;
    dest->stato=src->stato;
}

int compare_base(int stato1[4][4], int stato2[4][4]){
    int x,y;
    for(y=0;y<4;y++){
        for(x=0;x<4;x++){
            if(stato1[x][y] != stato2[x][y]){
                return 0;
            }
        }
    }
    return 1;//se sono uguali
}

void copy_state(int dest[4][4], int src[4][4]){
    int x,y;
    for(y=0;y<4;y++){
        for(x=0;x<4;x++){
            dest[x][y]=src[x][y];
        }
    }
}

void copy_base(struct base *dest, struct base *src){
    dest->xp=src->xp;
    dest->yp=src->yp;
    dest->xa=src->xa;
    dest->ya=src->ya;
    copy_state(dest->stato,src->stato);
}

void copy_in_old(){
    //copiare alieni
    int x,y,i;
    for(y=0;y<5;y++){
        for(x=0;x<9;x++){
            if(alieni[x][y].xp!=old_alieni[x][y].xp ||
                alieni[x][y].yp!=old_alieni[x][y].yp){
                    copy_alien(&old_alieni[x][y],&alieni[x][y]);
            }
        }
    }
    //copiare player
    if(player.xp != old_player.xp) 
        copy_object(&old_player,&player);
    //copiare basi
    for(i=0;i<4;i++){
        if(!compare_base(basi[i].stato,old_basi[i].stato)){
            copy_base(&old_basi[i],&basi[i]);
        }
    }
    //copiare ammo player
    for(i=0;i< qammop;i++){
        if(old_ammop[i].yp!=ammop[i].yp){
            copy_object(&old_ammop[i],&ammop[i]);
        }
    }
    //copiare ammo alien
    for(i=0;i< qammoa;i++){
        if(old_ammoa[i].yp!=ammoa[i].yp){
            copy_object(&old_ammoa[i],&ammoa[i]);
        }
    }
}

void initialize(){
    int x,y,tipo,inc,height,width,i;
    //inizializzare seme num casuali
    srand(time(NULL));
    //assegno valori e disegno alieni
    for(y=0;y<5;y++){
		for(x=0;x<9;x++){
			if(y==0)
                tipo=2,inc=5,height=8,width=8;
			else if(y==1 || y==2)
                tipo=0,inc=2,height=8,width=11;
			else if(y==3 || y==4)
		        tipo=1,inc=0,height=8,width=12;
	    //draw(tipo,15,145+(x*40)+inc,50+y*42,2);
            alieni[x][y].xp=145+(x*40)+inc;
	        alieni[x][y].yp=50+y*42;
	        alieni[x][y].xa=145+(x*40)+inc+width;
	        alieni[x][y].ya=50+y*42+height;
	        alieni[x][y].tipo=tipo;
	        alieni[x][y].stato=1;
		}
	}

    //assegno valori player 11*7    *3
    //draw(3,2,293,400,3);
    player.xp=299;
    player.yp=400;
    player.xa=299+(11*3);
    player.ya=400+(7*3);
    player.stato=3;

    //assegno valori basi 20*19     *2
    for(i=0;i<4;i++){
        basi[i].xp=186+(i*76);
        basi[i].yp=320;
        basi[i].xa=186+(i*76)+(20*2);
        basi[i].ya=320+(19*2);
        for(y=0;y<4;y++){
            for(x=0;x<4;x++){
                basi[i].stato[x][y]=1;
            }
        }
    }
    //assegno dinamicamente ammop e ammoa
    ammop= (struct object*)malloc(qammop *(sizeof(struct object)));
    ammoa= (struct object*)malloc(qammoa *(sizeof(struct object)));

    old_ammop= (struct object*)malloc(qammop *(sizeof(struct object)));
    old_ammoa= (struct object*)malloc(qammoa *(sizeof(struct object)));

    latoxb=(basi[0].xa-basi[0].xp)/4;
    latoyb=(basi[0].ya-basi[0].yp)/4;
}

void print_base(struct base *b_print, struct base *b_compare, int bc){
    int x,y,i,a,inc;
    for(i=0;i<4;i++){
        if(!compare_base(b_compare[i].stato, b_print[i].stato)){
            for(y=0;y<4;y++){
                for(x=0;x<4;x++){
                    if(b_print[i].stato[x][y]==1){
                        inc=0;
                        if(!((x==1 || x==2 ) && y==3)){
                            if(x==0 && y==0)
                                a=7;
                            else if(x==3 && y==0)
                                a=9;
                            else if(x==0 && y==3)
                                a=5;
                            else if(x==3 && y==3)
                                a=8,inc=-4;
                            else
                                a=6;
                            draw(a,bc,b_print[i].xp+(x*10)+inc,b_print[i].yp+(y*10),2);
                        }
                    }//verfico se è in buono stato la base
                }
            }
        }
    }
}

void print_object(struct alien (*a_print)[5], struct alien (*a_compare)[5], int ac,
                    struct object *p_print, struct object *p_compare, int pc,
                    struct base *b_print, struct base *b_compare, int bc,
                    struct object *ap_print,int apc,
                    struct object *aa_print,int aac){
    //da stampare da confrontare
    //stampo alieni
    int x,y,i;
    for(x=0;x<9;x++){
        for(y=0;y<5;y++){
            if(a_print[x][y].stato!=0){
                //stampo solo se si è mosso l'alieno
                if(a_compare[x][y].xp !=  a_print[x][y].xp || 
                    a_compare[x][y].yp !=  a_print[x][y].yp){
                        draw(a_print[x][y].tipo,ac,
                            a_print[x][y].xp,a_print[x][y].yp,2);
                    }
            }//non esploso può essere in salute o in esplosione
        }
    }
    //stampo player solo se si è mosso
    if(p_print->xp != p_compare->xp){
        draw(3,pc,p_print->xp,p_print->yp,3);
    }
    //stampo base
    print_base(b_print,b_compare,bc);
    //stampo munizioni  player
    for(i=0;i<qammop;i++){
        draw(10,apc,ap_print[i].xp,ap_print[i].yp,2);
    }
    //stampo munizioni alieni
    for(i=0;i<qammoa;i++){
        draw(10,aac,aa_print[i].xp,aa_print[i].yp,2);
    }
}

int key_pressed(){
    if(kbhit()){
        int tasto= getch();
        if(tasto==65 || tasto ==97)
            return 1;
        else if(tasto==68|| tasto ==100)
            return 2;
        else if(tasto==32)
            return 3;
        else if(tasto==27)
            return 4;
    }
    return 0;
}

void player_move(){
    int key;
    key=key_pressed();
    switch (key){
        case 1://sx
        if(player.xp>LIMIT_SX){
            player.xp-=PLAYER_MOV;
            player.xa-=PLAYER_MOV;
        }
        break;
        case 2://dx
        if(player.xa<LIMIT_DX){
            player.xp+=PLAYER_MOV;
            player.xa+=PLAYER_MOV;
        }
        break;
    }
}

int dir=0,alien_mov=5; //0 gli alieni si muovono verso sinistra, 1 verso destra

void alien_move(){
    /*gli alieni si muovono prima le utlime due righe poi altre due e poi l'ultima*/
    int found=0, colonna, riga, lriga,x,y;
    for(y=0;y<5;y++){
        for(x=0;x<9;x++){
            if(alieni[x][y].stato==1){
                riga=y,found=1;
                break;
            }
        }
        if(found)
            break;
    }//trovare elemento più sopra
    found=0;
    for(y=4;y>=0;y--){
        for(x=0;x<9;x++){
            if(alieni[x][y].stato==1){
                lriga=y,found=1;
                break;
            }
        }
        if(found)
            break;
    }//trovare elemento più sotto
    found=0;
    if(dir==0){ //sinistra
        for(x=0;x<9;x++){
            for(y=0;y<5;y++){
                if(alieni[x][y].stato==1){
                    colonna=x,found=1;
                    break;
                }
            }
            if(found)
                break;
        }//trovare elemento più a sinistra
        found=0;
        if(riga==0 && (alieni[0][0].xp - alieni[0][1].xp)>alien_mov  || lriga==0 && riga==0){
            for(x=0;x<9; x++){
                alieni[x][0].xp-=alien_mov;
                alieni[x][0].xa-=alien_mov;
            }
        }//controllo se la 2 e 3 riga sono più a sx della prima
        else if((riga <3) && (alieni[0][2].xp - alieni[0][3].xp)>alien_mov || (riga <3) && (lriga==1 || lriga==2)){
            for(y=1;y<3;y++){
                for(x=0;x<9; x++){
                    alieni[x][y].xp-=alien_mov;
                    alieni[x][y].xa-=alien_mov;
                }
            }
        }
        else if(alieni[colonna][4].xp-alien_mov>LIMIT_SX+10 && (lriga==3 ||  lriga==4)){
            for(y=3;y<5;y++){
                for(x=0;x<9; x++){
                    alieni[x][y].xp-=alien_mov;
                    alieni[x][y].xa-=alien_mov;
                }
            }
        }//muovo riga 4 e 5 se non oltrepasso oltre il limite altrimenti scendono e cambio dir
        else{
            for(y=0;y<5;y++){
                for(x=0;x<9; x++){
                    alieni[x][y].yp+=alien_mov/2;
                    alieni[x][y].ya+=alien_mov/2;
                }
            }
            dir=1;
        }
    }
    else{//destra
        for(x=8;x>=0;x--){
            for(y=0;y<5;y++){
                if(alieni[x][y].stato==1){
                    colonna=x,found=1;
                    break;
                }
            }
            if(found)
                break;
        }//trovare elemento più a destra
        if(riga==0 && (alieni[0][1].xp - alieni[0][0].xp)>alien_mov || lriga==0 && riga==0){
            for(x=0;x<9; x++){
                alieni[x][0].xp+=alien_mov;
                alieni[x][0].xa+=alien_mov;
            }
        }//controllo se la 2 e 3 riga sono più a sx della prima
        else if(riga<3 && (alieni[0][3].xp - alieni[0][2].xp)>alien_mov || (riga <3) && (lriga==1 || lriga==2)){
            for(y=1;y<3;y++){
                for(x=0;x<9; x++){
                    alieni[x][y].xp+=alien_mov;
                    alieni[x][y].xa+=alien_mov;
                }
            }
        }
        else if(alieni[colonna][4].xa+alien_mov<LIMIT_DX-10 && (lriga==3 || lriga==4)){
            for(y=3;y<5;y++){
                for(x=0;x<9; x++){
                    alieni[x][y].xp+=alien_mov;
                    alieni[x][y].xa+=alien_mov;
                }
            }
        }//muovo riga 4 e 5 se non oltrepasso oltre il limite altrimenti scendono e cambio dir
        else{
            for(y=0;y<5;y++){
                for(x=0;x<9; x++){
                    alieni[x][y].yp+=alien_mov/2;
                    alieni[x][y].ya+=alien_mov/2;
                }
            }
            dir=0;
        }
    }
}

void player_shoot(){
    int key;
    key=key_pressed();
    if(key==3){
        qammop++;
        ammop=(struct object*)realloc(ammop, qammop*sizeof(struct object));
        old_ammop=(struct object*)realloc(old_ammop, qammop*sizeof(struct object));
        //ply 11*7 *3  ammo
        ammop[qammop-1].xp=player.xp+((player.xa-player.xp)/2);
        ammop[qammop-1].yp=player.yp-8;
        ammop[qammop-1].xa=ammop[qammop-1].xp+2;
        ammop[qammop-1].ya=player.yp;
        ammop[qammop-1].stato=1;
    }
}

void alien_shoot(){
    int shooter[9],x,y,random_number,found=0;
    //qui metterò la posizione per ogni colonna dell'alieno che potrebbe stampare(che non ha nessuno sotto)
    if((rand()%1000 +1)<=PROB_4C_ASHOOT){
        for(x=0;x<9;x++){
            found=0;
            for(y=4;y>=0;y++){
                if(alieni[x][y].stato==1){
                    shooter[x]=y;
                    found=1;
                    break;
                }

            }
            if(!found)
                shooter[x]=10; //se nella fila non c'è nessuno mettiamo 10
        }
        random_number=rand()%9;
        while(1){
            if(shooter[random_number%9]!=10){
                qammoa++;
                ammoa=(struct object*)realloc(ammoa,qammoa*sizeof(struct object));
                old_ammoa=(struct object*)realloc(old_ammoa,qammoa*sizeof(struct object));
                ammoa[qammoa-1].xp=alieni[random_number][shooter[random_number]].xp + 
                                    ((alieni[random_number][shooter[random_number]].xa - alieni[random_number][shooter[random_number]].xp)/2);
                ammoa[qammoa-1].yp=alieni[random_number][shooter[random_number]].ya;
                ammoa[qammoa-1].xa=ammoa[qammoa-1].xp+2;
                ammoa[qammoa-1].ya=ammoa[qammoa-1].yp+8;
                ammoa[qammoa-1].stato=1;
                break;
            }
            random_number++;
        }
    }
}

void ammo_delete(struct object *ammo,struct object *old_ammo,int size,int index){
    int i;
    draw(10,DCOLOR,old_ammo[index].xp,old_ammo[index].yp,2);
    draw(10,DCOLOR,ammo[index].xp,ammo[index].yp,2);
    if(index!=size-1){
        for(i=index;i<size-1;i++){
            copy_object(&ammo[i],&ammo[i+1]);
            copy_object(&old_ammo[i],&old_ammo[i+1]);
        }
    }
}

void ammo_move(){
    int i;
    for(i=0;i< qammop;i++){
        if(ammop[i].ya>0-AMMO_MOV){
            ammop[i].ya-=AMMO_MOV;
            ammop[i].yp-=AMMO_MOV;
        }//non hanno raggiunto il limite
        else{
            ammo_delete(ammop,old_ammop,qammop,i);
            qammop--;
            i--;
        }//hanno raggiunto il limite
    }
    //muovere ammo alieni
    for(i=0;i< qammoa;i++){
        if(ammoa[i].ya<480+AMMO_MOV){
            ammoa[i].ya+=AMMO_MOV;
            ammoa[i].yp+=AMMO_MOV;
        }//non hanno raggiunto il limite
        else{
            ammo_delete(ammoa,old_ammoa,qammoa,i);
            qammoa--;
            i--;
        }//hanno raggiunto il limite
    }
}

void controlla_conflitti(){
    int i,j,oggetto,found,x,y;
    for(i=0;i< qammop;i++){
        found=0;
        //se il proiettile è a livello base
        if(ammop[i].yp>=basi[0].yp && ammop[i].yp<=basi[0].ya){
            for(j=0;j<4;j++){
                if(ammop[i].xp>=basi[j].xp && ammop[i].xp<=basi[j].xa){//bravo ti sei colpito una base ora vediamo quale punto
                    for(x=0;x<4;x++){
                        for(y=0;y<4;y++){
                            if(ammop[i].xp>= basi[j].xp+(latoxb*x) && ammop[i].xp<= basi[j].xa-(latoxb*(3-x)) &&
                                ammop[i].yp<= basi[j].ya-(latoyb*(3-y)) && ammop[i].yp>= basi[j].yp+(latoyb*y) && 
                                basi[j].stato[x][y]==1 && !(y==3 && (x==1 || x==2))){
                                //colpito
                                basi[j].stato[x][y]=0;
                                ammo_delete(ammop,old_ammop,qammop,i);
                                qammop--;
                                i--;
                                break;
                            }
                        }
                        if(found)
                        break;
                    }
                }
                if(found)
                    break;
            }
        }
        //se il proiettile è a livello alieni 4 riga
        else if(ammop[i].yp>=alieni[0][4].yp && ammop[i].yp<=alieni[0][4].ya){
            for(x=0;x<9;x++){
                if(ammop[i].xp>=alieni[x][4].xp && ammop[i].xp<=alieni[x][4].xa+10 && alieni[x][4].stato==1){
                    alieni[x][4].stato=0;
                    ammo_delete(ammop,old_ammop,qammop,i);
                    qammop--;
                    i--;
                    break;
                }
            }
        }
        //se il proiettile è a livello alieni 3 riga
        else if(ammop[i].yp>=alieni[0][3].yp && ammop[i].yp<=alieni[0][3].ya){
            for(x=0;x<9;x++){
                if(ammop[i].xp>=alieni[x][3].xp && ammop[i].xp<=alieni[x][3].xa+10 && alieni[x][3].stato==1){
                    alieni[x][3].stato=0;
                    ammo_delete(ammop,old_ammop,qammop,i);
                    qammop--;
                    i--;
                    break;
                }
            }
        }
        //se il proiettile è a livello alieni 2 riga
        else if(ammop[i].yp>=alieni[0][2].yp && ammop[i].yp<=alieni[0][2].ya){
            for(x=0;x<9;x++){
                if(ammop[i].xp>=alieni[x][2].xp && ammop[i].xp<=alieni[x][2].xa+10 && alieni[x][2].stato==1){
                    alieni[x][2].stato=0;
                    ammo_delete(ammop,old_ammop,qammop,i);
                    qammop--;
                    i--;
                    break;
                }
            }
        }
        //se il proiettile è a livello alieni 1 riga
        else if(ammop[i].yp>=alieni[0][1].yp && ammop[i].yp<=alieni[0][1].ya){
            for(x=0;x<9;x++){
                if(ammop[i].xp>=alieni[x][1].xp && ammop[i].xp<=alieni[x][1].xa+10 && alieni[x][1].stato==1){
                    alieni[x][1].stato=0;
                    ammo_delete(ammop,old_ammop,qammop,i);
                    qammop--;
                    i--;
                    break;
                }
            }
        }
        //se il proiettile è a livello alieni 0 riga
        else if(ammop[i].yp>=alieni[0][0].yp && ammop[i].yp<=alieni[0][0].ya){
            for(x=0;x<9;x++){
                if(ammop[i].xp>=alieni[x][0].xp && ammop[i].xp<=alieni[x][0].xa+10 && alieni[x][0].stato==1){
                    alieni[x][0].stato=0;
                    ammo_delete(ammop,old_ammop,qammop,i);
                    qammop--;
                    i--;
                    break;
                }
            }
        }
    }
    //controlliamo proiettili alieni
    for(i=0;i< qammoa;i++){
        found=0;
        //se il proiettile è a livello base
        if(ammoa[i].ya>=basi[0].yp && ammoa[i].ya<=basi[0].ya){
            for(j=0;j<4;j++){
                if(ammoa[i].xp>=basi[j].xp && ammoa[i].xp<=basi[j].xa){//bravo ti sei colpito una base ora vediamo quale punto
                    for(x=0;x<4;x++){
                        for(y=0;y<4;y++){
                            if(ammoa[i].xp>= basi[j].xp+(latoxb*x) && ammoa[i].xp<= basi[j].xa-(latoxb*(3-x)) &&
                                ammoa[i].ya<= basi[j].ya-(latoyb*(3-y)) && ammoa[i].ya>= basi[j].yp+(latoyb*y) && 
                                basi[j].stato[x][y]==1 && !(y==3 && (x==1 || x==2))){
                                //colpito
                                basi[j].stato[x][y]=0;
                                ammo_delete(ammoa,old_ammoa,qammoa,i);
                                qammoa--;
                                i--;
                                break;
                            }
                        }
                        if(found)
                        break;
                    }
                }
                if(found)
                    break;
            }
        }
        //se il proiettile è a livello player
        else if(ammoa[i].ya>=player.yp && ammoa[i].ya<=player.ya && ammoa[i].xp >= player.xp && ammoa[i].xp <= player.xa){
            player.stato=0;
            ammo_delete(ammoa,old_ammoa,qammoa,i);
            qammoa--;
            i--;
        }
    }
}

int alien_win(){
    if(player.stato==0 || alieni[0][4].ya+25>=basi[0].yp){
        gotoxy(30,20);
        printf("!!!!! ALIEN WIN!!!!!");
        return 1;
    }
    else
        return 0;
}

int player_win(){
    int found=1,x,y;
    for(y=0;y<5;y++){
        for(x=0;x<9;x++){
            if(alieni[x][y].stato==1)
                found=0;
        }
    }
    if(found){
        gotoxy(30,20);
        printf("!!!!! PLAYER WIN !!!!!");
    }
    return found;
}

int game(){
    int i=0;//640*480
    start();
    initialize();
    print_object(alieni,old_alieni,ACOLOR,&player,&old_player,PCOLOR,basi,old_basi,BCOLOR,ammop,ACOLOR,ammoa,ACOLOR);
    while(1){
        copy_in_old();
        alien_move();
        alien_shoot();
        player_move();
        player_shoot();
        ammo_move();
        /*controllare conflitti nel caso eliminare proiettili e
        modifcare stato elemento colpito*/
        controlla_conflitti();
        //pulisco quelli vecchi se il vecchio è diverso dal nuovo
        print_object(old_alieni,alieni,DCOLOR,&old_player,&player,DCOLOR,old_basi,basi,DCOLOR,old_ammop,DCOLOR,old_ammoa,DCOLOR);
        print_object(alieni,old_alieni,ACOLOR,&player,&old_player,PCOLOR,basi,old_basi,BCOLOR,ammop,ACOLOR,ammoa,ACOLOR);
        //copiare nuovi nei vecchi

        if(key_pressed()==4 || player_win()==1 || alien_win()==1){
            break;
        }
        //delay(50);
    }
}