Taller 01 

El código presente en estos archivos corresponde al realizado en clase, con sus debidos comentarios y algunas mejoras para que pueda compilar como es debido:

-En reto01, se añade la función malloc para *p y *q, pues sin esto el código no compilaba correctamente, posteriormente se libera la memoria apartada por malloc, en la linea donde se imprimía la dirección de p, se reemplaza %s, por %p, pues %s imprime caracteres en lugar de direcciones de memoria, por lo tanto la impresión resultaria incorrecta
-En reto02 en la linea donde se imprimía la dirección de q, se reemplaza %s, por %p, pues %s imprime caracteres en lugar de direcciones de memoria, por lo tanto la impresión resultaria incorrecta, además se añadió la función free() al final para liberar memoria
-En reto04 se devolvía código de error 0 cuando fallaba calloc, se reemplazó por 1, para tener mejor manejo de esto y diferenciarse del main
-En reto05 faltaba la parte final del main, se agregó para que compile correctamente.