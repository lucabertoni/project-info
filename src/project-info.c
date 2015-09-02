#include "project-info.h"	// Includo l'header
#include "common.h"

#define COMMANDSCOUNT   	4       // Numero di comandi presenti nel programma (aumentare in base alla necessità)

/*******************************************/
/* TODO			:			funzione log   */
/* TODO			:			funzione print */
/*******************************************/

// TODO			:			File di configurazione per questi parametri
#define MAXFILENAMELENGTH	255		// Lunghezza massima del nome di un file/direcory
#define MAXFILELINELEN		1024	// Lunghezza massima di una riga riga di un file di testo

struct config stConfig;

/*
	Cosa fa			:			Gestisce il funzionamento del programma in base i parametri (comandi e opzioni)
								inseriti dall'utente
*/
void scanProject(char *sCommand,char *sPath){
	char *aCommands[COMMANDSCOUNT];

	aCommands[0] = "--help";
	aCommands[1] = "--get-row-count";
	aCommands[2] = "--get-project-cost";
	aCommands[3] = "--get-languages";

	// Carico le configurazioni dell'utente
	stConfig = loadConf();

	// Gestisco la richiesta dell'utente in base al comando che ha inserito
	if(strcmp(sCommand,aCommands[0]) == 0)          printHelp();
	else if(strcmp(sCommand,aCommands[1]) == 0)     printRowCount(sPath);
	else if(strcmp(sCommand,aCommands[2]) == 0)     printProjectCost(sPath);
	else if(strcmp(sCommand,aCommands[3]) == 0)     printLanguages(sPath);
	else                                            unknownCommand();
}

/*
	Cosa fa     :     Stampa nello stdout l'help del programma
*/
void printHelp(void){
	fprintf(stdout,"                                   )                (          \n");
	fprintf(stdout,"        (         (     (       ( /(      (         )\\ )       \n");
	fprintf(stdout," `  )   )(    (   )\\   ))\\  (   )\\())___  )\\   (   (()/(   (   \n");
	fprintf(stdout," /(/(  (()\\   )\\ ((_) /((_) )\\ (_))/|___|((_)  )\\ ) /(_))  )\\  \n");
	fprintf(stdout,"((_)_\\  ((_) ((_)  ! (_))  ((_)| |_       (_) _(_/((_) _| ((_) \n");
	fprintf(stdout,"| '_ \\)| '_|/ _ \\ | |/ -_)/ _| |  _|      | || ' \\))|  _|/ _ \\ \n");
	fprintf(stdout,"| .__/ |_|  \\___/_/ |\\___|\\__|  \\__|      |_||_||_| |_|  \\___/ \n");
	fprintf(stdout,"|_|             |__/                                           \n");
	fprintf(stdout, "project-info is a tool developed by Luca Bertoni that helps you\n");
	fprintf(stdout, "to get info about your projects. You can get the total amount of rows\n");
	fprintf(stdout, "of your source code files. You can know the total size of your project (in MB).\n");
	fprintf(stdout, "Read below to discover all you can do!\n");
	fprintf(stdout, "Usage: project-info <command> <path>\n");
	fprintf(stdout, "\t<command>\n");
	fprintf(stdout, "\t\t--help :\n");
	fprintf(stdout, "\t\t\tShow this manual\n");
	fprintf(stdout, "\t\t--get-row-count :\n");
	fprintf(stdout, "\t\t\tGet the amount of rows of your project (ex: source code)\n");
	fprintf(stdout, "\t\t--get-project-cost :\n");
	fprintf(stdout, "\t\t\tGet an approximate cost of the project. This calculation uses an average-writing-time\n");
	fprintf(stdout, "\t\t\tof 20 seconds for each line of text in the project, and a hourly cost of 20 (euro/USD/...)\n");
	fprintf(stdout, "\t\t\tYou can configure this values in setting file (project-info.conf)\n");
	fprintf(stdout, "\t<path>\n");	
	fprintf(stdout, "\t\tPath of the folder you want to scan (without the / in the end). Ex: folder\n");
	fprintf(stdout, "\t\tThis will scan the directory called 'folder'\n");	
	fprintf(stdout, "\nContacts\n");
	fprintf(stdout, "\tEmail:\n");
	fprintf(stdout, "\t\tluca.bertoni24@gmail.com\n");
	fprintf(stdout, "\tFacebook profile:\n");
	fprintf(stdout, "\t\thttps://www.facebook.com/LucaBertoniLucaBertoni\n");
	fprintf(stdout, "\tGithub profile:\n");
	fprintf(stdout, "\t\thttps://github.com/lucabertoni\n");  
}

