#include <iostream>
#include <string>
#include <fstream> // Librería para manejo de archivos (persistencia de datos)

using namespace std;

// ==============================
// ESTRUCTURAS DE DATOS
// ==============================

// Nodo de lista enlazada (gestión de procesos)
struct Nodo {
    int id;               // ID único del proceso
    string nombre;        // Nombre del proceso
    int prioridad;        // Prioridad (mayor número = más prioridad)
    Nodo *siguiente;      // Puntero al siguiente nodo
};

// Nodo de pila (para gestión de memoria)
struct NodoPila {
    string proceso;        // Nombre del proceso
    int tamanoMemoria;     // Tamaño de memoria en KB
    NodoPila *siguiente;   // Apunta al siguiente nodo de la pila
};

// ==============================
// VARIABLES GLOBALES
// ==============================
Nodo *inicio = NULL;             // Lista enlazada de procesos
Nodo *colaPrioridad = NULL;      // Cola de prioridad (ordenada por prioridad)
NodoPila *tope = NULL;           // Pila para memoria

// ==============================
// FUNCIONES DE VALIDACIÓN
// ==============================

// Solicita al usuario un número entero no negativo
int leerEnteroNoNegativo(string mensaje) {
    int valor;
    while (true) {
        cout << mensaje;
        cin >> valor;

        if (cin.fail() || valor < 0) {
            cout << "Error: ingrese un número entero no negativo.\n";
            cin.clear();               // Limpia estado de error
            cin.ignore(10000, '\n');   // Limpia el buffer
        } else {
            cin.ignore(10000, '\n');
            return valor;
        }
    }
}

