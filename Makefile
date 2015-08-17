CC=cc

all: project-info

project-info: project-info.o main.o
	$(info Inzio generazione project-info ...)
	@$(CC)  build/main.o  build/project-info.o -o project-info
	$(info 'project-info' generato in './project-info')

main.o:
	$(info Inzio compilazione main.c ...)
	@$(CC) -c main.c -o build/main.o
	$(info 'main.c' compilato in 'build/main.o')

project-info.o:
	$(info Inzio compilazione project-info.c ...)
	@$(CC) -c project-info.c -o build/project-info.o
	$(info 'project-info.c' compilato in 'build/project-info.o')

clean:
	find . -name "*.o" -type f -delete
	rm project-info