/**
 * @file ListaGestion.h
 * @brief Lista enlazada NO genérica para gestión polimórfica de sensores
 * @author Eliezer Mores Oyervides
 * @date 2025
 */

#ifndef LISTAGESTION_H
#define LISTAGESTION_H

#include "SensorBase.h"
#include <iostream>

/**
 * @class ListaGestion
 * @brief Lista enlazada simple para almacenar punteros a SensorBase
 * 
 * Permite gestionar de forma polimórfica diferentes tipos de sensores
 * (SensorTemperatura, SensorPresion) a través de punteros a la clase base.
 */
class ListaGestion {
private:
    /**
     * @struct NodoSensor
     * @brief Nodo que almacena un puntero a SensorBase
     */
    struct NodoSensor {
        SensorBase* sensor;      ///< Puntero polimórfico al sensor
        NodoSensor* siguiente;   ///< Puntero al siguiente nodo
        
        /**
         * @brief Constructor del nodo
         * @param s Puntero al sensor
         */
        NodoSensor(SensorBase* s) : sensor(s), siguiente(nullptr) {}
    };
    
    NodoSensor* cabeza; ///< Primer nodo de la lista
    int tamanio;        ///< Número de sensores en la lista
    
public:
    /**
     * @brief Constructor por defecto
     */
    ListaGestion() : cabeza(nullptr), tamanio(0) {}
    
    /**
     * @brief Destructor - Libera todos los sensores y nodos
     */
    ~ListaGestion() {
        std::cout << "\n--- Liberación de Memoria en Cascada ---" << std::endl;
        while (cabeza != nullptr) {
            NodoSensor* temp = cabeza;
            cabeza = cabeza->siguiente;
            
            std::cout << "Liberando Nodo: " << temp->sensor->getNombre() << "." << std::endl;
            delete temp->sensor; // Llama al destructor virtual
            delete temp;
        }
        std::cout << "Sistema cerrado. Memoria limpia." << std::endl;
    }
    
    /**
     * @brief Inserta un sensor al final de la lista
     * @param sensor Puntero al sensor a insertar
     */
    void insertar(SensorBase* sensor) {
        NodoSensor* nuevo = new NodoSensor(sensor);
        
        if (cabeza == nullptr) {
            cabeza = nuevo;
        } else {
            NodoSensor* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevo;
        }
        tamanio++;
        std::cout << "Sensor '" << sensor->getNombre() << "' insertado en la lista de gestión." << std::endl;
    }
    
    /**
     * @brief Busca un sensor por su nombre
     * @param nombre Nombre del sensor a buscar
     * @return Puntero al sensor o nullptr si no existe
     */
    SensorBase* buscar(const char* nombre) {
        NodoSensor* actual = cabeza;
        while (actual != nullptr) {
            if (strcmp(actual->sensor->getNombre(), nombre) == 0) {
                return actual->sensor;
            }
            actual = actual->siguiente;
        }
        return nullptr;
    }
    
    /**
     * @brief Procesa todos los sensores de forma polimórfica
     */
    void procesarTodos() {
        std::cout << "\n--- Ejecutando Polimorfismo ---" << std::endl;
        NodoSensor* actual = cabeza;
        while (actual != nullptr) {
            actual->sensor->procesarLectura();
            actual = actual->siguiente;
        }
    }
    
    /**
     * @brief Imprime información de todos los sensores
     */
    void imprimirTodos() const {
        std::cout << "\n--- Información de Sensores Registrados ---" << std::endl;
        NodoSensor* actual = cabeza;
        int contador = 1;
        while (actual != nullptr) {
            std::cout << "\nSensor #" << contador << ":" << std::endl;
            actual->sensor->imprimirInfo();
            actual = actual->siguiente;
            contador++;
        }
    }
    
    /**
     * @brief Obtiene el tamaño de la lista
     * @return Número de sensores
     */
    int getTamanio() const {
        return tamanio;
    }
    
    /**
     * @brief Verifica si la lista está vacía
     * @return true si está vacía
     */
    bool estaVacia() const {
        return cabeza == nullptr;
    }
};

#endif // LISTAGESTION_H