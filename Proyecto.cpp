#include <iostream>
#include <string>
#include <fstream>

using namespace std;



// ======== ESTRUCTURAS ========

// Nodo para lista enlazada (Gestor de procesos)
struct Nodo {
    int id;
    string nombre;
    int prioridad;
    Nodo *siguiente;
};

// Nodo para pila (Gestor de memoria)
struct NodoPila {
    string proceso;
    NodoPila *siguiente;
};

// ======== VARIABLES GLOBALES ========
Nodo *inicio = NULL;             // Lista enlazada de procesos
Nodo *colaPrioridad = NULL;      // Cola de prioridad (ordenada por prioridad)
NodoPila *tope = NULL;           // Pila para memoria

// ======== FUNCIONES DE VALIDACIÓN ========
// Lee un entero no negativo
int leerEnteroNoNegativo(string mensaje) {
    int valor;
    while (true) {
        cout << mensaje;
        cin >> valor;

        if (cin.fail() || valor < 0) {
            cout << "Error: ingrese un número entero no negativo.\n";
            cin.clear(); // Limpiar el estado de error
            cin.ignore(10000, '\n'); // Ignorar la línea de entrada
        } else {
            cin.ignore(10000, '\n'); // Limpiar el buffer
            return valor; // Retornar el valor válido
        }
    }
}
// Función para leer opción del menú con validación
int pedirOpcion(int minimo, int maximo) {
    int op;
    do {
        cout << "Elegir opción (" << minimo << " - " << maximo << "): ";
        cin >> op;
        if (cin.fail() || op < minimo || op > maximo) {
            cout << "Opción incorrecta... \n";
            cin.clear(); // Limpiar el estado de error
            cin.ignore(10000, '\n'); // Ignorar la línea de entrada
        }
    } while (op < minimo || op > maximo);
    cin.ignore(10000, '\n'); // Limpiar el buffer
    return op;
}

bool existeID(int id) {
    Nodo* aux = inicio;
    while (aux != NULL) {
        if (aux->id == id) {
            return true; // El ID ya existe
        }
        aux = aux->siguiente;
    }
    return false; // El ID no existe
}


// ======== GESTOR DE PROCESOS (Lista enlazada) ========
void insertarProceso() {
    Nodo *nuevo = new Nodo();
    
    // Validar ID único
    bool idValido = false;
    while (!idValido) {
        nuevo->id = leerEnteroNoNegativo("\n Ingrese ID del proceso (número entero positivo): ");
        
        if (existeID(nuevo->id)) {
            cout << "Error: El ID " << nuevo->id << " ya está en uso. Por favor ingrese un ID diferente.\n";
        } else {
            idValido = true;
        }
    }
    
    cout << "Ingrese nombre del proceso: ";
    getline(cin, nuevo->nombre);
    nuevo->prioridad = leerEnteroNoNegativo("Ingrese prioridad del proceso (mayor número = mayor prioridad, mínimo 0): ");
    nuevo->siguiente = NULL;

    if (inicio == NULL) {
        inicio = nuevo;
    } else {
        Nodo *aux = inicio;
        while (aux->siguiente != NULL) {
            aux = aux->siguiente;
        }
        aux->siguiente = nuevo;
    }
    cout << "Proceso registrado exitosamente.\n";
}

void mostrarProcesos() {
    Nodo *aux = inicio;
    if (aux == NULL) {
        cout << "\n No hay procesos registrados.\n";
    } else {
        cout << "\n Lista de procesos:\n";
        while (aux != NULL) {
            cout << "ID: " << aux->id << " | Nombre: " << aux->nombre << " | Prioridad: " << aux->prioridad << endl;
            aux = aux->siguiente;
        }
    }
}

void eliminarProceso() {
    int id = leerEnteroNoNegativo("\n Ingrese el ID del proceso a eliminar: ");
    if (inicio == NULL) {
        cout << "La lista está vacía.\n";
        return;
    }
    Nodo *actual = inicio;
    Nodo *anterior = NULL;

    while (actual != NULL && actual->id != id) {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual == NULL) {
        cout << "No se encontró un proceso con ID " << id << ".\n";
        return;
    }

    if (anterior == NULL) {
        // El proceso a eliminar es el primero
        inicio = inicio->siguiente;
    } else {
        anterior->siguiente = actual->siguiente;
    }

    delete actual;
    cout << "Proceso con ID " << id << " eliminado correctamente.\n";
}

