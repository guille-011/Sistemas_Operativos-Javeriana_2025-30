#############################################################################################
# Fecha: 11/09/2025
# Autor: Guillermo Aponte
# Materia: Sistemas Operativos
# Tema: Concurrencia
# Objetivo: Hacer una metodología para la implementación de la multiplicación de matrices.
# La idea principal, es construir paso a paso la implementación para hacer uso de la
# biblioteca PTHREAD. Se implementa el Algoritmo Clásico de multiplicación de matrices, para
# matrices cuadradas, es decir, la dimensión de filas es igual a la de columnas.
# A continuación se describen los paso
#  - Reserva de memoria
#      - Creación de punteros para matrices del tipo doble
#      - Asignación de memoria
#      - Ingreso de argumentos de entrada (Dimensión matriz, número de hilos)
#      - Validación argumentos de entrada
#      - Inicializar las matrices
#     - Imprimir las matrices
#      - Función para inicializar las matrices
#      - Función para imprimir las matrices
#      - Algoritmo clásico de multiplicación matrices
#  - Se verifica el resultado
#      - Función para multiplicación las matrices
#      - Declaración vector de hilos
#      - Creación de hilos según tamaño de vector de hilos
#      - Crear estructura de datos que encapsule los argumentos de entrada de la función MM
# Implementación de paralelismo: resolución de la multiplicación de matrices
#  - Se crea el vector de hilos
#  - Se tiene pendiente la exclusión de los hilos
#  - Se pasa a globales las matrices
#  - Encapsular los datos para enviarlos a la función MxM
#  - Se desencapsulan los datos dentro de la función MxM (descomprimen)
##########################################################################################



#En el archivo makefile se compila el programa labRendiAponte, el cuál coresponde al programa
#de matrices desarrollado durante la clase del 11 de septiembre
#se crea la funcion que compilará el programa y creará el objeto modulo
#se crea la funcionalidad encargada de borrar los compilables y el objeto
GCC = gcc

MODULO = modulo

PROGRAMAS = labRendiAponte


labRendiAponte:
	$(GCC) $(MODULO).c -c 
	$(GCC) $@.c $(MODULO).o -o $@
clear:
	$(RM) $(PROGRAMAS) $(MODULO).o