/*
	Cosa fa			:			Stampa a schermo il numero totale di righe presenti nei file della cartella e sottocartelle
								specificata
	sPath			:			char, percorso della cartella da scansionare
*/
void printRowCount(char *sPath){
	int nTotaleRighe;

	fprintf(stdout, "I'm counting the lines, please wait...\n");
	nTotaleRighe = getRowCount(sPath);

	if (nTotaleRighe == -1)
		fprintf(stdout, "Error occurred during scan. Send log to luca.bertoni24@gmail.com to get support\n");
	else
		fprintf(stdout, "Total of lines: %d\n",nTotaleRighe);
}

/*
	Cosa fa			:			Apre un file in sola lettura e ne conta le righe al suo interno
	sFilePath		:			char, percorso del file da scansionare
	Ritorna			:			nRowCount -> intero, numero delle righe presenti nel file
*/
int getFileRowCount(char *sFilePath){
    FILE *pFile;					// Puntatore al file da scansionare
    int nRowCount;					// Variabile che conterrà il conteggio delle righe del file
    char sLine[MAXFILELINELEN];		// Variabile puramente inutile se non per il fatto che serve per
    								// poter scansionare il file
    // Imposto di default che non ci sono righe nel file
    nRowCount = 0;

    // Apro il file in sola lettura
    pFile = fopen (sFilePath, "r");

    // Se il file è stato aperto correttamente allora passo a contare le righe
    if (pFile!=NULL)
    	// Ciclo ed estraggo righe fino a quando non arrivo alla fine del file
    	while (fgets(sLine, sizeof(sLine), pFile)) ++nRowCount;

    // Chiudo il file che ho aperto in precedenza
    fclose(pFile);

    return nRowCount;
}

/*
	Cosa fa			:			Estrae il numero di righe presenti nei file della cartella (e sottocartelle) indicata
	sPath			:			char, perorso della cartella da analizzare
	Ritorna			:			nTotale -> intero, numero delle righe totali presenti nei vari file,
								oppure -1 in caso di errore
*/
int getRowCount(char *sPath){
	int nTotale,nApp;
	char sName[MAXFILENAMELENGTH];
	char sFilePath[MAXFILENAMELENGTH];

	nTotale = nApp = 0;

	DIR *pDirectory;		// 	Puntatore alla directory
	struct dirent *ep;		// 	Struttura che conterrà i valori generati da readdir, così formata:
							//	ino_t  d_ino       file serial number
							//	char   d_name[]    name of entry

	struct stat sb;     /*  struttura che contiene le informazioni sui file, cosi formata:
	                        struct stat {
	                            dev_t     st_dev;     // ID of device containing file
	                            ino_t     st_ino;     // inode number
	                            mode_t    st_mode;    // protection
	                            nlink_t   st_nlink;   // number of hard links
	                            uid_t     st_uid;     // user ID of owner
	                            gid_t     st_gid;     // group ID of owner
	                            dev_t     st_rdev;    // device ID (if special file)
	                            off_t     st_size;    // total size, in bytes
	                            blksize_t st_blksize; // blocksize for file system I/O
	                            blkcnt_t  st_blocks;  // number of 512B blocks allocated
	                            time_t    st_atime;   // time of last access
	                            time_t    st_mtime;   // time of last modification
	                            time_t    st_ctime;   // time of last status change
	                        };
	                    */
	// Apro la cartella
	pDirectory = opendir (sPath);

	// Se non ci sono stati errori durante l'apertura continuo ad analizzare la cartella...
	if (pDirectory != NULL){
		// Estraggo tutti i nomi di file/cartelle presenti e li gestisco a mio piacere
		while (ep = readdir (pDirectory)){

			// Inizializzo il percorso, ex: folder
			strcpy(sFilePath,sPath);
			strcat(sFilePath,"/");

			strcpy(sName,ep->d_name);

			// Concateno al percorso il nome del file
			strcat(sFilePath,sName);

			// Se il nome del file è . oppure .. ignoro
			if (strcmp(sName,".") == 0 || strcmp(sName,"..") == 0) continue;

			printf("Scanning: %s\n", sFilePath);

			// Provo ad estrarre le informazioni del file e se fallisce passo al file successivo
			if (stat(sFilePath, &sb) == -1) continue;

			// Se è una cartella richiamo questa stessa funzione passandogli come parametro il percorso della sotto cartella...
			if(S_ISDIR(sb.st_mode)){
				if ((nApp = getRowCount(sFilePath)) != -1)
					nTotale += nApp;
			}else{	// ... Altrimenti scansiono il file e conto il numero di righe
				nTotale += getFileRowCount(sFilePath);
			}
		}

		closedir (pDirectory);
	}else{	// ... Altrimenti ritorno -1

		return -1;
	}

	return nTotale;
}

