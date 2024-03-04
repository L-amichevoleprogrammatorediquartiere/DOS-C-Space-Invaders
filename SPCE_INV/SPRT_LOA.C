#include "SPRT_LOA.H"
#include <stdio.h>
#define BUFFER 20

//rows = sizeof(array)     / sizeof(array[0]);
//cols = sizeof(array[0])  / sizeof(array[0][0]);
//questa funzione inizializza gli sprite che sono stati disegnati come
//dei semplici file di testo
struct sprite sprites[11];

void load_sprite(){
	char nameFile[11][BUFFER]={"ALIEN1.TXT", "ALIEN2.TXT", "ALIEN3.TXT", "PLY.TXT",
				"EXPLODE.TXT","B1.TXT","B2.TXT","B3.TXT","B4.TXT","B5.TXT","AMMO.TXT"};
	int numFile=sizeof(nameFile)/BUFFER/sizeof(char);
	FILE *filePointer;
	char buffer[BUFFER];
	int i,y,x;
	for (i=0; i<numFile; i++){
		filePointer= fopen(nameFile[i],"r");
		if(filePointer== NULL)
			printf("file non aperto correttamente");
		y=0;
		while(fgets(buffer,BUFFER-1, filePointer)!= NULL){
			x=0;
			while(buffer[x]!='\n'){
				sprites[i].value[x][y]=buffer[x];
				x++;
			}
			y++;
		}
		fclose(filePointer);
	}
}