#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;

// =============================================================================
// ESTRUCTURA: Persona
// =============================================================================
struct Persona {
    int id;
    string nombre;
    string fecha_nac;
    Persona* padre;
    Persona* madre;
    Persona* izq;
    Persona* der;
};


// =============================================================================
// VALIDACIÓN DE FECHA
// =============================================================================
bool esValida(string fecha) {
    if (fecha.length() != 10 || fecha[2] != '/' || fecha[5] != '/') {
        return false;
    }
    
    for (int i = 0; i < 10; i++) {
        if (i != 2 && i != 5 && !isdigit(fecha[i])) {
            return false;
        }
    }
    
    int dia = stoi(fecha.substr(0, 2));
    int mes = stoi(fecha.substr(3, 2));
    int anio = stoi(fecha.substr(6, 4));
    
    if (mes < 1 || mes > 12) return false;
    if (dia < 1 || dia > 31) return false;
    if (mes == 2 && dia > 29) return false;
    if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) return false;
    if (anio < 1900 || anio > 2025) return false;
    
    return true;
}

// =============================================================================
// GESTIÓN DE ID AUTOMÁTICO
// =============================================================================
int proximoID = 1;

void actualizarProximoID(Persona* raiz) {
    if (raiz == NULL) return;
    if (raiz->id >= proximoID) {
        proximoID = raiz->id + 1;
    }
    actualizarProximoID(raiz->izq);
    actualizarProximoID(raiz->der);
}

// =============================================================================
// FUNCIÓN: crearPersona
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
// =============================================================================
Persona* encontrarMinimo(Persona* raiz) {
    while (raiz->izq != NULL) {
        raiz = raiz->izq;
    }
    return raiz;
}

// =============================================================================
// FUNCIÓN: eliminar
// =============================================================================
Persona* eliminar(Persona* raiz, int id) {
    if (raiz == NULL) return raiz;

    if (id < raiz->id) {
        raiz->izq = eliminar(raiz->izq, id);
    } else if (id > raiz->id) {
        raiz->der = eliminar(raiz->der, id);
    } else {
        if (raiz->izq == NULL) {
            Persona* temp = raiz->der;
            delete raiz;
            return temp;
        } else if (raiz->der == NULL) {
            Persona* temp = raiz->izq;
            delete raiz;
            return temp;
        }
        
        Persona* temp = encontrarMinimo(raiz->der);
        raiz->id = temp->id;
        raiz->nombre = temp->nombre;
        raiz->fecha_nac = temp->fecha_nac;
        raiz->der = eliminar(raiz->der, temp->id);
    }
    return raiz;
}

// =============================================================================
// TABLA DE DATOS
// =============================================================================
struct TablaPersonas {
    Persona* personas[100];
    int cantidad;
};

TablaPersonas tablaGlobal;

void inicializarTabla() {
    tablaGlobal.cantidad = 0;
}

void agregarATabla(Persona* p) {
    if (tablaGlobal.cantidad < 100) {
        tablaGlobal.personas[tablaGlobal.cantidad++] = p;
    }
}

void llenarTabla(Persona* raiz) {
    if (raiz == NULL) return;
    llenarTabla(raiz->izq);
    agregarATabla(raiz);
    llenarTabla(raiz->der);
}

void mostrarTabla() {
    cout << "\n+----------------------------------------------------------------------------+\n";
    cout << "¦                          TABLA DE PERSONAS                                  ¦\n";
    cout << "¦----------------------------------------------------------------------------¦\n";
    cout << "¦ ID  ¦ NOMBRE               ¦ FECHA NACIMIENTO ¦ PADRE ¦ MADRE             ¦\n";
    cout << "¦----------------------------------------------------------------------------¦\n";
    
    inicializarTabla();
    llenarTabla(tablaGlobal.personas[0] != NULL ? tablaGlobal.personas[0] : NULL);
    
    for (int i = 0; i < tablaGlobal.cantidad; i++) {
        Persona* p = tablaGlobal.personas[i];
        string padre = (p->padre != NULL) ? p->padre->nombre : "N/A";
        string madre = (p->madre != NULL) ? p->madre->nombre : "N/A";
        
        cout << "¦ " << setw(3) << p->id << " ¦ " << setw(20) << left << p->nombre 
             << "¦ " << setw(16) << p->fecha_nac << "¦ " << setw(5) << (p->padre != NULL ? to_string(p->padre->id) : "N/A")
             << "¦ " << setw(5) << (p->madre != NULL ? to_string(p->madre->id) : "N/A") << "¦\n";
    }
    
    cout << "+----------------------------------------------------------------------------+\n";
}

