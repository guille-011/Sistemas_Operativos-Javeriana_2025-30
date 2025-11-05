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
| lab_P_C_Aponte                  | Patrón FIFO mediante pipes                  |
| TallerProcesos_Aponte-Ramirez   | POSIX, fork() y pipe()                      |
| Pthread_Lab_Aponte              | Pthread, deadlocks, race conditions         |
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

#### Archivos del fichero

- lab01_Aponte.c: Primer programa, mostrando el ID de los procesos duplicados con fork()
- lab02_Aponte.c: Segundo programa, mostrando como los procesos duplicados con fork() pueden realizar tareas diferentes
- lab02_Aponte.c: Tercer programa, mostrando como se comunican los procesos duplicados con fork() utilizando pipe
- makefile: automatización para la compilación del código y eliminación de ejecutables y objetos.

---

### labP_C_Aponte

Laboratorio referente a los procesos de enseñanza en clase sobre Pipes, en donde se desarrolla el tema de Implementación del patrón Productor-Consumidor usando Pipe Named (FIFO). Los programas actúan como un cliente y un servidor en un esquema de comunicación entre procesos mediante pipes nombrados (FIFOs). Permite recibir mensajes de texto desde un proceso cliente, procesarlos (invertir la cadena) y enviar una respuesta de vuelta al cliente desde el servidor.

#### Archivos del fichero

- cliente.c: Representación del cliente, en donde el usuario envía una cadena de caracteres y espera recibir una respuesta del servidor.
- servidor.c: Representación del servidor, en donde el servidor recibe una cadena de caracteres desde el cliente, la invierte y la vuelve a enciar al cliente.
- makefile: automatización para la compilación del código y eliminación de ejecutables y objetos, compilable con el comando all.

---

### TallerProcesos_Aponte-Ramirez

Taller grupal realizado en compañía de Daniel Ramirez referente a la aplicación en un problema práctico de las funciones fork() y pipe() para la división y comunicación de procesos, para esto se efectuó la creación de múltiples procesos utilizando la función fork() y la comunicación de estos utilizando pipe(). El código presente en este fichero cuenta con documentación exhaustiva, como parte del proceso de aprendizaje, buscando aumentar la comprensión de los temas tratados durante la clase

#### Archivos del fichero

- Informe-TallerProcesos-Aponte_Ramirez.pdf: Informe en formato PDF con toda la información necesaria para la comprensión del taller, incluyendo Marco Teórico, objetivos, resultados y análisis de resultados. 
- makefile: automatización para la compilación del código y eliminación de ejecutables y objetos.
- modulo_procesos.h: Prototipos de la funciones.
- modulo_procesos.c: Implementación de las funciones necesarias para el desarrollo de lo planteado.
- taller_procesos.c: Implementación de la funcion main donde se realiza el llamado a las demás funciones, la división del programa en varios procesos y su comunicación. 

--- 

### Pthread_Lab_Aponte

Laboratorio iniciado en clase y desarrollado como asignación externa, referente a todos los procesos de enseñanza relacionados con los hilos mediante las funciones de pthread provenientes de la libreria POSIX, mostrando la prevención de race conditions y de deadlocks mediante el uso de MUTEX, waits, signals, etc. Los archivos presentes en el fichero contienen la correspondiente documentación exhaustiva de cada cosa realizada por el código, además de ciertas correciones con respecto a lo visto en clase, para mejora el funcionamiento del código.

#### Archivos del fichero

- lab_pthread00_Aponte.c: Primer programa, ejemplo de creación de hilos con pthreads
- lab_pthread01_Aponte.c: Segundo programa, ejemplo de creación de hilos con pthreads
- lab_pthread02_Aponte.c: Tercer programa, ejemplo de mutex para evitar race conditions
- lab_pthread03_Aponte.c: Cuarto programa, ejemplo de mutex y condicionales para evitar race conditions
- lab_pthread04_Aponte.c: Quinto programa, ejemplo de mutex para evitar race conditions
- lab_pthread05_Aponte.c: Sexto programa, ejemplo de race conditions que puede producirse al no usar mutex
- lab_pthread06_Aponte.c: Septimo programa, Correción del programa anterior, implementando mutex
- lab_pthread07_Aponte.c: Octavo programa, ejemplo de race conditions que puede producirse al no usar mutex ni condicionales
- lab_pthread08_Aponte.c: Noveno programa, Correción del programa anterior, implementando mutex y condicionales
- makefile: automatización para la compilación del código y eliminación de ejecutables y objetos.