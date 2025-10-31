/**
 * @mainpage Sistema IoT de Monitoreo Polimórfico
 * @brief Programa principal del Sistema de Gestión Polimórfica de Sensores IoT
 * * Este programa implementa un sistema completo de gestión de sensores que:
 * - Gestiona sensores de forma polimórfica utilizando la clase base SensorBase.
 * - Utiliza listas enlazadas genéricas (ListaSensor<T>) para almacenar lecturas.
 * - Implementa una gestión de memoria cuidadosa (Regla de los Tres/Cinco) y polimorfismo.
 *
 * @section Secciones_Proyecto Estructura del Proyecto:
 * Para ver el detalle de las clases, revisa el menú superior "Clases".
 * @li SensorBase.h: Interfaz abstracta para todos los sensores.
 * @li SensorTemperatura.h: Implementación para datos FLOAT.
 * @li SensorPresion.h: Implementación para datos INT.
 * @li ListaSensor.h: Contenedor genérico (Lista Enlazada) para lecturas.
 * @li ListaGestion.h: Contenedor no genérico para punteros a SensorBase (Polimorfismo).
 * * @author Eliezer Mores Oyervides
 * @date 2025
 */
// sudo chmod 666 /dev/ttyUSB0

#include "SensorBase.h"
#include "SensorTemperatura.h"
#include "SensorPresion.h"
#include "ListaGestion.h"
#include <iostream>
#include <cstring>

// Para comunicación serial en Linux
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

using namespace std;

/**
 * @brief Clase para manejar la comunicación con el puerto serial en Linux
 */
class SerialPort {
private:
    int fd;              ///< File descriptor del puerto serial
    bool conectado;      ///< Estado de la conexión

public:
    /**
     * @brief Constructor - Inicializa el puerto serial
     * @param puerto Nombre del puerto (ej: "/dev/ttyACM0" o "/dev/ttyUSB0")
     */
    SerialPort(const char* puerto) {
        conectado = false;
        
        // Abrir el puerto serial
        fd = open(puerto, O_RDWR | O_NOCTTY);
        
        if (fd != -1) {
            struct termios tty;
            memset(&tty, 0, sizeof(tty));
            
            // Obtener configuración actual
            if (tcgetattr(fd, &tty) == 0) {
                // Configurar velocidad (9600 baud)
                cfsetospeed(&tty, B9600);
                cfsetispeed(&tty, B9600);
                
                // Configurar 8N1 (8 bits, sin paridad, 1 bit de parada)
                tty.c_cflag &= ~PARENB;        // Sin paridad
                tty.c_cflag &= ~CSTOPB;        // 1 bit de parada
                tty.c_cflag &= ~CSIZE;
                tty.c_cflag |= CS8;            // 8 bits por byte
                tty.c_cflag &= ~CRTSCTS;       // Sin control de flujo hardware
                tty.c_cflag |= CREAD | CLOCAL; // Activar lectura, ignorar líneas de control
                
                // Configurar modo raw (sin procesamiento)
                tty.c_lflag &= ~ICANON;        // Modo no canónico
                tty.c_lflag &= ~ECHO;          // Sin eco
                tty.c_lflag &= ~ISIG;          // Sin señales
                
                // Desactivar control de flujo software
                tty.c_iflag &= ~(IXON | IXOFF | IXANY);
                tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);
                
                // Configurar modo de salida raw
                tty.c_oflag &= ~OPOST;
                tty.c_oflag &= ~ONLCR;
                
                // Aplicar configuración
                if (tcsetattr(fd, TCSANOW, &tty) == 0) {
                    conectado = true;
                    cout << "Conectado al puerto " << puerto << endl;
                    sleep(2); // Esperar que Arduino se reinicie
                } else {
                    cout << "No se pudo configurar el puerto " << puerto << endl;
                    close(fd);
                }
            } else {
                cout << "No se pudo leer la configuración del puerto " << puerto << endl;
                close(fd);
            }
        } else {
            cout << "No se pudo abrir el puerto " << puerto << endl;
            cout << "Verifica:" << endl;
            cout << "  1. Que el Arduino esté conectado" << endl;
            cout << "  2. Que tengas permisos: sudo chmod 666 " << puerto << endl;
            cout << "  3. O que estés en el grupo dialout: sudo usermod -a -G dialout $USER" << endl;
        }
    }
    
    /**
     * @brief Destructor - Cierra el puerto serial
     */
    ~SerialPort() {
        if (conectado) {
            close(fd);
            cout << "Puerto cerrado." << endl;
        }
    }
    
    /**
     * @brief Lee una línea del puerto serial
     * @param buffer Buffer donde almacenar los datos
     * @param maxLen Tamaño máximo del buffer
     * @return true si se leyó correctamente
     */
    bool leerLinea(char* buffer, int maxLen) {
        if (!conectado) return false;
        
        int pos = 0;
        char c;
        
        while (pos < maxLen - 1) {
            if (read(fd, &c, 1) > 0) {
                if (c == '\n') {
                    buffer[pos] = '\0';
                    return true;
                }
                if (c != '\r') {
                    buffer[pos++] = c;
                }
            }
        }
        
        buffer[pos] = '\0';
        return false;
    }
    
    /**
     * @brief Verifica si está conectado
     * @return true si está conectado
     */
    bool estaConectado() const {
        return conectado;
    }
};