// Solicita una opción dentro de un rango
int pedirOpcion(int minimo, int maximo) {
    int op;
    do {
        cout << "Elegir opción (" << minimo << " - " << maximo << "): ";
        cin >> op;
        if (cin.fail() || op < minimo || op > maximo) {
            cout << "Opción incorrecta... \n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    } while (op < minimo || op > maximo);
    cin.ignore(10000, '\n');
    return op;
}

// Verifica si el ID ya existe en la lista de procesos
bool existeID(int id) {
    Nodo* aux = inicio;
    while (aux != NULL) {
        if (aux->id == id) {
            return true; // El ID ya está en uso
        }
        aux = aux->siguiente;
    }
    return false; // ID válido
}
// ==============================
// GESTIÓN DE PROCESOS (LISTA ENLAZADA)
// ==============================

void insertarProceso() {
    Nodo *nuevo = new Nodo(); // Crear nuevo nodo
    bool idValido = false;

    // Validar que el ID sea único
    while (!idValido) {
        nuevo->id = leerEnteroNoNegativo("\nIngrese ID del proceso (entero positivo): ");
        if (existeID(nuevo->id)) {
            cout << "Error: El ID " << nuevo->id << " ya está en uso. Ingrese otro.\n";
        } else {
            idValido = true;
        }
    }

    cout << "Ingrese nombre del proceso: ";
    getline(cin, nuevo->nombre);

    nuevo->prioridad = leerEnteroNoNegativo("Ingrese prioridad del proceso (mayor número = mayor prioridad): ");
    nuevo->siguiente = NULL;

    // Insertar al final de la lista enlazada
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

    // Buscar el nodo con el ID dado
    while (actual != NULL && actual->id != id) {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual == NULL) {
        cout << "No se encontró un proceso con ID " << id << ".\n";
        return;
    }

    // Eliminar el nodo
    if (anterior == NULL) {
        inicio = inicio->siguiente; // Era el primero
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

// Sobrecarga para retornar el nodo encontrado (usada en otras funciones)
Nodo* buscarProcesoPorID(int id) {
    Nodo* aux = inicio;
    while (aux != NULL) {
        if (aux->id == id) {
            return aux;
        }
        aux = aux->siguiente;
    }
    return NULL; // No encontrado
}
// ==============================
// ACTUALIZAR COLA DE PRIORIDAD
// ==============================

void actualizarColaPrioridad(int id, int nuevaPrioridad) {
    if (colaPrioridad == NULL) return;

    Nodo* actual = colaPrioridad;
    Nodo* anterior = NULL;
    Nodo* nodoAMover = NULL;

    // Buscar y remover el nodo con el ID correspondiente
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

    // Si el nodo fue encontrado, se reubica con la nueva prioridad
    if (nodoAMover != NULL) {
        nodoAMover->prioridad = nuevaPrioridad;
        nodoAMover->siguiente = NULL;

        // Insertar nuevamente ordenado por prioridad
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

// ==============================
// MODIFICAR PRIORIDAD DE UN PROCESO
// ==============================

void modificarPrioridad() {
    int id = leerEnteroNoNegativo("\nIngrese el ID del proceso a modificar: ");
    Nodo* proceso = buscarProcesoPorID(id);

    if (proceso == NULL) {
        cout << "No se encontró ningún proceso con ID " << id << ".\n";
        return;
    }

    int nuevaPrioridad = leerEnteroNoNegativo("Ingrese la nueva prioridad: ");

    // Mostrar datos actuales del proceso
    cout << "\nProceso encontrado:\n";
    cout << "ID: " << proceso->id << " | Nombre: " << proceso->nombre 
         << " | Prioridad actual: " << proceso->prioridad << endl;

    // Actualizar prioridad tanto en lista como en cola
    proceso->prioridad = nuevaPrioridad;
    cout << "Prioridad actualizada a: " << nuevaPrioridad << endl;
    actualizarColaPrioridad(id, nuevaPrioridad);
}

// ==============================
// ENCOLAR PROCESO EN LA COLA DE PRIORIDAD
// ==============================

void encolarProceso() {
    int id = leerEnteroNoNegativo("\nIngrese ID del proceso a encolar: ");
    Nodo* proceso = buscarProcesoPorID(id);
    if (proceso == NULL) {
        cout << "Proceso con ID " << id << " no encontrado en la lista de procesos.\n";
        return;
    }

    // Crear un nodo nuevo con la información del proceso encontrado
    Nodo* nuevo = new Nodo();
    nuevo->id = proceso->id;
    nuevo->nombre = proceso->nombre;
    nuevo->prioridad = proceso->prioridad;
    nuevo->siguiente = NULL;

    // Insertar ordenado por prioridad
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
// ==============================
// DESENCOLAR PROCESO DE LA COLA DE PRIORIDAD
// ==============================

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

// ==============================
// MOSTRAR COLA DE PRIORIDAD ACTUAL
// ==============================

void mostrarCola() {
    Nodo *aux = colaPrioridad;
    if (aux == NULL) {
        cout << "\nLa cola está vacía.\n";
    } else {
        cout << "\nCola de procesos por prioridad:\n";
        while (aux != NULL) {
            cout << "ID: " << aux->id << " | Nombre: " << aux->nombre 
                 << " | Prioridad: " << aux->prioridad << endl;
            aux = aux->siguiente;
        }
    }
}

// ==============================
// GESTOR DE MEMORIA (PILA)
// ==============================

// Asigna memoria a un proceso (PUSH)
void pushMemoria() {
    int id = leerEnteroNoNegativo("\nIngrese ID del proceso que usará memoria: ");
    Nodo* proceso = buscarProcesoPorID(id);
    if (proceso == NULL) {
        cout << "No se encontró el proceso con ese ID.\n";
        return;
    }

    NodoPila *nuevo = new NodoPila();
    nuevo->proceso = proceso->nombre; // Asocia el nombre del proceso
    nuevo->tamanoMemoria = leerEnteroNoNegativo("Ingrese el tamaño de memoria a asignar (en KB): ");
    nuevo->siguiente = tope;
    tope = nuevo;

    cout << "Memoria asignada al proceso '" << proceso->nombre 
         << "' (" << nuevo->tamanoMemoria << " KB).\n";
}

// Libera el último bloque de memoria asignado (POP)
void popMemoria() {
    if (tope == NULL) {
        cout << "\nNo hay memoria asignada.\n";
    } else {
        NodoPila *aux = tope;
        tope = tope->siguiente;
        cout << "\nMemoria liberada del proceso: " << aux->proceso 
             << " (" << aux->tamanoMemoria << " KB)\n";
        delete aux;
    }
}

// Muestra el estado actual de la pila de memoria
void mostrarMemoria() {
    NodoPila *aux = tope;
    if (aux == NULL) {
        cout << "\nLa pila de memoria está vacía.\n";
    } else {
        cout << "\nEstado de la memoria (pila):\n";
        while (aux != NULL) {
            cout << "Proceso: " << aux->proceso 
                 << " | Memoria asignada: " << aux->tamanoMemoria << " KB\n";
            aux = aux->siguiente;
        }
    }
}
// ==============================
// PERSISTENCIA DE DATOS EN ARCHIVO
// ==============================

// Guarda la lista de procesos en un archivo .txt
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

// Carga procesos desde el archivo procesos.txt si existe
void cargarProcesos() {
    ifstream archivo("procesos.txt");
    if (!archivo) {
        cout << "Archivo de procesos no encontrado. Se creará uno nuevo al guardar.\n";
        return;
    }

    string linea;
    while (getline(archivo, linea)) {
        int pos1 = linea.find(',');
        int pos2 = linea.rfind(',');

        if (pos1 == string::npos || pos2 == string::npos || pos1 == pos2) {
            continue; // Línea inválida
        }

        string idStr = linea.substr(0, pos1);
        string nombre = linea.substr(pos1 + 1, pos2 - pos1 - 1);
        string prioridadStr = linea.substr(pos2 + 1);

        // Convertir ID de string a entero
        int id = 0;
        for (int i = 0; i < idStr.length(); i++) {
            char c = idStr[i];
            if (isdigit(c)) id = id * 10 + (c - '0');
        }

        // Verificar duplicado
        if (existeID(id)) {
            cout << "Advertencia: Se omitió el proceso con ID duplicado: " << id << endl;
            continue;
        }

        // Convertir prioridad de string a entero
        int prioridad = 0;
        for (int i = 0; i < prioridadStr.length(); i++) {
            char c = prioridadStr[i];
            if (isdigit(c)) prioridad = prioridad * 10 + (c - '0');
        }

        Nodo *nuevo = new Nodo();
        nuevo->id = id;
        nuevo->nombre = nombre;
        nuevo->prioridad = prioridad;
        nuevo->siguiente = NULL;

        // Agregar a la lista enlazada
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


// ==============================
// MENÚ PRINCIPAL Y SUBMENÚS
// ==============================

// Prototipos de submenús
void menuListaProcesos();
void menuColaPrioridad();
void menuPilaMemoria();

// Muestra el menú principal
void mostrarMenuPrincipal() {
    cout << "\n===== MENÚ PRINCIPAL =====\n";
    cout << "1. Gestión de lista de procesos\n";
    cout << "2. Gestión de cola de prioridad (CPU)\n";
    cout << "3. Gestión de memoria (pila)\n";
    cout << "0. Guardar y salir\n";
}
// ==============================
// FUNCIÓN PRINCIPAL DEL PROGRAMA
// ==============================

int main() {
    cargarProcesos();  // Carga procesos desde archivo al iniciar el sistema
    setlocale(LC_CTYPE, "Spanish"); // Para permitir caracteres especiales (acentos, etc.)

    int op;
    do {
        mostrarMenuPrincipal();
        op = pedirOpcion(0, 3); // Rango de opciones válidas

        switch (op) {
            case 1:
                menuListaProcesos(); // Submenú para lista enlazada
                break;
            case 2:
                menuColaPrioridad(); // Submenú para cola de prioridad
                break;
            case 3:
                menuPilaMemoria(); // Submenú para pila (memoria)
                break;
            case 0:
                guardarProcesos(); // Guardar antes de salir
                cout << "Saliendo del programa...\n";
                break;
            default:
                cout << "Opción inválida.\n";
        }
    } while (op != 0);

    return 0;
}

// ==============================
// SUBMENÚ PARA GESTIÓN DE PROCESOS
// ==============================

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
            case 1: insertarProceso(); break;
            case 2: buscarProcesoPorID(); break;
            case 3: eliminarProceso(); break;
            case 4: modificarPrioridad(); break;
            case 5: mostrarProcesos(); break;
            case 0: cout << "Volviendo al menú principal...\n"; break;
        }
    } while (op != 0);
}

// ==============================
// SUBMENÚ PARA COLA DE PRIORIDAD
// ==============================

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
            case 1: encolarProceso(); break;
            case 2: desencolarProceso(); break;
            case 3: mostrarCola(); break;
            case 0: cout << "Volviendo al menú principal...\n"; break;
        }
    } while (op != 0);
}

// ==============================
// SUBMENÚ PARA PILA DE MEMORIA
// ==============================

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
            case 1: pushMemoria(); break;
            case 2: popMemoria(); break;
            case 3: mostrarMemoria(); break;
            case 0: cout << "Volviendo al menú principal...\n"; break;
        }
    } while (op != 0);
}

