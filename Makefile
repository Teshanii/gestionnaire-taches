CC = gcc
CFLAGS = -Wall -Wextra -I./frontend -I./backend

# Flags spécifiques Windows pour SDL
ifdef OS
    LDFLAGS = -lmingw32 -lSDLmain -lSDL -lSDL_gfx -lm
else
    LDFLAGS = -lSDL -lSDL_gfx -lm
endif

SRC = main.c \
      backend/tache.c \
      frontend/interface_sdl.c \
      frontend/bouton.c \
      frontend/graphiques.c

OBJ = $(SRC:.c=.o)
EXEC = gestionnaire.exe

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ) $(EXEC)

rebuild: clean all

.PHONY: all clean rebuild
