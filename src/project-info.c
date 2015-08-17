#include "project-info.h"

#define COMMANDSCOUNT   2       // Numero di comandi presenti nel programma (aumentare in base alla necessità)

void scanProject(char *sCommand,char *sPath){
	char *aCommands[COMMANDSCOUNT];

	aCommands[0] = "--help";
	aCommands[1] = "--get-row-count";

	if(strcmp(sCommand,aCommands[0]) == 0)          printHelp();
	else if(strcmp(sCommand,aCommands[1]) == 0)     getRowCount(sPath);
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
	fprintf(stdout, "\nContacts\n");
	fprintf(stdout, "\tEmail:\n");
	fprintf(stdout, "\t\tluca.bertoni24@gmail.com\n");
	fprintf(stdout, "\tFacebook profile:\n");
	fprintf(stdout, "\t\thttps://www.facebook.com/LucaBertoniLucaBertoni\n");
	fprintf(stdout, "\tGithub profile:\n");
	fprintf(stdout, "\t\thttps://github.com/lucabertoni\n");  
}

int getRowCount(char *sPath){
	fprintf(stdout, "I'm counting the lines, please wait...\n");
}

void unknownCommand(void){
	fprintf(stderr, "Unknown command, read the manual using --help command\n");
}