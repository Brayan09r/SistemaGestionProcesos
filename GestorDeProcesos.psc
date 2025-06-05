Algoritmo GestorDeProcesos
	Dimensionar id(100), nombre(100), prioridad(100)
	Definir n, i Como Entero
	n <- 1
	Repetir
		Escribir 'Ingrese ID del proceso:'
		Leer id[n]
		Escribir 'Ingrese nombre:'
		Leer nombre[n]
		Escribir 'Ingrese prioridad:'
		Leer prioridad[n]
		Escribir '¿Desea ingresar otro proceso? (1=Sí / 0=No):'
		Leer i
		Si i=1 Entonces
			n <- n+1
		FinSi
	Hasta Que i=0
	Escribir 'Procesos registrados:'
	Para i<-1 Hasta n Hacer
		Escribir 'ID:', id[i], ' - Nombre:', nombre[i], ' - Prioridad:', prioridad[i]
	FinPara
FinAlgoritmo
