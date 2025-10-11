# Sistemas_Operativos-Javeriana_2025-30



Bienvenido a mi repositorio!!!!! En este repositorio se recogen los **códigos correspondiente a talleres, laboratorios y ejercicios de la materia de Sistemas Operativos** para la Pontificia Universidad Javeriana, periodo 2025-30, creados por Guillermo Aponte.

---

## Descripción

Este repositorio guarda todos programas en lenguaje C,realizados como ejercicios para reforzar el aprendizaje corespondiente a los temas vistos durante las clases magistrales de la materia de sistemas operativos. Sirve como portafolio de laboratorio y código fuente entregable para la asignatura.

---

## Requisitos

Para compilar y ejecutar este proyecto necesitas:  

- Un sistema Unix / Linux / macOS (o WSL si estás en Windows)  
- GCC (compilador de C, por ejemplo `gcc`)  
- Make (para usar el `Makefile`)  

---

## Resumen de contenidos

| Nombre del fichero              | Tema trabajado                              |
|---------------------------------|---------------------------------------------|
| Taller00_Aponte                 | Compilacion modular                         |
| Taller01_Aponte                 | Memoria dinámica, concurrencia y rendimiento|
| laboratorioRendimiento_Aponte   | Memoria dinámica, concurrencia y rendimiento|
| labForkPipe_Aponte              | POSIX, fork() y pipe()                      | 

---

## Desarrollo de contenidos

### Taller00_Aponte 

Taller referente a compilación modular, titulado como taller 0 para mantener orden en el repositorio  
  

Código realizado en clase, como parte del proceso de aprendizaje para compilar un programa de manera modular.  
Se hicieron algunas modificaciones para que el programa compilara correctamente:  

- Se cambio la manera en que se declaraban las matrices en las funciones, incluyendo apuntadores para que estas se manejaran con mamoria dinámica.  
- Se cambió la manera en que se declaraban las matrices en el main, usando la función malloc para darles memoria dinámica y declarándolas con apuntadores.  
- Se incluyeron nuevos printf en el main, para tener mas claridad en lo que se estaba imprimiento por consola. 

#### Archivos del fichero

- makefile: automatización para la compilación del código y eliminación de ejecutables y objetos.
- modulo.h: prototipos de funciones
- funciones.c: implementacion de funciones 
- main.c: desarrollo del main del programa.
---

### Taller01_Aponte 

Taller referente a memoria dinámica y rendimiento, iniciado en clase, terminado y perfeccionado aparte.  
  

Código realizado en clase, como parte del proceso de aprendizaje sobre rendimiento y manejo de memoria dinámica usando apuntadores, el código se encuentra debidamente comentariado para fomentar el proceso de aprendizaje, explicando y comprendiendo la totalidad del mismo.    

Se hicieron algunas mejoras para que el programa compilara correctamente:  

- En reto01, se añade la función malloc para *p y *q, pues sin esto el código no compilaba correctamente, posteriormente se libera la memoria apartada por malloc, en la linea donde se imprimía la dirección de p, se reemplaza %s, por %p, pues %s imprime caracteres en lugar de direcciones de memoria, por lo tanto la impresión resultaria incorrecta  
- En reto02 en la linea donde se imprimía la dirección de q, se reemplaza %s, por %p, pues %s imprime caracteres en lugar de direcciones de memoria, por lo tanto la impresión resultaria incorrecta, además se añadió la función free() al final para liberar memoria   
- En reto04 se devolvía código de error 0 cuando fallaba calloc, se reemplazó por 1, para tener mejor manejo de esto y diferenciarse del main  
- En reto05 faltaba la parte final del main, se agregó para que compile correctamente.

#### Archivos del fichero

- reto01.c: Primer programa con las temáticas tratadas
- reto02.c: Segundo programa con las temáticas tratadas
- reto03.c: Tercer programa con las temáticas tratadas
- reto04.c: Cuarto programa con las temáticas tratadas
- reto05.c: Quinto programa con las temáticas tratadas

---

### laboratorioRendimiento_Aponte 

Laboratorio referente a memoria dinámica y rendimiento, iniciado en clase, terminado y perfeccionado aparte.  
  

Código realizado en clase, como parte del proceso de aprendizaje sobre rendimiento y manejo de memoria dinámica usando apuntadores, el código se encuentra debidamente comentariado, para fomentar el proceso de aprendizaje, explicando y comprendiendo la totalidad del mismo.    

Se hicieron algunas mejoras para que el programa compilara correctamente:  

- En reto01, se añade la función malloc para *p y *q, pues sin esto el código no compilaba correctamente, posteriormente se libera la memoria apartada por malloc, en la linea donde se imprimía la dirección de p, se reemplaza %s, por %p, pues %s imprime caracteres en lugar de direcciones de memoria, por lo tanto la impresión resultaria incorrecta  
- En reto02 en la linea donde se imprimía la dirección de q, se reemplaza %s, por %p, pues %s imprime caracteres en lugar de direcciones de memoria, por lo tanto la impresión resultaria incorrecta, además se añadió la función free() al final para liberar memoria   
- En reto04 se devolvía código de error 0 cuando fallaba calloc, se reemplazó por 1, para tener mejor manejo de esto y diferenciarse del main  
- En reto05 faltaba la parte final del main, se agregó para que compile correctamente.

#### Archivos del fichero

- makefile: automatización para la compilación del código y eliminación de ejecutables y objetos.
- modulo.h: prototipos de funciones
- modulo.c: implementacion de funciones 
- labRendiAponte.c: desarrollo del main del programa.

---

### labForkPipe_Aponte

Laboratorio referente a los procesos de enseñanza en clase sobre POSIX, en donde se trataron temas como la creación de múltiples procesos utilizando la función fork() y la comunicación de estos utilizando pipe(). El código presente en este fichero cuenta con documentación exhaustiva, como parte del proceso de aprendizaje, buscando aumentar la comprensión de los temas tratados durante la clase

### Archivos del fichero

- lab01_Aponte.c: Primer programa, mostrando el ID de los procesos duplicados con fork()
- lab02_Aponte.c: Segundo programa, mostrando como los procesos duplicados con fork() pueden realizar tareas diferentes
- lab02_Aponte.c: Tercer programa, mostrando como se comunican los procesos duplicados con fork() utilizando pipe
- makefile: automatización para la compilación del código y eliminación de ejecutables y objetos.