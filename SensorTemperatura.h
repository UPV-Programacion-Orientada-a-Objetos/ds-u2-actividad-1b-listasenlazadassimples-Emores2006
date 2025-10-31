/**
 * @file SensorTemperatura.h
 * @brief Sensor especializado para lecturas de temperatura (float)
 * @author Eliezer Mores Oyervides
 * @date 2025
 */

#ifndef SENSORTEMPERATURA_H
#define SENSORTEMPERATURA_H

#include "SensorBase.h"
#include "ListaSensor.h"
#include <iostream>
#include <cstdlib>

/**
 * @class SensorTemperatura
 * @brief Clase derivada que maneja lecturas de temperatura en punto flotante
 * 
 * Implementa la funcionalidad específica para sensores de temperatura,
 * procesando lecturas de tipo float y calculando promedios tras eliminar
 * el valor más bajo.
 */
class SensorTemperatura : public SensorBase {
private:
    ListaSensor<float> historial; ///< Lista genérica para almacenar lecturas float
    
public:
    /**
     * @brief Constructor del sensor de temperatura
     * @param nom Nombre identificador del sensor
     */
    SensorTemperatura(const char* nom) : SensorBase(nom) {
        std::cout << "Sensor de Temperatura '" << nombre << "' creado." << std::endl;
    }
    
    /**
     * @brief Destructor que libera la lista interna
     */
    ~SensorTemperatura() {
        std::cout << "  [Destructor Sensor " << nombre << "] Liberando Lista Interna..." << std::endl;
    }
    
    /**
     * @brief Agrega una lectura de temperatura a la lista
     * @param valor String con el valor de temperatura
     */
    void agregarLectura(const char* valor) override {
        float temp = atof(valor);
        historial.insertar(temp);
        std::cout << "ID: " << nombre << ". Valor: " << temp << " (float)" << std::endl;
    }
    
    /**
     * @brief Procesa las lecturas: elimina el mínimo y calcula promedio
     * 
     * Implementación específica para temperatura que elimina la lectura
     * más baja y calcula el promedio de las restantes.
     */
    void procesarLectura() override {
        std::cout << "-> Procesando Sensor " << nombre << "..." << std::endl;
        
        if (historial.estaVacia()) {
            std::cout << "No hay lecturas para procesar." << std::endl;
            return;
        }
        
        if (historial.getTamanio() > 1) {
            float minimo = historial.eliminarMinimo();
            float promedio = historial.calcularPromedio();
            std::cout << "[" << nombre << "] (Temperatura): Lectura más baja ("<< minimo << ") eliminada. Promedio restante: " << promedio << "." << std::endl;
        } else {
            float promedio = historial.calcularPromedio();
            std::cout << "Promedio calculado sobre 1 lectura (" << promedio << ")." << std::endl;
        }
    }
    
    /**
     * @brief Imprime información del sensor y sus lecturas
     */
    void imprimirInfo() const override {
        std::cout << "  Sensor: " << nombre << " (Temperatura - FLOAT)" << std::endl;
        std::cout << "  Lecturas almacenadas: " << historial.getTamanio() << std::endl;
        historial.imprimir();
    }
};

#endif // SENSORTEMPERATURA_H