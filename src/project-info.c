#include "project-info.h"	// Includo l'header

// TODO			:			File di configurazione per questi parametri
#define COMMANDSCOUNT   	3       // Numero di comandi presenti nel programma (aumentare in base alla necessità)
#define MAXFILENAMELENGTH	255		// Lunghezza massima del nome di un file/direcory
#define MAXFILELINELEN		1024	// Lunghezza massima di una riga riga di un file di testo
#define HOURLYCOST			20		// Costo orario nella moneta dell'utente per lo sviluppo del progetto
#define AVERAGEWRITINGTIME	15		// Tempo medio in secondi di scrittura di una riga di testo

/*
	Cosa fa			:			Gestisce il funzionamento del programma in base i parametri (comandi e opzioni)
								inseriti dall'utente
*/
void scanProject(char *sCommand,char *sPath){
	char *aCommands[COMMANDSCOUNT];

	aCommands[0] = "--help";
	aCommands[1] = "--get-row-count";
	aCommands[2] = "--get-project-cost";

	if(strcmp(sCommand,aCommands[0]) == 0)          printHelp();
	else if(strcmp(sCommand,aCommands[1]) == 0)     printRowCount(sPath);
	else if(strcmp(sCommand,aCommands[2]) == 0)     printProjectCost(sPath);
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
	fprintf(stdout, "\t\t\tYou can configure this values in setting file (settings.txt)\n");
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


void printProjectCost(char *sPath){
	float nProjectCost;

	fprintf(stdout, "Calculating project cost...\n");

	nProjectCost = getProjectCost(sPath);

	if (nProjectCost == -1)
		fprintf(stdout, "Error occurred during scan. Send log to luca.bertoni24@gmail.com to get support\n");
	else
		fprintf(stdout, "Project cost: %f\n",nProjectCost);
}

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

	nProjectCost = (float)(((float)((float)((float)(nRowCount*AVERAGEWRITINGTIME))/60)/60)*HOURLYCOST);

	return nProjectCost;
}

/*
	Cosa fa			:			Stamnpa a schermo che il comando inserito non è valido
*/
void unknownCommand(void){
	fprintf(stderr, "Unknown command, read the manual using --help command\n");
}