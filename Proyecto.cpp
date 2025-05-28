# SistemaGestionProcesos
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
        nuevo->id = leerEnteroNoNegativo("\nIngrese ID del proceso (número entero positivo): ");
        
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
        cout << "\nNo hay procesos registrados.\n";
    } else {
        cout << "\nLista de procesos:\n";
        while (aux != NULL) {
            cout << "ID: " << aux->id << " | Nombre: " << aux->nombre << " | Prioridad: " << aux->prioridad << endl;
            aux = aux->siguiente;
        }
    }
}

void eliminarProceso() {
    int id = leerEnteroNoNegativo("\nIngrese el ID del proceso a eliminar: ");
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
    int id = leerEnteroNoNegativo("\nIngrese el ID del proceso a buscar: ");
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
// ======== MENÚ PRINCIPAL Y SUBMENÚS ========

// Prototipos para submenús
void menuListaProcesos();
void menuColaPrioridad();
void menuPilaMemoria();

void mostrarMenuPrincipal() {
    cout << "\n===== MENÚ PRINCIPAL =====\n";
    cout << "1. Gestión de lista de procesos\n";
    cout << "2. Gestión de cola de prioridad (CPU)\n";
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
void encolarProceso() {
    int id = leerEnteroNoNegativo("\nIngrese ID del proceso a encolar: ");
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
        cout << "\nLa cola está vacía.\n";
    } else {
        cout << "\nCola de procesos por prioridad:\n";
        while (aux != NULL) {
            cout << "ID: " << aux->id << " | Nombre: " << aux->nombre << " | Prioridad: " << aux->prioridad << endl;
            aux = aux->siguiente;
        }
    }
}
