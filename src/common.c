#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
	Cosa fa			:	
*/
char** explode(char *sLine,char cSeparatore){
	char sApp[strlen(sLine)];	// Stringa di appoggio
	char cCarattere;
	int i,nCount,i2,nLen;
	char **aRet = NULL;

	aRet = (char**)malloc(sizeof(char*));

	i = nCount = i2 = 0;
	nLen = 1024;

	while(cCarattere = sLine[i]){

		// Se il carattere trovato è il carattere separatore, allora salvo la riga che ho estratto
		// fin'ora e poi la azzero
		if(cCarattere == cSeparatore){
			++i2;

			// Alloco memoria all'array di stringhe per un elemento
			aRet = (char**)realloc(aRet,i2*sizeof(aRet));

			nLen = strlen(sApp);

			// Alloco memoria per una nuova stringa per l'array di stringhe
			aRet[i2-1] = (char*)malloc(nLen*sizeof(sLine));

			// Copio la stringa di appoggio nell'array di ritorno
			strcpy(aRet[i2-1],sApp);

			// Azzero la stringa di appoggio e il suo contatore
			strcpy(sApp,"");
			nCount = 0;
		}else{
			// Aggiungo il carattere estratto in fondo alla riga di appoggio
			sApp[nCount] = cCarattere;
			sApp[nCount+1] = '\0';
			++nCount;
		}
		++i;
	}

	// Alloco memoria per una nuova stringa per l'array di stringhe
	aRet[i2] = (char*)malloc(nLen*sizeof(sLine));
	strcpy(aRet[i2],sApp);

	return aRet;
}