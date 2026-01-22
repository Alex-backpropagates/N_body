# Options de compilation
# (infos debug, tous warnings, standard C99, dossier include)
CFLAGS=-g -Wall -Wextra -std=c99 -Iinclude

# Options d'édition des liens (pour utiliser math.h)
LDLIBS=-lm

# Identification des exécutables à générer à partir des fichiers de test
EXE=$(patsubst test/%.c, bin/%, $(wildcard test/*.c))

# Identification de tous les fichiers objets associés aux fichiers sources
ALL_OBJ=$(patsubst src/%.c, obj/%.o, $(wildcard src/*.c))

# Règle par défaut : générer tous les exécutables
all: $(EXE)

# Règle générique pour générer les fichiers objets pour les sources (.c -> .o)
obj/%.o: src/%.c
	gcc $(CFLAGS) -c $^ -o $@

# Règle générique pour générer les fichiers objets pour les tests (.c -> .o)
obj/test_%.o: test/test_%.c
	gcc $(CFLAGS) -c $^ -o $@

# Règle générique pour générer les exécutables de test (.o -> executable)
bin/test_%: $(ALL_OBJ) obj/test_%.o
	gcc $^ $(LDLIBS) -o $@

# Règle de nettoyage
clean:
	rm -rf bin obj
	mkdir bin obj
