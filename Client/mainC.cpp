#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <regex>
#include <variant>
#include <grpcpp/grpcpp.h>
#include "GRPC_Client.h"

// Function to parse command and arguments
bool parseCommand(const std::string& cmd, std::string& operation, std::vector<std::string>& args) {
    // Debug output
    std::cout << "Parsing command: " << cmd << std::endl;

    // Busca el parentesis que abre
    size_t openParen = cmd.find('(');
    if (openParen == std::string::npos) {
        return false;
    }

    // Obtiene el nombre de la operacion
    operation = cmd.substr(0, openParen);

    // Busca el parentesis de cierre
    size_t closeParen = cmd.find(')', openParen);
    if (closeParen == std::string::npos) {
        return false;
    }

    // Extra los argumentos
    std::string argsStr = cmd.substr(openParen + 1, closeParen - openParen - 1);

    std::string currentArg;
    bool inQuotes = false;

    for (size_t i = 0; i < argsStr.length(); ++i) {
        char c = argsStr[i];

        if (c == '"' || c == '\'') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            // End of argument
            currentArg.erase(0, currentArg.find_first_not_of(" \t"));
            currentArg.erase(currentArg.find_last_not_of(" \t") + 1);
            args.push_back(currentArg);
            currentArg.clear();
        } else {
            currentArg += c;
        }
    }

    // Annade el ultimo argumento
    if (!currentArg.empty()) {
        currentArg.erase(0, currentArg.find_first_not_of(" \t"));
        currentArg.erase(currentArg.find_last_not_of(" \t") + 1);
        args.push_back(currentArg);
    }

    std::cout << "Operacion: " << operation << std::endl;
    std::cout << "Argumentos: ";
    for (const auto& a : args) {
        std::cout << "'" << a << "' ";
    }
    std::cout << std::endl;

    return true;
}

// Comprueba si el valor string es un entero valido
bool isInteger(const std::string& s) {
    if (s.empty()) return false;

    size_t i = 0;
    if (s[0] == '-' || s[0] == '+') i = 1;

    for (; i < s.length(); ++i) {
        if (!std::isdigit(s[i])) return false;
    }

    return true;
}

// Comprueba si el valor string es un flotante valido
bool isFloat(const std::string& s) {
    if (s.empty()) return false;

    try {
        size_t pos;
        [[maybe_unused]] float value = std::stof(s, &pos);  // Se almacena el valor retornado
        return pos == s.length();
    } catch (...) {
        return false;
    }
}

// Función para analizar el valor del string
std::string parseStringValue(const std::string& s) {
    if (s.length() >= 2 && (s[0] == '"' && s[s.length()-1] == '"' ||
                            s[0] == '\'' && s[s.length()-1] == '\'')) {
        return s.substr(1, s.length() - 2);
    }
    return s;
}

int main(int argc, char** argv) {

    std::string target_str = argv[1];

    // Create a gRPC channel
    auto channel = grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials());
    MemoryManagerClient client(channel);

    std::cout << "Memory Manager Client conectado a  " << target_str << std::endl;
    std::cout << "Escribe 'Exit' para finalizar el cliente" << std::endl;

    std::string command;
    bool running = true;

    while (running) {
        std::cout << "> ";
        std::getline(std::cin, command);

        //convierte los comando para que puedan ser entendidos por el cliente
        std::string lowerCmd = command;
        std::transform(lowerCmd.begin(), lowerCmd.end(), lowerCmd.begin(),
                      [](unsigned char c){ return std::tolower(c); });

        if (lowerCmd == "exit") {
            std::cout << "Exiting client..." << std::endl;
            running = false;
            continue;
        }
        std::string operation;
        std::vector<std::string> args;
        if (parseCommand(command, operation, args)) {
            if (operation == "Create" && args.size() == 2) {
                try {
                    int size = std::stoi(args[0]);
                    std::string type = args[1];
                    std::string response = client.Create(size, type);
                    std::cout << response << std::endl;
                    continue;
                } catch (const std::exception& e) {
                    std::cout << "Error con los argumentoss del create: " << e.what() << std::endl;
                }
            } else if (operation == "Set" && args.size() == 2) {
                try {
                    int id = std::stoi(args[0]);
                    std::string valueStr = args[1];

                    std::variant<int, float, std::string> value;
                    if (isInteger(valueStr)) {
                        value = std::stoi(valueStr);
                    } else if (isFloat(valueStr)) {
                        value = std::stof(valueStr);
                    } else {
                        value = parseStringValue(valueStr);
                    }

                    std::string response = client.Set(id, value);
                    std::cout << response << std::endl;
                    continue;
                } catch (const std::exception& e) {
                    std::cout << "Error con los argumentos del Set " << e.what() << std::endl;
                }
            } else if (operation == "Get" && args.size() == 1) {
                try {
                    int id = std::stoi(args[0]);
                    std::string response = client.Get(id);
                    std::cout << response << std::endl;
                    continue;
                } catch (const std::exception& e) {
                    std::cout << "Error con los argumentos del Get " << e.what() << std::endl;
                }
            }else if (operation == "IncreaseRefCount" && args.size() == 1) {
                try {
                    int id = std::stoi(args[0]);
                    std::string response = client.IncreaseRefCount(id);
                    std::cout << response << std::endl;
                    continue;
                }catch (const std::exception& e) {
                    std::cout << "Error con los argumentos del IncreaseRefCount "<< e.what() << std::endl;
                }
            }else if (operation == "DecreaseRefCount" && args.size() == 1) {
                try {
                    int id = std::stoi(args[0]);
                    std::string response = client.DecreaseRefCount(id);
                    std::cout << response << std::endl;
                    continue;
                }catch (const std::exception& e) {
                    std::cout << "Error con los argumentos del DecreaseRefCount: " << e.what() << std::endl;
                }
            } else if (operation == "Free" && args.size() == 1) {
                try {
                    int id = std::stoi(args[0]);
                    std::string response = client.Free(id);
                    std::cout << response << std::endl;
                    continue;
                }catch (const std::exception& e) {
                    std::cout << "Error con los argumentos del free " << e.what() << std::endl;
                }
            } else if (operation == "Defragment" && args.empty()) {
                try {
                    std::string response = client.Defragment();
                    std::cout << response << std::endl;
                    continue;
                } catch (const std::exception& e) {
                    std::cout << "Error al ejecutar Defragment: " << e.what() << std::endl;
                }
            }

        }
        std::cout << "Comando no identificado" << std::endl;
    }
    return 0;
}