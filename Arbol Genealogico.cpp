#include <iostream>
#include <string>

using namespace std;

// =============================================================================
// ESTRUCTURA: Persona
// DESCRIPCIÓN: Representa un nodo en el árbol genealógico con información personal
//              y enlaces a familiares (padres e hijos).
// =============================================================================
struct Persona {
    int id;             // Identificador único
    string nombre;      // Nombre completo
    string fecha_nac;   // Fecha de nacimiento (formato dd/mm/aaaa)
    Persona* padre;     // Puntero al padre (NULL si no existe)
    Persona* madre;     // Puntero a la madre (NULL si no existe)
    Persona* izq;       // Hijo izquierdo (para estructura de árbol binario)
    Persona* der;       // Hijo derecho (para estructura de árbol binario)
};

// =============================================================================
// FUNCIÓN: crearPersona
// OBJETIVO: Crea y devuelve un nuevo nodo Persona con los datos proporcionados.
// PARÁMETROS:
//    - id: Identificador único
//    - nombre: Nombre completo
//    - fecha: Fecha de nacimiento
// RETORNO: Puntero al nuevo nodo Persona creado.
// =============================================================================
Persona* crearPersona(int id, string nombre, string fecha) {
    Persona* nueva = new Persona;
    nueva->id = id;
    nueva->nombre = nombre;
    nueva->fecha_nac = fecha;
    nueva->padre = NULL;
    nueva->madre = NULL;
    nueva->izq = NULL;
    nueva->der = NULL;
    return nueva;
}

// =============================================================================
// FUNCIÓN: insertar
// OBJETIVO: Inserta un nuevo nodo en el árbol binario de búsqueda (BST) según el ID.
//           También establece los punteros al padre y madre si se proporcionan.
// PARÁMETROS:
//    - raiz: Referencia al puntero raíz del árbol/subárbol
//    - id: Identificador único del nuevo nodo
//    - nombre: Nombre completo
//    - fecha: Fecha de nacimiento
//    - padre: Puntero al nodo padre (opcional)
//    - madre: Puntero al nodo madre (opcional)
// =============================================================================
void insertar(Persona* &raiz, int id, string nombre, string fecha, Persona* padre, Persona* madre) {
    if (raiz == NULL) {
        raiz = crearPersona(id, nombre, fecha);
        raiz->padre = padre;
        raiz->madre = madre;
        return;
    }
    
    if (id < raiz->id) {
        insertar(raiz->izq, id, nombre, fecha, padre, madre);
    } else if (id > raiz->id) {
        insertar(raiz->der, id, nombre, fecha, padre, madre);
    }
}

// =============================================================================
// FUNCIÓN: buscar
// OBJETIVO: Busca un nodo en el árbol por su ID (recursivo).
// PARÁMETROS:
//    - raiz: Puntero al nodo raíz del árbol/subárbol
//    - id: Identificador a buscar
// RETORNO: Puntero al nodo encontrado o NULL si no existe.
// =============================================================================
Persona* buscar(Persona* raiz, int id) {
    if (raiz == NULL || raiz->id == id) {
        return raiz;
    }
    
    if (id < raiz->id) {
        return buscar(raiz->izq, id);
    } else {
        return buscar(raiz->der, id);
    }
}

// =============================================================================
// FUNCIÓN: encontrarMinimo
// OBJETIVO: Encuentra el nodo con el ID mínimo en un subárbol (el más a la izquierda).
// PARÁMETROS:
//    - raiz: Puntero al nodo raíz del subárbol
// RETORNO: Puntero al nodo con el valor mínimo.
// =============================================================================
Persona* encontrarMinimo(Persona* raiz) {
    while (raiz->izq != NULL) {
        raiz = raiz->izq;
    }
    return raiz;
}

