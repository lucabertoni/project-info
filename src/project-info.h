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
int getFileRowCount(char *sName);