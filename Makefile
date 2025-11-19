# Compilateur
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -Isrc/backend

# Fichiers sources
SOURCES = src/frontend/menu.c src/backend/tache.c

# Fichiers objets (dans bin/)
OBJECTS = bin/menu.o bin/tache.o

# Nom du programme final
PROGRAMME = bin/main

# Compiler tout le projet
all: $(PROGRAMME)

# Creer l'executable final
$(PROGRAMME): $(OBJECTS)
	mkdir -p bin
	mkdir -p data
	$(CC) $(CFLAGS) -o $(PROGRAMME) $(OBJECTS)
	@echo "Compilation reussie !"

# Compiler menu.c
bin/menu.o: src/frontend/menu.c
	mkdir -p bin
	$(CC) $(CFLAGS) -c src/frontend/menu.c -o bin/menu.o

# Compiler tache.c
bin/tache.o: src/backend/tache.c
	mkdir -p bin
	$(CC) $(CFLAGS) -c src/backend/tache.c -o bin/tache.o

# Nettoyer les fichiers generes
clean:
	rm -rf bin/
	@echo "Nettoyage termine !"

# Lancer le programme
run: all
	./$(PROGRAMME)

# Recompiler tout depuis zero
rebuild: clean all

.PHONY: all clean run rebuild