/*
	Cosa fa			:			Stampa a schermo il costo attuale del progetto
	sPath			:			char, percorso della cartella da scansionare
*/
void printProjectCost(char *sPath){
	float nProjectCost;

	fprintf(stdout, "Calculating project cost...\n");

	nProjectCost = getProjectCost(sPath);

	if (nProjectCost == -1)
		fprintf(stdout, "Error occurred during scan. Send log to luca.bertoni24@gmail.com to get support\n");
	else
		fprintf(stdout, "Project cost: %f\n",nProjectCost);
}

/*
	Cosa fa			:			Calcola il costo attuale del progetto
	sPath			:			char, percorso della cartella da scansionare
*/
int getProjectCost(char *sPath){
	int nRowCount;
	float nProjectCost;

	nProjectCost = 0;

	/*
		Cosa fa			:			Estrae il numero di righe presenti nei file della cartella (e sottocartelle) indicata
		sPath			:			char, perorso della cartella da analizzare
		Ritorna			:			nTotale -> intero, numero delle righe totali presenti nei vari file,
									oppure -1 in caso di errore
	*/
	nRowCount = getRowCount(sPath);
	if (nRowCount == -1)
		return -1;

	nProjectCost = (float)(((float)((float)((float)(nRowCount*stConfig.nAverageWritingTime))/60)/60)*stConfig.nHourlyCost);

	return nProjectCost;
}
/*
	Cosa fa			:			Incrementa il valore associato ad un linguaggio(e non) di nIncremento.
								Se il linguaggio(e non) non esiste, viene aggiunto 
	sLinguaggio		:			stringa, linguaggio di programmazione di cui incrementare il valore
	nIncremento		:			intero -> incremento da assegnare al linguaggio
	stLanguages		:			struct stLanguages *, struttura alla quale incrementare il linguaggio
	Ritorna			:			aRet -> struct stLanguages *, struttura con i valori aggiornati
*/
struct stLanguages *incrementaLinguaggio(char* sLinguaggio,int nIncremento,struct stLanguages *aLanguages){
//	aLanguages[0].sLinguaggio = (char*) malloc(sizeof(char*));
	int i,bIncrementato;
	struct stLanguages *aRet;

	// Copio la struct attuale in quella di ritorno
	aRet = malloc(sizeof(aLanguages));

	// Copia il contenuto del primo elemento all'interno del secondo
	aRet = aLanguages;

	// Ciclo fino a quando trovo il linguaggio da incrementare oppure fino alla fine dell'array
	i = 0;
	bIncrementato = 0;	// 0 -> valore non incrementato => linguaggio NON presente nell'array => devo crearlo e
						// 		poi assegnargli il valore
						// 1 -> valore incrementato => linguaggio presente nell'array
	while(aRet[i].sLinguaggio){
		// Se il linguaggio corrisponde con un elemento dell'array ne incremento il valore
		if(strcmp(aRet[i].sLinguaggio,sLinguaggio) == 0){
			aRet[i].nTotale += nIncremento;
			bIncrementato = 1;
			break;
		}
	++i;
	}


	// Se non ho incrementato il valore è perchè il linguaggio non è presente nell'array
	// Allora devo aggiungerlo e poi assegnargli il valore
	if (!bIncrementato){
		// Aggiungo uno spazio in più in memoria
		aRet = (struct stLanguages*) realloc (aRet, (++i * sizeof(struct stLanguages)));

		// Alloco la memoria necessaria per contenere il nome del linguaggio
		aRet[i-1].sLinguaggio = (char*) malloc(sizeof(char*));

		// Assegno i valori al nuovo linguaggio
		aRet[i-1].sLinguaggio = sLinguaggio;
		aRet[i-1].nTotale = nIncremento;
	}

