# Se crea una variable llamada GCC que almacena el compilador
# que se usará para compilar los archivos fuente en C. Esto

GCC = gcc

# Variable que contiene los nombres de los ejecutables que

PROGRAMAS = lab01_Aponte lab02_Aponte lab03_Aponte

# ============================================================
# Regla para compilar lab01_Aponte
# ============================================================
# $@ hace referencia al nombre del objetivo (target), en este
# caso "lab01_Aponte". Se compilará el archivo lab01_Aponte.c
# y se generará el ejecutable con el mismo nombre.


lab01_Aponte:
	$(GCC) $@.c -o $@

# ============================================================
# Regla para compilar lab02_Aponte
# ============================================================
# Se aplica el mismo principio: $@ expande al nombre del target
# y se compila el archivo fuente asociado. Esto evita escribir
# manualmente el nombre en la línea de comando.


lab02_Aponte:
	$(GCC) $@.c -o $@

# ============================================================
# Regla para compilar lab03_Aponte
# ============================================================
# Misma lógica anterior. Cada regla compila de forma individual
# el archivo fuente correspondiente en C.


lab03_Aponte:
	$(GCC) $@.c -o $@


# ============================================================
# Regla para limpiar ejecutables generados
# ============================================================
# La regla "clean" elimina los
# ejecutables compilados. Se utiliza $(RM), que es una variable
# automática de make que ejecuta el comando "rm -f" para borrar
# archivos sin generar errores si no existen.
clean:
	$(RM) $(PROGRAMAS)

