/**
 * @file SensorBase.h
 * @brief Clase base abstracta para todos los sensores del sistema IoT
 * @author Eliezer Mores Oyervides
 * @date 2025
 */

#ifndef SENSORBASE_H
#define SENSORBASE_H

#include <cstring>

/**
 * @class SensorBase
 * @brief Clase abstracta que define la interfaz común para todos los sensores
 * 
 * Esta clase base establece el contrato que todas las clases derivadas
 * deben cumplir, implementando métodos virtuales puros para procesamiento
 * e impresión de información.
 */
class SensorBase {
protected:
    char nombre[50]; ///< Identificador único del sensor
    
public:
    /**
     * @brief Constructor que inicializa el nombre del sensor
     * @param nom Nombre identificador del sensor
     */
    SensorBase(const char* nom) {
        strncpy(nombre, nom, 49);
        nombre[49] = '\0';
    }
    
    /**
     * @brief Destructor virtual para garantizar correcta liberación polimórfica
     */
    virtual ~SensorBase() {}
    
    /**
     * @brief Método virtual puro para procesar lecturas del sensor
     * 
     * Cada clase derivada debe implementar su propia lógica de procesamiento
     */
    virtual void procesarLectura() = 0;
    
    /**
     * @brief Método virtual puro para imprimir información del sensor
     * 
     * Cada clase derivada debe implementar su propia forma de mostrar datos
     */
    virtual void imprimirInfo() const = 0;
    
    /**
     * @brief Método virtual puro para agregar una lectura desde string
     * @param valor String que contiene el valor a agregar
     */
    virtual void agregarLectura(const char* valor) = 0;
    
    /**
     * @brief Obtiene el nombre del sensor
     * @return Puntero al nombre del sensor
     */
    const char* getNombre() const {
        return nombre;
    }
};

#endif // SENSORBASE_H