// =============================================================================
// FUNCIÓN: mostrarAncestros
// =============================================================================
void mostrarAncestros(Persona* persona, int nivel = 0) {
    if (persona == NULL) return;
    
    for (int i = 0; i < nivel; i++) cout << "  ";
    cout << persona->nombre << endl;
    
    if (persona->padre != NULL) {
        for (int i = 0; i < nivel; i++) cout << "  ";
        cout << "+- Padre: ";
        mostrarAncestros(persona->padre, nivel + 1);
    }
    
    if (persona->madre != NULL) {
        for (int i = 0; i < nivel; i++) cout << "  ";
        cout << "+- Madre: ";
        mostrarAncestros(persona->madre, nivel + 1);
    }
}

// =============================================================================
// FUNCIÓN: mostrarDescendientes
// =============================================================================
void mostrarDescendientes(Persona* persona) {
    if (persona == NULL) return;
    
    cout << "Descendientes de " << persona->nombre << ":\n";
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
// RECORRIDOS DEL ÁRBOL
// =============================================================================

// PREORDEN: Raíz - Izquierda - Derecha
void preorden(Persona* raiz) {
    if (raiz == NULL) return;
    cout << "[" << raiz->id << "] " << raiz->nombre << " (" << raiz->fecha_nac << ")\n";
    preorden(raiz->izq);
    preorden(raiz->der);
}

// INORDEN: Izquierda - Raíz - Derecha
void inorden(Persona* raiz) {
    if (raiz == NULL) return;
    inorden(raiz->izq);
    cout << "[" << raiz->id << "] " << raiz->nombre << " (" << raiz->fecha_nac << ")\n";
    inorden(raiz->der);
}

// POSTORDEN: Izquierda - Derecha - Raíz
void postorden(Persona* raiz) {
    if (raiz == NULL) return;
    postorden(raiz->izq);
    postorden(raiz->der);
    cout << "[" << raiz->id << "] " << raiz->nombre << " (" << raiz->fecha_nac << ")\n";
}

// =============================================================================
// ESTRUCTURA: Cola para recorrido por niveles
// =============================================================================
struct Cola {
    Persona* elementos[100];
    int frente;
    int final;
};

void inicializarCola(Cola &c) {
    c.frente = 0;
    c.final = -1;
}

void encolar(Cola &c, Persona* p) {
    if (c.final < 99) {
        c.elementos[++c.final] = p;
    }
}

Persona* frente(Cola &c) {
    if (c.frente > c.final) return NULL;
    return c.elementos[c.frente];
}

void desencolar(Cola &c) {
    if (c.frente <= c.final) {
        c.frente++;
    }
}

bool colaVacia(Cola &c) {
    return c.frente > c.final;
}

// RECORRIDO POR NIVELES (BFS)
void porNiveles(Persona* raiz) {
    if (raiz == NULL) return;
    
    Cola c;
    inicializarCola(c);
    encolar(c, raiz);
    
    int nivelActual = 0;
    int nodosNivel = 1;
    int nodosContados = 0;
    
    while (!colaVacia(c)) {
        Persona* actual = frente(c);
        desencolar(c);
        
        cout << "[" << actual->id << "] " << actual->nombre << " (" << actual->fecha_nac << ")  ";
        nodosContados++;
        
        if (actual->izq != NULL) encolar(c, actual->izq);
        if (actual->der != NULL) encolar(c, actual->der);
        
        if (nodosContados == nodosNivel) {
            cout << "\n";
            nivelActual++;
            nodosNivel *= 2;
            nodosContados = 0;
        }
    }
    if (nodosContados > 0) cout << "\n";
}

// =============================================================================
// MENÚ PRINCIPAL
// =============================================================================
void menu() {
    Persona* arbol = NULL;
    int opcion, id_padre, id_madre;
    string nombre, fecha;
    
    while (true) {
        system("clear || cls");
        
        cout << "\n+---------------------------------------------------------------------------+\n";
        cout << "¦                        ARBOL GENEALOGICO                                   ¦\n";
        cout << "¦---------------------------------------------------------------------------¦\n";
        cout << "¦  1. Agregar persona                                                       ¦\n";
        cout << "¦  2. Eliminar persona                                                      ¦\n";
        cout << "¦  3. Buscar persona                                                        ¦\n";
        cout << "¦  4. Ver tabla de personas                                                 ¦\n";
        cout << "¦  5. Mostrar ancestros                                                     ¦\n";
        cout << "¦  6. Mostrar descendientes                                                 ¦\n";
        cout << "¦  7. Ver recorridos del arbol                                              ¦\n";
        cout << "¦  8. Salir                                                                 ¦\n";
        cout << "+---------------------------------------------------------------------------+\n";
        cout << "Ingrese opcion: ";
        cin >> opcion;
        cin.ignore();
        
        switch(opcion) {
            case 1: {
                system("clear || cls");
                cout << "\n---------------------------------------------------------------------------\n";
                cout << "                        AGREGAR NUEVA PERSONA \n";
                cout << "---------------------------------------------------------------------------\n\n";
                
                int nuevoID = proximoID++;
                
                cout << " ID asignado automaticamente : " << nuevoID << "\n\n";
                
                cout << "Ingrese nombre: ";
                getline(cin, nombre);
                
                bool fechaValida = false;
                while (!fechaValida) {
                    cout << "Ingrese fecha de nacimiento (dd/mm/aaaa): ";
                    cin >> fecha;
                    if (esValida(fecha)) {
                        fechaValida = true;
                    } else {
                        cout << " Fecha invalida. Intente de nuevo (formato: dd/mm/aaaa, 1900-2025)\n";
                    }
                }
                
                cin.ignore();
                
                cout << "\n ¿Desea asignar padres? (s/n): ";
                char respuesta;
                cin >> respuesta;
                cin.ignore();
                
                Persona* padre = NULL;
                Persona* madre = NULL;
                
                if (respuesta == 's' || respuesta == 'S') {
                    cout << "ID del padre (0 si no existe): ";
                    cin >> id_padre;
                    padre = (id_padre != 0) ? buscar(arbol, id_padre) : NULL;
                    
                    cout << "ID de la madre (0 si no existe): ";
                    cin >> id_madre;
                    madre = (id_madre != 0) ? buscar(arbol, id_madre) : NULL;
                    cin.ignore();
                }
                
                insertar(arbol, nuevoID, nombre, fecha, padre, madre);
                agregarATabla(buscar(arbol, nuevoID));
                
                cout << "\n  Persona agregada correctamente\n";
                cout << "\n Presione ENTER para continuar...";
                cin.get();
                break;
            }
                
            case 2: {
                system("clear || cls");
                cout << "\n---------------------------------------------------------------------------\n";
                cout << "                         ELIMINAR PERSONA \n";
                cout << "---------------------------------------------------------------------------\n\n";
                
                int id;
                cout << "ID a eliminar: ";
                cin >> id;
                
                Persona* encontrado = buscar(arbol, id);
                if (encontrado != NULL) {
                    arbol = eliminar(arbol, id);
                    cout << "\n Persona eliminada correctamente \n";
                } else {
                    cout << "\n Persona no encontrada\n";
                }
                
                cout << "\n Presione ENTER para continuar...";
                cin.ignore();
                cin.get();
                break;
            }
                
            case 3: {
                system("clear || cls");
                cout << "\n---------------------------------------------------------------------------\n";
                cout << "                         BUSCAR PERSONA \n";
                cout << "---------------------------------------------------------------------------\n\n";
                
                int id;
                cout << "ID a buscar: ";
                cin >> id;
                
                Persona* encontrado = buscar(arbol, id);
                if (encontrado != NULL) {
                    cout << "\n Encontrado:\n";
                    cout << "  ID: " << encontrado->id << "\n";
                    cout << "  Nombre: " << encontrado->nombre << "\n";
                    cout << "  Fecha: " << encontrado->fecha_nac << "\n";
                } else {
                    cout << "\n No encontrado\n";
                }
                
                cout << "\n Presione ENTER para continuar...";
                cin.ignore();
                cin.get();
                break;
            }
                
            case 4: {
                system("clear || cls");
                inicializarTabla();
                llenarTabla(arbol);
                
                cout << "\n+----------------------------------------------------------------------------+\n";
                cout << "¦                          TABLA DE PERSONAS                                  ¦\n";
                cout << "¦----------------------------------------------------------------------------¦\n";
                cout << "¦ ID  ¦ NOMBRE               ¦ FECHA NACIMIENTO ¦ PADRE  ¦ MADRE             ¦\n";
                cout << "¦----------------------------------------------------------------------------¦\n";
                
                for (int i = 0; i < tablaGlobal.cantidad; i++) {
                    Persona* p = tablaGlobal.personas[i];
                    cout << "¦ " << setw(3) << p->id << " ¦ " << setw(20) << left << p->nombre 
                         << "¦ " << setw(16) << p->fecha_nac << "¦ ";
                    
                    if (p->padre != NULL) {
                        cout << setw(6) << left << p->padre->id;
                    } else {
                        cout << setw(6) << left << "N/A";
                    }
                    
                    cout << "¦ ";
                    if (p->madre != NULL) {
                        cout << setw(17) << left << p->madre->nombre << "¦\n";
                    } else {
                        cout << setw(17) << left << "N/A" << "¦\n";
                    }
                }
                
                cout << "+----------------------------------------------------------------------------+\n";
                cout << "\n Presione ENTER para continuar...";
                cin.ignore();
                cin.get();
                break;
            }
                
            case 5: {
                system("clear || cls");
                cout << "\n---------------------------------------------------------------------------\n";
                cout << "                         VER ANCESTROS \n";
                cout << "---------------------------------------------------------------------------\n\n";
                
                int id;
                cout << "ID para ver ancestros: ";
                cin >> id;
                
                Persona* p = buscar(arbol, id);
                if (p != NULL) {
                    cout << "\n";
                    mostrarAncestros(p);
                } else {
                    cout << "\n Persona no encontrada \n";
                }
                
                cout << "\n Presione ENTER para continuar...";
                cin.ignore();
                cin.get();
                break;
            }
                
            case 6: {
                system("clear || cls");
                cout << "\n---------------------------------------------------------------------------\n";
                cout << "                      VER DESCENDIENTES \n";
                cout << "---------------------------------------------------------------------------\n\n";
                
                int id;
                cout << "ID para ver descendientes: ";
                cin >> id;
                
                Persona* p = buscar(arbol, id);
                if (p != NULL) {
                    cout << "\n";
                    mostrarDescendientes(p);
                } else {
                    cout << "\n Persona no encontrada\n";
                }
                
                cout << "\n Presione ENTER para continuar...";
                cin.ignore();
                cin.get();
                break;
            }
                
            case 7: {
                system("clear || cls");
                cout << "\n+---------------------------------------------------------------------------+\n";
                cout << "¦                      RECORRIDOS DEL ARBOL                                 ¦\n";
                cout << "¦---------------------------------------------------------------------------¦\n";
                cout << "¦  1. Preorden (Raiz - Izquierda - Derecha)                               ¦\n";
                cout << "¦  2. Inorden (Izquierda - Raiz - Derecha)                                ¦\n";
                cout << "¦  3. Postorden (Izquierda - Derecha - Raiz)                              ¦\n";
                cout << "¦  4. Por Niveles (BFS)                                                   ¦\n";
                cout << "¦  5. Volver al menu principal                                            ¦\n";
                cout << "+---------------------------------------------------------------------------+\n";
                cout << "Ingrese opcion: ";
                
                int opcionRecorrido;
                cin >> opcionRecorrido;
                cin.ignore();
                
                if (arbol == NULL) {
                    cout << "\n  El arbol esta vacio \n";
                    cout << "Presione ENTER para continuar...";
                    cin.get();
                    break;
                }
                
                system("clear || cls");
                
                switch(opcionRecorrido) {
                    case 1: {
                        cout << "\n---------------------------------------------------------------------------\n";
                        cout << "                    RECORRIDO PREORDEN \n";
                        cout << "                 (Raíz - Izquierda - Derecha)\n";
                        cout << "---------------------------------------------------------------------------\n\n";
                        preorden(arbol);
                        break;
                    }
                    case 2: {
                        cout << "\n---------------------------------------------------------------------------\n";
                        cout << "                    RECORRIDO INORDEN \n";
                        cout << "                 (Izquierda - Raíz - Derecha)\n";
                        cout << "---------------------------------------------------------------------------\n\n";
                        inorden(arbol);
                        break;
                    }
                    case 3: {
                        cout << "\n---------------------------------------------------------------------------\n";
                        cout << "                   RECORRIDO POSTORDEN \n";
                        cout << "                 (Izquierda - Derecha - Raíz)\n";
                        cout << "---------------------------------------------------------------------------\n\n";
                        postorden(arbol);
                        break;
                    }
                    case 4: {
                        cout << "\n---------------------------------------------------------------------------\n";
                        cout << "                   RECORRIDO POR NIVELES (BFS) \n";
                        cout << "---------------------------------------------------------------------------\n\n";
                        porNiveles(arbol);
                        break;
                    }
                    case 5: {
                        break;
                    }
                    default: {
                        cout << "\n Opción no válida\n";
                    }
                }
                
                cout << "\n Presione ENTER para continuar...";
                cin.get();
                break;
            }
                
            case 8: {return;  // salir del menú y terminar el programa
              }
            	
                
            default:{
            	cout << "\n Opción no valida\n";
                cout << "Presione ENTER para continuar...";
                cin.ignore();
                cin.get();
				break;
			}
               
        }
    }
}

// =============================================================================
// FUNCIÓN: main
// =============================================================================
int main() {
    menu();
    return 0;
}