// =============================================================================
// FUNCIÓN: eliminar
// OBJETIVO: Elimina un nodo del árbol por su ID (recursivo) y reestructura el árbol.
// PARÁMETROS:
//    - raiz: Puntero al nodo raíz del árbol/subárbol
//    - id: Identificador del nodo a eliminar
// RETORNO: Puntero a la nueva raíz del subárbol modificado.
// =============================================================================
Persona* eliminar(Persona* raiz, int id) {
    if (raiz == NULL) return raiz;

    if (id < raiz->id) {
        raiz->izq = eliminar(raiz->izq, id);
    } else if (id > raiz->id) {
        raiz->der = eliminar(raiz->der, id);
    } else {
        // Caso 1: Nodo sin hijos o con un solo hijo
        if (raiz->izq == NULL) {
            Persona* temp = raiz->der;
            delete raiz;
            return temp;
        } else if (raiz->der == NULL) {
            Persona* temp = raiz->izq;
            delete raiz;
            return temp;
        }
        
        // Caso 2: Nodo con dos hijos
        Persona* temp = encontrarMinimo(raiz->der);
        raiz->id = temp->id;
        raiz->nombre = temp->nombre;
        raiz->fecha_nac = temp->fecha_nac;
        raiz->der = eliminar(raiz->der, temp->id);
    }
    return raiz;
}

// =============================================================================
// FUNCIÓN: inorden
// OBJETIVO: Recorre el árbol en orden (izquierda-raíz-derecha) e imprime los nodos.
// PARÁMETROS:
//    - raiz: Puntero al nodo raíz del árbol/subárbol
// =============================================================================
void inorden(Persona* raiz) {
    if (raiz != NULL) {
        inorden(raiz->izq);
        cout << "ID: " << raiz->id << ", Nombre: " << raiz->nombre 
             << ", Nacimiento: " << raiz->fecha_nac << endl;
        inorden(raiz->der);
    }
}

// =============================================================================
// FUNCIÓN: mostrarAncestros
// OBJETIVO: Muestra los ancestros de un nodo de forma jerárquica (recursivo).
// PARÁMETROS:
//    - persona: Puntero al nodo inicial
//    - nivel: Nivel de indentación para la visualización (opcional, default=0)
// =============================================================================
void mostrarAncestros(Persona* persona, int nivel = 0) {
    if (persona == NULL) return;
    
    for (int i = 0; i < nivel; i++) cout << "  ";
    cout << persona->nombre << endl;
    
    if (persona->padre != NULL) {
        for (int i = 0; i < nivel; i++) cout << "  ";
        cout << "Padre: ";
        mostrarAncestros(persona->padre, nivel + 1);
    }
    
    if (persona->madre != NULL) {
        for (int i = 0; i < nivel; i++) cout << "  ";
        cout << "Madre: ";
        mostrarAncestros(persona->madre, nivel + 1);
    }
}

// =============================================================================
// CONSTANTE: MAX_NODOS
// DESCRIPCIÓN: Tamaño máximo para estructuras auxiliares (ej. listas/colas).
// =============================================================================
const int MAX_NODOS = 100;

// =============================================================================
// ESTRUCTURA: ListaPersonas
// DESCRIPCIÓN: Almacena un arreglo de punteros a nodos Persona y su cantidad.
// =============================================================================
struct ListaPersonas {
    Persona* elementos[MAX_NODOS];  // Arreglo de punteros a Persona
    int cantidad;                   // Número actual de elementos
};

// =============================================================================
// FUNCIÓN: inicializarLista
// OBJETIVO: Inicializa una lista vacía.
// PARÁMETROS:
//    - lista: Referencia a la lista a inicializar
// =============================================================================
void inicializarLista(ListaPersonas &lista) {
    lista.cantidad = 0;
}

// =============================================================================
// FUNCIÓN: agregarALista
// OBJETIVO: Añade un puntero a Persona a la lista si hay espacio.
// PARÁMETROS:
//    - lista: Referencia a la lista
//    - p: Puntero al nodo Persona a agregar
// =============================================================================
void agregarALista(ListaPersonas &lista, Persona* p) {
    if (lista.cantidad < MAX_NODOS) {
        lista.elementos[lista.cantidad++] = p;
    }
}

// =============================================================================
// FUNCIÓN: almacenarNodos
// OBJETIVO: Almacena todos los nodos del árbol en una lista (recorrido inorden).
// PARÁMETROS:
//    - raiz: Puntero al nodo raíz del árbol
//    - lista: Referencia a la lista donde se almacenarán los nodos
// =============================================================================
void almacenarNodos(Persona* raiz, ListaPersonas &lista) {
    if (raiz == NULL) return;
    almacenarNodos(raiz->izq, lista);
    agregarALista(lista, raiz);
    almacenarNodos(raiz->der, lista);
}

