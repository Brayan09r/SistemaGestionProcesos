Proceso GestorDeMemoria
    Dimension pila[100]
    Definir tope, opcion, i Como Entero
    tope <- 0
	
    Repetir
        Escribir "GESTOR DE MEMORIA:"
        Escribir "1. Asignar memoria (Push)"
        Escribir "2. Liberar memoria (Pop)"
        Escribir "3. Mostrar pila"
        Escribir "0. Salir"
        Leer opcion
		
        Segun opcion Hacer
            1:
                Si tope < 100 Entonces
                    tope <- tope + 1
                    Escribir "Ingrese nombre del proceso que usará memoria:"
                    Leer pila[tope]
                Sino
                    Escribir "Memoria llena"
                FinSi
				
            2:
                Si tope >= 1 Entonces
                    Escribir "Se liberó memoria del proceso: ", pila[tope]
                    tope <- tope - 1
                Sino
                    Escribir "No hay memoria para liberar"
                FinSi
				
            3:
                Si tope >= 1 Entonces
                    Para i <- tope Hasta 1 Con Paso -1
                        Escribir pila[i]
                    FinPara
                Sino
                    Escribir "La pila está vacía"
                FinSi
				
            0:
                Escribir "Saliendo..."
        FinSegun
    Hasta Que opcion = 0
FinProceso