void buscarProcesoPorID() {
    int id = leerEnteroNoNegativo("\n Ingrese el ID del proceso a buscar: ");
    Nodo* aux = inicio;

    while (aux != NULL) {
        if (aux->id == id) {
            cout << "Proceso encontrado:\n";
            cout << "ID: " << aux->id << endl;
            cout << "Nombre: " << aux->nombre << endl;
            cout << "Prioridad: " << aux->prioridad << endl;
            return;
        }
        aux = aux->siguiente;
    }

    cout << "No se encontró ningún proceso con ID " << id << ".\n";
}


Nodo* buscarProcesoPorID(int id) {
    Nodo* aux = inicio;
    while (aux != NULL) {
        if (aux->id == id) {
            return aux; // Retorna el nodo encontrado
        }
        aux = aux->siguiente;
    }
    return NULL; // No encontrado
}
// ======== ACTUALIZAR COLA DE PRIORIDAD ========
void actualizarColaPrioridad(int id, int nuevaPrioridad) {
    if (colaPrioridad == NULL) return;
    
    // Buscar el proceso en la cola
    Nodo* actual = colaPrioridad;
    Nodo* anterior = NULL;
    Nodo* nodoAMover = NULL;
    
    // Primero buscamos y separamos el nodo a actualizar
    while (actual != NULL) {
        if (actual->id == id) {
            nodoAMover = actual;
            if (anterior == NULL) {
                colaPrioridad = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }
            break;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    
    // Si encontramos el nodo, lo reinsertamos con la nueva prioridad
    if (nodoAMover != NULL) {
        nodoAMover->prioridad = nuevaPrioridad;
        nodoAMover->siguiente = NULL;
        
        // Reinsertar en la posición correcta según la nueva prioridad
        if (colaPrioridad == NULL || nuevaPrioridad > colaPrioridad->prioridad) {
            nodoAMover->siguiente = colaPrioridad;
            colaPrioridad = nodoAMover;
        } else {
            Nodo* temp = colaPrioridad;
            while (temp->siguiente != NULL && temp->siguiente->prioridad >= nuevaPrioridad) {
                temp = temp->siguiente;
            }
            nodoAMover->siguiente = temp->siguiente;
            temp->siguiente = nodoAMover;
        }
        
        cout << "Proceso actualizado en la cola de prioridad.\n";
    }
}

// ======== FUNCIÓN PARA MODIFICAR PRIORIDAD ========

void modificarPrioridad() {
    int id = leerEnteroNoNegativo("\nIngrese el ID del proceso a modificar: ");
    Nodo* proceso = buscarProcesoPorID(id);
    
    if (proceso == NULL) {
        cout << "No se encontró ningún proceso con ID " << id << ".\n";
        return;
    }
    
    int nuevaPrioridad = leerEnteroNoNegativo("Ingrese la nueva prioridad (mayor número = mayor prioridad, mínimo 0): ");
    
    // Mostrar información actual
    cout << "\nProceso encontrado:\n";
    cout << "ID: " << proceso->id << " | Nombre: " << proceso->nombre 
         << " | Prioridad actual: " << proceso->prioridad << endl;
    
    // Actualizar la prioridad en la lista principal
    proceso->prioridad = nuevaPrioridad;
    cout << "Prioridad actualizada a: " << nuevaPrioridad << endl;
    
    // Actualizar la cola de prioridad si el proceso está en ella
    actualizarColaPrioridad(id, nuevaPrioridad);
}

void encolarProceso() {
    int id = leerEnteroNoNegativo("\n Ingrese ID del proceso a encolar: ");
    Nodo* proceso = buscarProcesoPorID(id);
    if (proceso == NULL) {
        cout << "Proceso con ID " << id << " no encontrado en la lista de procesos.\n";
        return;
    }

    // Crear un nuevo nodo para la cola con los datos del proceso encontrado
    Nodo* nuevo = new Nodo();
    nuevo->id = proceso->id;
    nuevo->nombre = proceso->nombre;
    nuevo->prioridad = proceso->prioridad;
    nuevo->siguiente = NULL;

    // Insertar ordenado por prioridad (mayor prioridad primero)
    if (colaPrioridad == NULL || nuevo->prioridad > colaPrioridad->prioridad) {
        nuevo->siguiente = colaPrioridad;
        colaPrioridad = nuevo;
    } else {
        Nodo* aux = colaPrioridad;
        while (aux->siguiente != NULL && aux->siguiente->prioridad >= nuevo->prioridad) {
            aux = aux->siguiente;
        }
        nuevo->siguiente = aux->siguiente;
        aux->siguiente = nuevo;
    }
    cout << "Proceso '" << nuevo->nombre << "' encolado según prioridad.\n";
}
void desencolarProceso() {
    if (colaPrioridad == NULL) {
        cout << "\nLa cola está vacía.\n";
    } else {
        Nodo *aux = colaPrioridad;
        colaPrioridad = colaPrioridad->siguiente;
        cout << "\nProceso ejecutado: " << aux->nombre << " (ID: " << aux->id << ")\n";
        delete aux;
    }
}

void mostrarCola() {
    Nodo *aux = colaPrioridad;
    if (aux == NULL) {
        cout << "\n La cola está vacía.\n";
    } else {
        cout << "\nCola de procesos por prioridad:\n";
        while (aux != NULL) {
            cout << "ID: " << aux->id << " | Nombre: " << aux->nombre << " | Prioridad: " << aux->prioridad << endl;
            aux = aux->siguiente;
        }
    }
}

// ======== GESTOR DE MEMORIA (Pila) ========
void pushMemoria() {
    NodoPila *nuevo = new NodoPila();
    cout << "\nIngrese nombre del proceso que usará memoria: ";
    getline(cin, nuevo->proceso);
    nuevo->siguiente = tope;
    tope = nuevo;
    cout << "Memoria asignada al proceso.\n";
}

void popMemoria() {
    if (tope == NULL) {
        cout << "\nNo hay memoria asignada.\n";
    } else {
        NodoPila *aux = tope;
        tope = tope->siguiente;
        cout << "\nMemoria liberada del proceso: " << aux->proceso << endl;
        delete aux;
    }
}

void mostrarMemoria() {
    NodoPila *aux = tope;
    if (aux == NULL) {
        cout << "\nLa pila de memoria está vacía.\n";
    } else {
        cout << "\nEstado de la memoria (pila):\n";
        while (aux != NULL) {
            cout << "Proceso: " << aux->proceso << endl;
            aux = aux->siguiente;
        }
    }
}

// ======== PERSISTENCIA DE DATOS ========
void guardarProcesos() {
    ofstream archivo("procesos.txt");
    if (!archivo) {
        cout << "No se pudo abrir el archivo para guardar.\n";
        return;
    }
    Nodo *aux = inicio;
    while (aux != NULL) {
        archivo << aux->id << "," << aux->nombre << "," << aux->prioridad << "\n";
        aux = aux->siguiente;
    }
    archivo.close();
    cout << "Procesos guardados correctamente en procesos.txt\n";
}

void cargarProcesos() {
    ifstream archivo("procesos.txt");
    if (!archivo) {
        cout << "Archivo de procesos no encontrado. Se creará uno nuevo al guardar.\n";
        return;
    }
    string linea;
    while (getline(archivo, linea)) {
        // Parseo manual para no usar funciones avanzadas
        int pos1 = linea.find(',');
        int pos2 = linea.rfind(',');

        if (pos1 == string::npos || pos2 == string::npos || pos1 == pos2) {
            continue; // línea no válida, saltar
        }

        string idStr = linea.substr(0, pos1);
        string nombre = linea.substr(pos1 + 1, pos2 - pos1 - 1);
        string prioridadStr = linea.substr(pos2 + 1);

        // Convertir idStr a entero
        int id = 0;
        for (size_t i = 0; i < idStr.length(); i++) {
            char c = idStr[i];
            if (c >= '0' && c <= '9') {
                id = id * 10 + (c - '0');
            }
        }

        // Verificar si el ID ya existe antes de agregar
        if (existeID(id)) {
            cout << "Advertencia: Se omitió el proceso con ID duplicado: " << id << endl;
            continue;
        }

        // Convertir prioridadStr a entero
        int prioridad = 0;
        for (size_t i = 0; i < prioridadStr.length(); i++) {
            char c = prioridadStr[i];
            if (c >= '0' && c <= '9') {
                prioridad = prioridad * 10 + (c - '0');
            }
        }
        
        Nodo *nuevo = new Nodo();
        nuevo->id = id;
        nuevo->nombre = nombre;
        nuevo->prioridad = prioridad;
        nuevo->siguiente = NULL;

        if (inicio == NULL) {
            inicio = nuevo;
        } else {
            Nodo *aux = inicio;
            while (aux->siguiente != NULL) {
                aux = aux->siguiente;
            }
            aux->siguiente = nuevo;
        }
    }
    archivo.close();
    cout << "Procesos cargados desde procesos.txt\n";
}

// ======== MENÚ PRINCIPAL Y SUBMENÚS ========

// Prototipos para submenús
void menuListaProcesos();
void menuColaPrioridad();
void menuPilaMemoria();

void mostrarMenuPrincipal() {
    cout << "\n===== MENÚ PRINCIPAL =====\n";
    cout << "1. Gestión de lista de procesos\n";
    cout << "2. Gestión de cola de prioridad (CPU)\n";
    cout << "3. Gestión de memoria (pila)\n";
    cout << "0. Guardar y salir\n";
}

int main() {
	
    cargarProcesos();  // Cargar procesos guardados
	setlocale(LC_CTYPE, "Spanish");
    int op;
    do {
        mostrarMenuPrincipal();
        op = pedirOpcion(0, 3);

        switch (op) {
            case 1: 
                menuListaProcesos(); 
                break;
            case 2: 
                menuColaPrioridad(); 
                break;
            case 3: 
                menuPilaMemoria(); 
                break;
            case 0:
                guardarProcesos();
                cout << "Saliendo del programa...\n";
                break;
            default:
                cout << "Opción inválida.\n";
        }
    } while (op != 0);

    return 0;
}

// ======= SUBMENÚ LISTA DE PROCESOS =======
void menuListaProcesos() {
    int op;
    do {
        cout << "\n-- Gestión de Lista de Procesos --\n";
        cout << "1. Insertar proceso\n";
        cout << "2. Buscar proceso\n";
        cout << "3. Eliminar proceso\n";
        cout << "4. Modificar prioridad\n";
        cout << "5. Mostrar procesos\n";
        
        cout << "0. Volver al menú principal\n";
        op = pedirOpcion(0, 5);

        switch (op) {
            case 1: 
                insertarProceso(); 
                break;
            case 2: 
                buscarProcesoPorID();
                break;
            case 3: 
               	eliminarProceso(); 
                break;
            case 4: 
                modificarPrioridad(); 
                break;
            case 5: 
                mostrarProcesos(); 
                break;
            case 0: 
                cout << "Volviendo al menú principal...\n"; 
                break;
        }
    } while (op != 0);
}

// ======= SUBMENÚ COLA DE PRIORIDAD =======
void menuColaPrioridad() {
    int op;
    do {
        cout << "\n-- Gestión de Cola de Prioridad (CPU) --\n";
        cout << "1. Encolar proceso\n";
        cout << "2. Ejecutar proceso (desencolar)\n";
        cout << "3. Mostrar cola\n";
        cout << "0. Volver al menú principal\n";
        op = pedirOpcion(0, 3);

        switch (op) {
            case 1: 
                encolarProceso(); 
                break;
            case 2: 
                desencolarProceso(); 
                break;
            case 3: 
                mostrarCola(); 
                break;
            case 0: 
                cout << "Volviendo al menú principal...\n"; 
                break;
        }
    } while (op != 0);
}

// ======= SUBMENÚ PILA DE MEMORIA =======
void menuPilaMemoria() {
    int op;
    do {
        cout << "\n-- Gestión de Memoria (Pila) --\n";
        cout << "1. Asignar memoria (Push)\n";
        cout << "2. Liberar memoria (Pop)\n";
        cout << "3. Mostrar estado de la memoria\n";
        cout << "0. Volver al menú principal\n";
        op = pedirOpcion(0, 3);
        
        switch (op) {
            case 1: 
                pushMemoria(); 
                break;
            case 2: 
                popMemoria(); 
                break;
            case 3: 
                mostrarMemoria(); 
                break;
            case 0: 
                cout << "Volviendo al menú principal...\n"; 
                break;
        }
    } while (op != 0);
}
