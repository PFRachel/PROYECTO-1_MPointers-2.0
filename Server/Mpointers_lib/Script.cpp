//
// Created by Rachel on 27/03/2025.
//

#include "MPointer.h"
int main(int argc,char** argv) {
    try {
        // verificamos argumentos
        if (argc != 2) {
            std::cerr << "Uso: " << argv[0] << " <server_address>" << std::endl;
            return -1;
        }
        //inicializar el Mpointer con direccion del servidor
        MPointer<int>::Init(argv[1]);
        std::cout << "Conectado al Memory Manager en: " << argv[1] << std::endl;

        //-----------------------------
        // EJEMPLO uso de MPointer
        MPointer<int> ptr1 = MPointer<int>::New();
        //*ptr1 = 42; // almacenar un valor en memoria remota

        std::cout << "Valor almacenado: " << *ptr1 << std::endl;
        std::cout << "ID de memoria: " << &ptr1 << std::endl;
        // Crear otro puntero
        MPointer<int> ptr2 = MPointer<int>::New();
        //*ptr2 = 100;

        std::cout << "Segundo valor: " << *ptr2 << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;

}