// =============================================================================
// FUNCIÓN: construirArbolBalanceado
// OBJETIVO: Construye un árbol balanceado a partir de una lista ordenada.
// PARÁMETROS:
//    - lista: Lista de nodos ordenados
//    - inicio: Índice de inicio del subarreglo
//    - fin: Índice de fin del subarreglo
// RETORNO: Puntero a la raíz del nuevo subárbol balanceado.
// =============================================================================
Persona* construirArbolBalanceado(ListaPersonas &lista, int inicio, int fin) {
    if (inicio > fin) return NULL;
    
    int medio = (inicio + fin) / 2;
    Persona* raiz = lista.elementos[medio];
    
    raiz->izq = construirArbolBalanceado(lista, inicio, medio - 1);
    raiz->der = construirArbolBalanceado(lista, medio + 1, fin);
    
    return raiz;
}

// =============================================================================
// FUNCIÓN: balancearArbol
// OBJETIVO: Balancea el árbol para optimizar futuras operaciones.
// PARÁMETROS:
//    - raiz: Referencia al puntero raíz del árbol
// =============================================================================
void balancearArbol(Persona* &raiz) {
    ListaPersonas lista;
    inicializarLista(lista);
    almacenarNodos(raiz, lista);
    raiz = construirArbolBalanceado(lista, 0, lista.cantidad - 1);
}

// =============================================================================
// ESTRUCTURA: Cola
// DESCRIPCIÓN: Implementa una cola FIFO para el recorrido por niveles (BFS).
// =============================================================================
struct Cola {
    Persona* elementos[MAX_NODOS];  // Arreglo de punteros a Persona
    int frente;                     // Índice del frente de la cola
    int final;                      // Índice del final de la cola
};

// =============================================================================
// FUNCIÓN: inicializarCola
// OBJETIVO: Inicializa una cola vacía.
// PARÁMETROS:
//    - c: Referencia a la cola a inicializar
// =============================================================================
void inicializarCola(Cola &c) {
    c.frente = 0;
    c.final = -1;
}

// =============================================================================
// FUNCIÓN: encolar
// OBJETIVO: Añade un elemento al final de la cola si hay espacio.
// PARÁMETROS:
//    - c: Referencia a la cola
//    - p: Puntero al nodo Persona a encolar
// =============================================================================
void encolar(Cola &c, Persona* p) {
    if (c.final < MAX_NODOS - 1) {
        c.elementos[++c.final] = p;
    }
}

// =============================================================================
// FUNCIÓN: frente
// OBJETIVO: Devuelve el elemento al frente de la cola sin desencolarlo.
// PARÁMETROS:
//    - c: Referencia a la cola
// RETORNO: Puntero al nodo Persona en el frente o NULL si está vacía.
// =============================================================================
Persona* frente(Cola &c) {
    if (c.frente > c.final) return NULL;
    return c.elementos[c.frente];
}

// =============================================================================
// FUNCIÓN: desencolar
// OBJETIVO: Elimina el elemento al frente de la cola.
// PARÁMETROS:
//    - c: Referencia a la cola
// =============================================================================
void desencolar(Cola &c) {
    if (c.frente <= c.final) {
        c.frente++;
    }
}

// =============================================================================
// FUNCIÓN: colaVacia
// OBJETIVO: Verifica si la cola está vacía.
// PARÁMETROS:
//    - c: Referencia a la cola
// RETORNO: true si está vacía, false en caso contrario.
// =============================================================================
bool colaVacia(Cola &c) {
    return c.frente > c.final;
}

// =============================================================================
// FUNCIÓN: porNiveles
// OBJETIVO: Recorre el árbol por niveles (BFS) e imprime los nodos.
// PARÁMETROS:
//    - raiz: Puntero al nodo raíz del árbol
// =============================================================================
void porNiveles(Persona* raiz) {
    if (raiz == NULL) return;
    
    Cola c;
    inicializarCola(c);
    encolar(c, raiz);
    
    while (!colaVacia(c)) {
        Persona* actual = frente(c);
        desencolar(c);
        
        cout << "ID: " << actual->id << ", Nombre: " << actual->nombre << endl;
        
        if (actual->izq != NULL) encolar(c, actual->izq);
        if (actual->der != NULL) encolar(c, actual->der);
    }
}

