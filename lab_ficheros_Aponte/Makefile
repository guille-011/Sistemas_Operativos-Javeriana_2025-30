#************************************************************************************
# Pontificia Universidad Javeriana                                                  *
# Fecha: 13 de Noviembre del 2025                                                   *
# Autor: Guillermo Aponte                                                           *
# Materia: Sistemas Operativos                                                      *
# Tema: Compilación con Makefile y OpenMP                                           *
# Descripción: Fichero que automatiza la compilación de los laboratorios sobre       *
#              programación paralela usando OpenMP.                                 *
#              Genera un directorio "Ejecutables" donde se almacenan los binarios.  *
#************************************************************************************/

# Compilador a utilizar
GCC = gcc

# Banderas necesarias:
# -fopenmp : habilita el soporte para OpenMP
# -lm      : enlaza la librería matemática (math.h)
# Directorio donde se almacenarán los ejecutables
DIRECTORIO = Ejecutables

# Lista de programas a compilar
PROGRAMAS = lab_ficheros_01 lab_ficheros_02 lab_ficheros_03

# Regla general que compila todos los programas
all: $(PROGRAMAS)

# Cada regla crea el directorio si no existe, luego compila su archivo .c y lo deja en Ejecutables/
lab_ficheros_01:
	mkdir -p $(DIRECTORIO)
	$(GCC) $@.c -o $(DIRECTORIO)/$@ $(FLAGS)

lab_ficheros_02:
	mkdir -p $(DIRECTORIO)
	$(GCC) $@.c -o $(DIRECTORIO)/$@ $(FLAGS)

lab_ficheros_03:
	mkdir -p $(DIRECTORIO)
	$(GCC) $@.c -o $(DIRECTORIO)/$@ $(FLAGS)

# Regla para limpiar el directorio de ejecutables
clean:
	$(RM) -rf $(DIRECTORIO)