	return aRet;
}

// Cosa fa			:			Traduce un'estensione nel suo linguaggio/nome corrispondente, es:
//								(.)py -> Python
// sEstensione		:			stringa, estensione da tradurre, es: py (senza il punto)
// Ritorna			:			sRet -> stringa, traduzione dell'estensione.
//								Nel caso non sia presente ritorna "Unknown"
char* estensione2Linguaggio(char* sEstensione){
	char* sRet;

	stringToUpper(sEstensione);
	if(strcmp(sEstensione,"C") == 0) sRet = "C";
	else if((strcmp(sEstensione,"O") == 0) || (strcmp(sEstensione,"OUT") == 0)) sRet = "C";
	else if((strcmp(sEstensione,"H") == 0)) sRet = "C";
	else if((strcmp(sEstensione,"CONF") == 0)) sRet = "Config file";
	else sRet = "Unknown";

	return sRet;
}

/*
	Cosa fa			:			Estrae la lista di linguaggi(e non) utilizzati nel progetto, insieme al
								numero totale con il quale sono presenti
	sPath			:			stringa, percorso della cartella del progetto
	Ritorna			:			aLanguages -> struct stLanguages *, contiene la lista dei linguaggi-totali
								Vuoto in caso di errore o nessun file presente
*/
struct stLanguages *getLanguages(char *sPath){
	int nPos,i,i29;
	nPos = 0;

	// Conterrà il file esploso (nome, estensione(ultimo elemento))
	char** aFile;

	// Conterrà l'estensione del file
	char* sEstensione;

	// Nome del linguaggio associato all'estensione del file
	char* sLinguaggio;

	// Strutta di appoggo che conterrà i dati in caso di ricorsione
	struct stLanguages *aApp;

	char sName[MAXFILENAMELENGTH];
	char sFilePath[MAXFILENAMELENGTH];

	// Array di struct di ritorno
	struct stLanguages *aLanguages = malloc((sizeof(struct stLanguages)));

	DIR *pDirectory;		// 	Puntatore alla directory
	struct dirent *ep;		// 	Struttura che conterrà i valori generati da readdir, così formata:
							//	ino_t  d_ino       file serial number
							//	char   d_name[]    name of entry

	struct stat sb;     /*  struttura che contiene le informazioni sui file, cosi formata:
	                        struct stat {
	                            dev_t     st_dev;     // ID of device containing file
	                            ino_t     st_ino;     // inode number
	                            mode_t    st_mode;    // protection
	                            nlink_t   st_nlink;   // number of hard links
	                            uid_t     st_uid;     // user ID of owner
	                            gid_t     st_gid;     // group ID of owner
	                            dev_t     st_rdev;    // device ID (if special file)
	                            off_t     st_size;    // total size, in bytes
	                            blksize_t st_blksize; // blocksize for file system I/O
	                            blkcnt_t  st_blocks;  // number of 512B blocks allocated
	                            time_t    st_atime;   // time of last access
	                            time_t    st_mtime;   // time of last modification
	                            time_t    st_ctime;   // time of last status change
	                        };
	                    */
	// Apro la cartella
	pDirectory = opendir (sPath);

