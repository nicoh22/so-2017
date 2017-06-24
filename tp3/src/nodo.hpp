#ifndef _NODO_H
#define _NODO_H

#define BUFFER_SIZE 1024

/* Función que maneja un nodo.
 * Recibe el rank del nodo.
 */
void nodo(unsigned int rank);
/* Comandos que recibe el nodo (desde la consola)
 * */
void nodoLoad(char *data);
void nodoAdd(char *data);
void nodoMember(char *data);
void nodoMaximum();
void nodoQuit();

/* Simula un tiempo de procesamiento no determinístico.
 */
void trabajarArduamente();

#endif  /* _NODO_H */
