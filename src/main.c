/*                                                               
                                   )                (         
        (         (     (       ( /(      (         )\ )      
 `  )   )(    (   )\   ))\  (   )\())___  )\   (   (()/(   (  
 /(/(  (()\   )\ ((_) /((_) )\ (_))/|___|((_)  )\ ) /(_))  )\ 
((_)_\  ((_) ((_)  ! (_))  ((_)| |_       (_) _(_/((_) _| ((_)
| '_ \)| '_|/ _ \ | |/ -_)/ _| |  _|      | || ' \))|  _|/ _ \
| .__/ |_|  \___/_/ |\___|\__|  \__|      |_||_||_| |_|  \___/
|_|             |__/                                          

*/

#include "project-info.h"

#define PATHLEN         1024    // Lunghezza massima del percorso di una cartella
#define COMMANDLEN      100     // Lunghezza massima di un comando (incrementare in base alla necessità)
#define ARGC            3       // Numero di parametri da passare al programma da riga di comando

int main(int argc, char *argv[])
{
  char sPath[PATHLEN];      // Percorso della cartella da analizzare
  char sCommand[COMMANDLEN];   // Comando da eseguire
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

  /*
  Controllo che si stato passato il percorso della cartella da scansionare
  argc è così formato:
    [0] => Nome programma
    [1] => comando da eseguire
    [2] => percorso della cartella da analizzare
  Se non rispetta questi termini stampo l'help nello stdout e chiudo
  */
  if (argc != ARGC) {
    printHelp();
    exit(0);
  }

  strcpy(sPath,argv[2]);
  //sPath = argv[2];
  // Provo ad estrarre le informazioni del file e se fallisce stampo l'errore e termino l'esecuzione
  if (stat(sPath, &sb) == -1) {
      perror("Error");
      exit(0);
  }

  /*
  Se il file è stato analizzato correttamente allora passo a controllare se è una cartella
  In caso negativo, stampo l'errore e termino
  */
  if(!S_ISDIR(sb.st_mode)){
    printf("'%s' is not a folder. Please insert the path of your project folder\n",sPath);
    exit(0);
  }

  // Ho escluso tutte le possibilità di errore, adesso posso iniziare a processare la richiesta

  strcpy(sCommand,argv[1]);
  scanProject(sCommand, sPath);

}