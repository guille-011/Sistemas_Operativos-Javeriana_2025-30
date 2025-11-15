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

FLAGS = -lrt -pthread

DIRECTORIO_ARCHIVOS_MAIN = ficherosMainProductor-Consumidor

MODULOPRODUCTOR = moduloProducer
DIRECTORIO_MODULO_PRODUCTOR = modulosProducer


MODULOCONSUMIDOR = moduloConsumer
DIRECTORIO_MODULO_CONSUMIDOR = modulosConsumer

EJECUTABLES = Ejecutables

PROGRAMAS = producer consumer

all: $(PROGRAMAS)

producer:
	mkdir -p $(EJECUTABLES)
#Compila el modulo y genera un archivo.o
	$(GCC) -c $(DIRECTORIO_MODULO_PRODUCTOR)/$(MODULOPRODUCTOR).c -o $(EJECUTABLES)/$(MODULOPRODUCTOR).o
#Compila el main y enlaza el modulo generando un ejecutable
	$(GCC) $(DIRECTORIO_ARCHIVOS_MAIN)/$@.c $(EJECUTABLES)/$(MODULOPRODUCTOR).o -o $(EJECUTABLES)/$@ $(FLAGS)

consumer:
	mkdir -p $(EJECUTABLES)
#Compila el modulo y genera un archivo.o
	$(GCC) -c $(DIRECTORIO_MODULO_CONSUMIDOR)/$(MODULOCONSUMIDOR).c -o $(EJECUTABLES)/$(MODULOCONSUMIDOR).o
#Compila el main y enlaza el modulo generando un ejecutable
	$(GCC) $(DIRECTORIO_ARCHIVOS_MAIN)/$@.c $(EJECUTABLES)/$(MODULOCONSUMIDOR).o -o $(EJECUTABLES)/$@ $(FLAGS)

clean:
# Elimina el ejecutable y el archivo objeto del módulo
	rm -rf $(EJECUTABLES)