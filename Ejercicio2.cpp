#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <semaphore.h>
#include <chrono>  // para simular tiempo de uso del cajero

std::mutex mtx; // Mutex para proteger el acceso a la cuenta compartida
std::counting_semaphore<1> atm_semaphore(1); // Semáforo para permitir acceso de un solo cliente al cajero

double saldo = 100000.0; // Saldo inicial

void cliente(int id, double monto_retiro) {
    atm_semaphore.acquire(); // El cliente toma control del cajero

    std::lock_guard<std::mutex> lock(mtx); // Bloquear el acceso al saldo
    std::cout << "Cliente " << id << " está intentando retirar Q" << monto_retiro << std::endl;

    if (monto_retiro <= saldo) {
        saldo -= monto_retiro;
        std::cout << "Cliente " << id << " ha retirado Q" << monto_retiro << ". Saldo restante: Q" << saldo << std::endl;
    } else {
        std::cout << "Cliente " << id << " no puede retirar Q" << monto_retiro << " debido a saldo insuficiente." << std::endl;
    }

    // Simular tiempo de uso del cajero
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    atm_semaphore.release(); // El cliente libera el cajero
}

int main() {
    int num_clientes;
    std::cout << "Ingrese la cantidad de clientes: ";
    std::cin >> num_clientes;

    std::vector<std::thread> clientes;
    std::vector<double> montos_retiro(num_clientes);

    // Ingresar los montos de retiro para cada cliente
    for (int i = 0; i < num_clientes; ++i) {
        std::cout << "Ingrese el monto que el cliente " << i + 1 << " intentará retirar: Q";
        std::cin >> montos_retiro[i];
    }

    // Crear un hilo para cada cliente
    for (int i = 0; i < num_clientes; ++i) {
        clientes.push_back(std::thread(cliente, i + 1, montos_retiro[i]));
    }

    // Esperar a que todos los hilos terminen
    for (auto& th : clientes) {
        th.join();
    }

    std::cout << "Simulación completada. Saldo final: Q" << saldo << std::endl;

    return 0;
}