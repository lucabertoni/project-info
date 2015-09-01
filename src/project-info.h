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
struct config loadConf();
char** parseConf(char *sConfLine);
int getLanguages(char *sPath);
void printLanguages(char *sPath);