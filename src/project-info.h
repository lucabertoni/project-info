#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

// Le descrizioni delle funzioni si trovano appena sopra l'inizializzazione della funzione stessa
// Questa è solamente una dichiarazione di un prototip
void printHelp(void);
void scanProject(char *sCommand,char *sPath);
void unknownCommand(void);
int getRowCount(char *sPath);
void printRowCount(char *sPath);
int getFileRowCount(char *sFilePath);
void printProjectCost(char *sPath);
int getProjectCost(char *sPath);
struct config
{
	float nAverageWritingTime;
	float nHourlyCost;
};
struct stLanguages
{
	char *sLinguaggio; // Linguaggio e non, es: Java
	int nTotale;		// Numero di occorrenze totali, es: 15
};
struct config loadConf();
char** parseConf(char *sConfLine);
char* estensione2Linguaggio(char* sEstensione);
struct stLanguages *incrementaLinguaggio(char* sLinguaggio,int nTotale,struct stLanguages *aLanguages);
struct stLanguages *getLanguages(char *sPath);
void printLanguages(char *sPath);