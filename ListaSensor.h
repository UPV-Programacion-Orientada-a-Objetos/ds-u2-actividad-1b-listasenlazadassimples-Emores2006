/**
 * @file ListaSensor.h
 * @brief Implementación de Lista Enlazada Simple Genérica para sensores
 * @author Eliezer Mores Oyervides
 * @date 2025
 */

#ifndef LISTASENSOR_H
#define LISTASENSOR_H

#include <iostream>

/**
 * @class ListaSensor
 * @brief Lista enlazada simple genérica para almacenar lecturas de sensores
 * @tparam T Tipo de dato a almacenar (int, float, double, etc.)
 * 
 * Implementa una estructura de datos dinámica con gestión manual de memoria
 * para almacenar lecturas de sensores de forma flexible.
 */
template <typename T>
class ListaSensor {
private:
    /**
     * @struct Nodo
     * @brief Estructura que representa un nodo de la lista enlazada
     */
    struct Nodo {
        T dato;           ///< Valor almacenado en el nodo
        Nodo* siguiente;  ///< Puntero al siguiente nodo
        
        /**
         * @brief Constructor del nodo
         * @param valor Valor a almacenar
         */
        Nodo(T valor) : dato(valor), siguiente(nullptr) {}
    };
    
    Nodo* cabeza; ///< Puntero al primer nodo de la lista
    int tamanio;  ///< Número de elementos en la lista
    
public:
    /**
     * @brief Constructor por defecto
     */
    ListaSensor() : cabeza(nullptr), tamanio(0) {}
    
    /**
     * @brief Destructor - Libera toda la memoria de los nodos
     */
    ~ListaSensor() {
        std::cout << "  Liberando lista interna..." << std::endl;
        limpiar();
    }
    
    /**
     * @brief Constructor de copia (Regla de los Tres)
     * @param otra Lista a copiar
     */
    ListaSensor(const ListaSensor& otra) : cabeza(nullptr), tamanio(0) {
        copiar(otra);
    }
    
    /**
     * @brief Operador de asignación (Regla de los Tres)
     * @param otra Lista a asignar
     * @return Referencia a esta lista
     */
    ListaSensor& operator=(const ListaSensor& otra) {
        if (this != &otra) {
            limpiar();
            copiar(otra);
        }
        return *this;
    }
    
    /**
     * @brief Inserta un elemento al final de la lista
     * @param valor Valor a insertar
     */
    void insertar(T valor) {
        Nodo* nuevo = new Nodo(valor);
        
        if (cabeza == nullptr) {
            cabeza = nuevo;
        } else {
            Nodo* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevo;
        }
        tamanio++;
        std::cout << "Insertando nuevo nodo con valor: " << valor << std::endl;
    }
    
    /**
     * @brief Calcula el promedio de los elementos
     * @return Promedio de tipo T
     */
    T calcularPromedio() const {
        if (tamanio == 0) return T(0);
        
        T suma = T(0);
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            suma += actual->dato;
            actual = actual->siguiente;
        }
        return suma / tamanio;
    }
    
    /**
     * @brief Encuentra y elimina el valor mínimo de la lista
     * @return Valor mínimo encontrado
     */
    T eliminarMinimo() {
        if (cabeza == nullptr) return T(0);
        
        // Buscar el mínimo
        Nodo* actual = cabeza;
        Nodo* nodoMin = cabeza;
        Nodo* prevMin = nullptr;
        Nodo* prev = nullptr;
        
        while (actual != nullptr) {
            if (actual->dato < nodoMin->dato) {
                nodoMin = actual;
                prevMin = prev;
            }
            prev = actual;
            actual = actual->siguiente;
        }
        
        T valorMin = nodoMin->dato;
        
        // Eliminar el nodo mínimo
        if (prevMin == nullptr) {
            cabeza = cabeza->siguiente;
        } else {
            prevMin->siguiente = nodoMin->siguiente;
        }
        
        std::cout << "    Nodo " << valorMin << " eliminado (mínimo)." << std::endl;
        delete nodoMin;
        tamanio--;
        
        return valorMin;
    }
    
    /**
     * @brief Obtiene el tamaño actual de la lista
     * @return Número de elementos
     */
    int getTamanio() const {
        return tamanio;
    }
    
    /**
     * @brief Verifica si la lista está vacía
     * @return true si está vacía, false en caso contrario
     */
    bool estaVacia() const {
        return cabeza == nullptr;
    }
    
    /**
     * @brief Imprime todos los elementos de la lista
     */
    void imprimir() const {
        Nodo* actual = cabeza;
        std::cout << "    Lecturas: ";
        while (actual != nullptr) {
            std::cout << actual->dato << " ";
            actual = actual->siguiente;
        }
        std::cout << std::endl;
    }
    
private:
    /**
     * @brief Libera toda la memoria de los nodos
     */
    void limpiar() {
        while (cabeza != nullptr) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            std::cout << "    Nodo " << temp->dato << " liberado." << std::endl;
            delete temp;
            tamanio--;
        }
    }
    
    /**
     * @brief Copia los elementos de otra lista
     * @param otra Lista a copiar
     */
    void copiar(const ListaSensor& otra) {
        Nodo* actual = otra.cabeza;
        while (actual != nullptr) {
            insertar(actual->dato);
            actual = actual->siguiente;
        }
    }
};

#endif // LISTASENSOR_H