###############################################
#
# Makefile
# Script para automatizar la compilación de los laboratorios de hilos PTHREAD
# Carrera: Ingeniería en Sistemas
# Autor: Guillermo Aponte
# Materia: Sistemas Operativos
# Fecha: 04/11/2025
# Tema: uso de la libreria PTHREAD
#
###############################################

# Compilador a usar
GCC = gcc

# Flags de compilación (agregar -Wall, -pthread si quieres)
FLAGS = -Wall -pthread

# Directorio donde se guardarán los ejecutables
DIRECTORIO = Ejecutables

# Lista de programas/labs a compilar
PROGRAMAS = lab_pthread00_Aponte \
            lab_pthread01_Aponte \
            lab_pthread02_Aponte \
            lab_pthread03_Aponte \
            lab_pthread04_Aponte \
            lab_pthread05_Aponte \
            lab_pthread06_Aponte \
            lab_pthread07_Aponte \
            lab_pthread08_Aponte

###############################################
# Regla principal: compilar todos los laboratorios
###############################################
all: $(DIRECTORIO) $(PROGRAMAS:%=$(DIRECTORIO)/%)

###############################################
# Crear el directorio de ejecutables si no existe
###############################################
$(DIRECTORIO):
	mkdir -p $(DIRECTORIO)

###############################################
# Regla genérica para compilar cualquier lab
# 
# Explicación:
#   $(DIRECTORIO)/%: %.c
#   - $(DIRECTORIO)/%     -> el archivo ejecutable que se va a generar (comodín % para cualquier nombre)
#   - %.c                 -> el archivo fuente correspondiente en el directorio actual
#   - $<                  -> variable automática que representa el primer archivo dependiente (%.c)
#   - $@                  -> variable automática que representa el archivo que se va a generar ($(DIRECTORIO)/%)
#
###############################################
$(DIRECTORIO)/%: %.c
	$(GCC) $(FLAGS) $< -o $@

###############################################
# Limpieza de ejecutables generados
###############################################
clean:
	rm -f $(DIRECTORIO)/*

