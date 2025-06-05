Proceso ColaDePrioridad
		Dimension id[100], nombre[100], prioridad[100]
		Definir n, i, j, auxEntero Como Entero
		Definir auxTexto Como Cadena
		n <- 1
		
		Repetir
			Escribir "Ingrese ID del proceso:"
			Leer id[n]
			Escribir "Ingrese nombre:"
			Leer nombre[n]
			Escribir "Ingrese prioridad:"
			Leer prioridad[n]
			
			Escribir "¿Desea ingresar otro proceso? (1=Sí / 0=No):"
			Leer i
			Si i = 1 Entonces
				n <- n + 1
			FinSi
		Hasta Que i = 0
		
		// Ordenar por prioridad descendente (mayor prioridad primero)
		Para i <- 1 Hasta n - 1
			Para j <- i + 1 Hasta n
				Si prioridad[i] < prioridad[j] Entonces
					auxEntero <- prioridad[i]
					prioridad[i] <- prioridad[j]
					prioridad[j] <- auxEntero
					
					auxEntero <- id[i]
					id[i] <- id[j]
					id[j] <- auxEntero
					
					auxTexto <- nombre[i]
					nombre[i] <- nombre[j]
					nombre[j] <- auxTexto
				FinSi
			FinPara
		FinPara
		
		Escribir "Cola de prioridad:"
		Para i <- 1 Hasta n
			Escribir "ID:", id[i], " - Nombre:", nombre[i], " - Prioridad:", prioridad[i]
		FinPara
FinProceso

