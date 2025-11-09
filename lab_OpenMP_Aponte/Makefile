#************************************************************************************
# Pontificia Universidad Javeriana                                                  *
# Fecha: 11 de Noviembre del 2025                                                   *
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
FLAGS = -fopenmp -lm

# Directorio donde se almacenarán los ejecutables
DIRECTORIO = Ejecutables

# Lista de programas a compilar
PROGRAMAS = lab_OMP01 lab_OMP02 lab_OMP03 lab_OMP04 lab_OMP05 lab_OMP06

# Regla general que compila todos los programas
all: $(PROGRAMAS)

# Cada regla crea el directorio si no existe, luego compila su archivo .c y lo deja en Ejecutables/
lab_OMP01:
	mkdir -p $(DIRECTORIO)
	$(GCC) $@.c -o $(DIRECTORIO)/$@ $(FLAGS)

lab_OMP02:
	mkdir -p $(DIRECTORIO)
	$(GCC) $@.c -o $(DIRECTORIO)/$@ $(FLAGS)

lab_OMP03:
	mkdir -p $(DIRECTORIO)
	$(GCC) $@.c -o $(DIRECTORIO)/$@ $(FLAGS)

lab_OMP04:
	mkdir -p $(DIRECTORIO)
	$(GCC) $@.c -o $(DIRECTORIO)/$@ $(FLAGS)

lab_OMP05:
	mkdir -p $(DIRECTORIO)
	$(GCC) $@.c -o $(DIRECTORIO)/$@ $(FLAGS)

lab_OMP06:
	mkdir -p $(DIRECTORIO)
	$(GCC) $@.c -o $(DIRECTORIO)/$@ $(FLAGS)

# Regla para limpiar el directorio de ejecutables
clean:
	$(RM) -rf $(DIRECTORIO)