	// Se non ci sono stati errori durante l'apertura continuo ad analizzare la cartella...
	if (pDirectory != NULL){
		// Estraggo tutti i nomi di file/cartelle presenti e li gestisco a mio piacere
		while (ep = readdir (pDirectory)){

			// Inizializzo il percorso, ex: folder
			strcpy(sFilePath,sPath);
			strcat(sFilePath,"/");

			strcpy(sName,ep->d_name);

			// Concateno al percorso il nome del file
			strcat(sFilePath,sName);

			// Se il nome del file è . oppure .. ignoro
			if (strcmp(sName,".") == 0 || strcmp(sName,"..") == 0) continue;

			// Provo ad estrarre le informazioni del file e se fallisce passo al file successivo
			if (stat(sFilePath, &sb) == -1) continue;

			// Se è una cartella richiamo questa stessa funzione passandogli come parametro il percorso della sotto cartella...
			if(S_ISDIR(sb.st_mode)){
				// Estraggo il totate dei linguaggi dalla sottocartella
				aApp = getLanguages(sFilePath);

				// Se ho estratto almeno un linguaggio lo salvo nell'array di ritorno
				if(aApp[0].sLinguaggio){
					i = 0;
					// Ciclo su ogni elemento per copiarlo nell'array di ritorno
					while(aApp[i].sLinguaggio){
						// Incremento l'array di ritorno con i totali dei linguaggi estratti dalla sottocartella
						aLanguages = incrementaLinguaggio(aApp[i].sLinguaggio,aApp[i].nTotale,aLanguages);
						++i;
					}

					free(aApp);
				}
			}else{	// ... Altrimenti incremento di uno il linguaggio corrispondente al file
				/*
					Cosa fa			:			Esplode una stringa su un carattere
					sLine			:			char* (array di caratteri/stringa), stringa da esplodere
					cSeparatore		:			char, carattere sul quale esplodere la stringa
					Ritorna			:			aRet -> char** (array di stringhe), contiene tutti gli elementi esplosi
				*/
				// Esplodo la stringa sul punto e 
				aFile = explode(sName,'.');
				i29 = 0;
				while(aFile[i29]) ++i29;

				sEstensione = aFile[i29-1];

				sLinguaggio = estensione2Linguaggio(sEstensione);

				// Incremento di uno il linguaggio
				aLanguages = incrementaLinguaggio(sLinguaggio,1,aLanguages);

				// Rilascio la variabile
				free(aFile);
			}
		}

		closedir (pDirectory);
	}else{	// ... Altrimenti, in caso di erore ritorno l'array vuoto
		return aLanguages;
	}

	return aLanguages;
}

/*
	Cosa fa			:			Stampa a schermo la percentuale dei vari linguaggi di programmazione e non
	sPath			:			char, percorso della cartella da scansionare
*/
void printLanguages(char *sPath){
	struct stLanguages *aLanguages;

	fprintf(stdout, "Calculating percentage of languages ...\n");

	
	aLanguages = getLanguages(sPath);

	if (!(aLanguages[0].sLinguaggio))
		fprintf(stdout, "No files found in %s\n",sPath);
	else
		printf("Language - Total - Percentages\n");
}

/*
	Cosa fa			:			Esegue il parse della riga file di configurazione basandosi sul
								carattere separatore '='
	sConfLine		:			*char(array di caratteri), riga di cui fare il parse es:
								AVERAGEWRITINGTIME=15
	Ritorna			:			aRet -> array di strighe, es:
									[0] => AVERAGEWRITINGTIME
									[1] => 15
*/
char** parseConf(char *sConfLine){
	char separatore;
	separatore = '=';
	char **aRet = NULL;

	aRet = explode(sConfLine,separatore);

	return aRet;
}

/*
	Cosa fa			:			Carica il file di configurazione con ciò di cui necessita
								Il path del file è fisso (.\project-info.conf)	
	Ritorna			:			struct config, struttura contenente le configurazioni necessarie
*/
struct config loadConf(){
	const char sConfPath[] = "project-info.conf";
	char sLine[MAXFILELINELEN];
	char **aSettings = NULL;
	struct config structConfig;

	// Puntatore al file delle configurazioni
	FILE *pFile;

	structConfig.nAverageWritingTime = 15;	// Tempo medio di scrittura di una riga di testo
	structConfig.nHourlyCost = 20;			// Costo orario del detentore del progetto

	// Apro il file delle configurazioni in modalià lettura
	pFile = fopen (sConfPath, "r");

	// Se il file non è stato aperto stampo un messaggio d'errore e termino l'esecuzione
	if (pFile==NULL){
		printf("Error occurred reading conf file.\n");
		exit(0);
	}

	// Leggo il file riga per riga
	while (fgets(sLine, sizeof(sLine), pFile)){
		aSettings = parseConf(sLine);
		if (strcmp(aSettings[0],"AVERAGEWRITINGTIME") == 0)	structConfig.nAverageWritingTime = atof(aSettings[1]);
		else if(strcmp(aSettings[0],"HOURLYCOST") == 0)	structConfig.nHourlyCost = atof(aSettings[1]);
	}

	return structConfig;
}

/*
	Cosa fa			:			Stamnpa a schermo che il comando inserito non è valido
*/
void unknownCommand(void){
	fprintf(stderr, "Unknown command, read the manual using --help command\n");
}
