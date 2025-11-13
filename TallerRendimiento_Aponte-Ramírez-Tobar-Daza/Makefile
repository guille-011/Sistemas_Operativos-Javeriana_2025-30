#**************************************************************
#          Pontificia Universidad Javeriana
#  Materia: Sistemas Operativos
#  Tema: Multiplicación de matrices con OpenMP (versión modular)
#  Autores: Guillermo Aponte - Juan Daza - Daniel Ramírez - David Tobar
#  Archivo: Makefile
#**************************************************************

GCC = gcc
CFLAGS = -lm
FOPENMP = -fopenmp -O3
POSIX = -lpthread

EJECUTABLES = Ejecutables
PROGRAMAS = calculadoraPromedios mmFilasOpenMP mmClasicaPosix mmClasicaFork mmClasicaOpenMP

MODULOFILASOMP = modulo_filas_openMP
DIRECTORIOFILASOMP = OpenMpFilas

MODULOCLASICOPOSIX = moduloPosix
DIRECTORIOCLASICAPOSIX = ClasicaPosix

MODULOCLASICOOMP = moduloClasicoOpenMP
DIRECTORIOCLASICOOMP = ClasicaOpenMP

MODULOCLASICOFORK = moduloClasicoFork
DIRECTORIOCLASICOFORK = ClasicaFork

all: $(PROGRAMAS)

mmClasicaPosix::
	mkdir -p $(EJECUTABLES)
	$(GCC) $(CFLAGS) -c $(DIRECTORIOCLASICAPOSIX)/$(MODULOCLASICOPOSIX).c -o $(EJECUTABLES)/$(MODULOCLASICOPOSIX).o
	$(GCC) $(CFLAGS) $(DIRECTORIOCLASICAPOSIX)/$@.c $(EJECUTABLES)/$(MODULOCLASICOPOSIX).o -o $(EJECUTABLES)/$@
mmClasicaFork:
	mkdir -p $(EJECUTABLES)
	$(GCC) $(CFLAGS) -c $(DIRECTORIOCLASICOFORK)/$(MODULOCLASICOFORK).c -o $(EJECUTABLES)/$(MODULOCLASICOFORK).o
	$(GCC) $(CFLAGS) $(DIRECTORIOCLASICOFORK)/$@.c $(EJECUTABLES)/$(MODULOCLASICOFORK).o -o $(EJECUTABLES)/$@
mmClasicaOpenMP:
	mkdir -p $(EJECUTABLES)
	$(GCC) $(CFLAGS) -c $(DIRECTORIOCLASICOOMP)/$(MODULOCLASICOOMP).c -o $(EJECUTABLES)/$(MODULOCLASICOOMP).o $(FOPENMP)
	$(GCC) $(CFLAGS) $(DIRECTORIOCLASICOOMP)/$@.c $(EJECUTABLES)/$(MODULOCLASICOOMP).o -o $(EJECUTABLES)/$@ $(FOPENMP)
mmFilasOpenMP:
	mkdir -p $(EJECUTABLES)
	$(GCC) $(CFLAGS) -c $(DIRECTORIOFILASOMP)/$(MODULOFILASOMP).c -o $(EJECUTABLES)/$(MODULOFILASOMP).o $(FOPENMP)
	$(GCC) $(CFLAGS) $(DIRECTORIOFILASOMP)/$@.c $(EJECUTABLES)/$(MODULOFILASOMP).o -o $(EJECUTABLES)/$@ $(FOPENMP)
calculadoraPromedios:
	mkdir -p $(EJECUTABLES)
	$(GCC) $@.c -o $(EJECUTABLES)/$@
clean:
	rm -rf $(EJECUTABLES)