/**
 * @brief Muestra el menú principal
 */
void mostrarMenu() {
    cout << "\n=== Sistema IoT de Monitoreo Polimórfico ===" << endl;
    cout << "1. Crear Sensor de Temperatura (FLOAT)" << endl;
    cout << "2. Crear Sensor de Presión (INT)" << endl;
    cout << "3. Leer datos del Arduino (modo automático)" << endl;
    cout << "4. Registrar lectura manual" << endl;
    cout << "5. Ejecutar Procesamiento Polimórfico" << endl;
    cout << "6. Mostrar información de sensores" << endl;
    cout << "7. Cerrar Sistema" << endl;
    cout << "Opción: ";
}

/**
 * @brief Determina el tipo de dato a partir del string
 * @param valor String con el valor
 * @return true si es float, false si es int
 */
bool esFloat(const char* valor) {
    for (int i = 0; valor[i] != '\0'; i++) {
        if (valor[i] == '.') return true;
    }
    return false;
}

/**
 * @brief Función principal del programa
 */
int main() {
    ListaGestion gestorSensores;
    SensorBase* sensorTemp = nullptr;
    SensorBase* sensorPres = nullptr;
    
    cout << "--- Sistema IoT de Monitoreo Polimórfico ---\n" << endl;
    
    int opcion;
    do {
        mostrarMenu();
        cin >> opcion;
        cin.ignore(); // Limpiar buffer
        
        switch(opcion) {
            case 1: {
                char nombre[50];
                cout << "Nombre del sensor (ej: T-001): ";
                cin.getline(nombre, 50);
                
                sensorTemp = new SensorTemperatura(nombre);
                gestorSensores.insertar(sensorTemp);
                break;
            }
            
            case 2: {
                char nombre[50];
                cout << "Nombre del sensor (ej: P-105): ";
                cin.getline(nombre, 50);
                
                sensorPres = new SensorPresion(nombre);
                gestorSensores.insertar(sensorPres);
                break;
            }
            
            case 3: {
                char puerto[50];
                cout << "Puerto serial (ej: /dev/ttyACM0 o /dev/ttyUSB0): ";
                cin.getline(puerto, 50);
                
                int numLecturas;
                cout << "Número de lecturas a tomar: ";
                cin >> numLecturas;
                cin.ignore();
                
                SerialPort serial(puerto);
                
                if (serial.estaConectado()) {
                    cout << "\nLeyendo " << numLecturas << " valores del Arduino..." << endl;
                    
                    char buffer[100];
                    int lecturas = 0;
                    
                    while (lecturas < numLecturas) {
                        if (serial.leerLinea(buffer, 100)) {
                            cout << "Valor recibido: " << buffer << endl;
                            
                            // Determinar tipo y asignar al sensor correspondiente
                            if (esFloat(buffer)) {
                                if (sensorTemp != nullptr) {
                                    sensorTemp->agregarLectura(buffer);
                                } else {
                                    cout << "No hay sensor de temperatura creado." << endl;
                                }
                            } else {
                                if (sensorPres != nullptr) {
                                    sensorPres->agregarLectura(buffer);
                                } else {
                                    cout << "No hay sensor de presión creado." << endl;
                                }
                            }
                            lecturas++;
                        }
                    }
                    
                    cout << "Lectura completada." << endl;
                }
                break;
            }
            
            case 4: {
                char nombre[50];
                char valor[50];
                
                cout << "ID del sensor: ";
                cin.getline(nombre, 50);
                cout << "Valor: ";
                cin.getline(valor, 50);
                
                SensorBase* sensor = gestorSensores.buscar(nombre);
                if (sensor != nullptr) {
                    sensor->agregarLectura(valor);
                } else {
                    cout << "Sensor no encontrado." << endl;
                }
                break;
            }
            
            case 5: {
                if (!gestorSensores.estaVacia()) {
                    gestorSensores.procesarTodos();
                } else {
                    cout << "No hay sensores registrados." << endl;
                }
                break;
            }
            
            case 6: {
                if (!gestorSensores.estaVacia()) {
                    gestorSensores.imprimirTodos();
                } else {
                    cout << "No hay sensores registrados." << endl;
                }
                break;
            }
            
            case 7: {
                cout << "\nCerrando sistema y liberando memoria..." << endl;
                break;
            }
            
            default:
                cout << "Opción inválida." << endl;
        }
        
    } while (opcion != 7);
    
    return 0;
}