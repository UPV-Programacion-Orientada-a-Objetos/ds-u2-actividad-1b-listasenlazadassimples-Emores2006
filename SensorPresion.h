/**
 * @file SensorPresion.h
 * @brief Sensor especializado para lecturas de presión (int)
 * @author Eliezer Mores Oyervides
 * @date 2025
 */

#ifndef SENSORPRESION_H
#define SENSORPRESION_H

#include "SensorBase.h"
#include "ListaSensor.h"
#include <iostream>
#include <cstdlib>

/**
 * @class SensorPresion
 * @brief Clase derivada que maneja lecturas de presión en enteros
 * 
 * Implementa la funcionalidad específica para sensores de presión,
 * procesando lecturas de tipo int y calculando promedios.
 */
class SensorPresion : public SensorBase {
private:
    ListaSensor<int> historial; ///< Lista genérica para almacenar lecturas int
    
public:
    /**
     * @brief Constructor del sensor de presión
     * @param nom Nombre identificador del sensor
     */
    SensorPresion(const char* nom) : SensorBase(nom) {
        std::cout << " Sensor de Presión '" << nombre << "' creado." << std::endl;
    }
    
    /**
     * @brief Destructor que libera la lista interna
     */
    ~SensorPresion() {
        std::cout << "Liberando Lista Interna del sensor" << nombre  << std::endl;
    }
    
    /**
     * @brief Agrega una lectura de presión a la lista
     * @param valor String con el valor de presión
     */
    void agregarLectura(const char* valor) override {
        int presion = atoi(valor);
        historial.insertar(presion);
        std::cout << "ID: " << nombre << ". Valor: " << presion << " (int)" << std::endl;
    }
    
    /**
     * @brief Procesa las lecturas: calcula el promedio
     * 
     * Implementación específica para presión que calcula el promedio
     * de todas las lecturas almacenadas.
     */
    void procesarLectura() override {
        std::cout << "-> Procesando Sensor " << nombre << "..." << std::endl;
        
        if (historial.estaVacia()) {
            std::cout << "No hay lecturas para procesar." << std::endl;
            return;
        }
        
        int promedio = historial.calcularPromedio();
        std::cout << "[" << nombre << "] (Presion): Promedio de lecturas: " 
                  << promedio << "." << std::endl;
        std::cout << "Promedio calculado sobre " 
                  << historial.getTamanio() << " lecturas (" 
                  << promedio << ")." << std::endl;
    }
    
    /**
     * @brief Imprime información del sensor y sus lecturas
     */
    void imprimirInfo() const override {
        std::cout << "  Sensor: " << nombre << " (Presión - INT)" << std::endl;
        std::cout << "  Lecturas almacenadas: " << historial.getTamanio() << std::endl;
        historial.imprimir();
    }
};

#endif // SENSORPRESION_H