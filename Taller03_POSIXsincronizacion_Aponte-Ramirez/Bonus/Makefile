#*******************************************************************
# Pontificia Universidad Javeriana
# Facultad de Ingeniería
# Carrera: Ingeniería de Sistemas
# Autores: Guillermo Aponte, Daniel Ramírez
# Materia: Sistemas Operativos
# Tema: Makefile 
# Archivo: Makefile
# Descripcion: Automatiza la compilación modular de varios ficheros 
#			   Teniendo en cuenta la estructura de directorios,
#			   es decir, directorios de modulos y de ficheros main
# Fecha: 14/11/25
#*******************************************************************


GCC = gcc
FLAGS = -pthread -lrt
MODULO = moduloConcurrenciaPosix
EJECUTABLES = Ejecutables
# Carpeta donde se guardarán los ejecutables y objetos


PROGRAMAS = concurrenciaPOsix

# Regla principal
all: $(PROGRAMAS)

concurrenciaPOsix:
	mkdir -p $(EJECUTABLES)
	$(GCC) -c $(MODULO).c -o $(EJECUTABLES)/$(MODULO).o
	$(GCC) $@.c $(EJECUTABLES)/$(MODULO).o -o $(EJECUTABLES)/$@ $(FLAGS)

clean:
	rm -rf $(EJECUTABLES)