// =============================================================================
// FUNCIÓN: mostrarDescendientes
// OBJETIVO: Muestra los descendientes directos de un nodo (recursivo).
// PARÁMETROS:
//    - persona: Puntero al nodo inicial
// =============================================================================
void mostrarDescendientes(Persona* persona) {
    if (persona == NULL) return;
    
    cout << "Descendientes de " << persona->nombre << ":" << endl;
    if (persona->izq != NULL) {
        cout << "- " << persona->izq->nombre << endl;
        mostrarDescendientes(persona->izq);
    }
    if (persona->der != NULL) {
        cout << "- " << persona->der->nombre << endl;
        mostrarDescendientes(persona->der);
    }
}

// =============================================================================
// FUNCIÓN: menu
// OBJETIVO: Muestra un menú interactivo para gestionar el árbol genealógico.
// =============================================================================
void menu() {
    Persona* arbol = NULL;
    int opcion, id, id_padre, id_madre;
    string nombre, fecha;
    
    do {
        cout << "\n=== MENU ARBOL GENEALOGICO ===" << endl;
        cout << "1. Agregar persona" << endl;
        cout << "2. Eliminar persona" << endl;
        cout << "3. Buscar persona" << endl;
        cout << "4. Mostrar arbol (inorden)" << endl;
        cout << "5. Mostrar por niveles" << endl;
        cout << "6. Mostrar ancestros" << endl;
        cout << "7. Mostrar descendientes" << endl;
        cout << "8. Balancear arbol" << endl;
        cout << "9. Salir" << endl;
        cout << "Ingrese opcion: ";
        cin >> opcion;
        
        switch(opcion) {
            case 1: {
                cout << "ID: "; cin >> id;
                cout << "Nombre: "; cin.ignore(); getline(cin, nombre);
                cout << "Fecha nacimiento (dd/mm/aaaa): "; cin >> fecha;
                cout << "ID del padre (0 si no existe): "; cin >> id_padre;
                cout << "ID de la madre (0 si no existe): "; cin >> id_madre;
                
                Persona* padre = (id_padre != 0) ? buscar(arbol, id_padre) : NULL;
                Persona* madre = (id_madre != 0) ? buscar(arbol, id_madre) : NULL;
                
                insertar(arbol, id, nombre, fecha, padre, madre);
                break;
            }
                
            case 2: {
                cout << "ID a eliminar: "; cin >> id;
                arbol = eliminar(arbol, id);
                break;
            }
                
            case 3: {
                cout << "ID a buscar: "; cin >> id;
                Persona* encontrado = buscar(arbol, id);
                if (encontrado != NULL) {
                    cout << "Encontrado: " << encontrado->nombre << endl;
                } else {
                    cout << "No encontrado" << endl;
                }
                break;
            }
                
            case 4: {
                cout << "\nArbol genealogico (inorden):" << endl;
                inorden(arbol);
                break;
            }
                
            case 5: {
                cout << "\nArbol por niveles:" << endl;
                porNiveles(arbol);
                break;
            }
                
            case 6: {
                cout << "ID para ver ancestros: "; cin >> id;
                mostrarAncestros(buscar(arbol, id));
                break;
            }
                
            case 7: {
                cout << "ID para ver descendientes: "; cin >> id;
                mostrarDescendientes(buscar(arbol, id));
                break;
            }
                
            case 8: {
                balancearArbol(arbol);
                cout << "Arbol balanceado" << endl;
                break;
            }
                
            case 9: {
                cout << "Saliendo..." << endl;
                break;
            }
                
            default:
                cout << "Opcion no valida" << endl;
        }
    } while (opcion != 9);
}

// =============================================================================
// FUNCIÓN: main
// OBJETIVO: Punto de entrada del programa. Ejecuta el menú principal.
// =============================================================================
int main() {
    menu();
    return 0;
}
