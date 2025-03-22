//
// Created by Rachel on 09/03/2025.
//
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

    // Find the opening parenthesis
    size_t openParen = cmd.find('(');
    if (openParen == std::string::npos) {
        return false;
    }

    // Extract the operation name
    operation = cmd.substr(0, openParen);

    // Find the closing parenthesis
    size_t closeParen = cmd.find(')', openParen);
    if (closeParen == std::string::npos) {
        return false;
    }

    // Extract the arguments string
    std::string argsStr = cmd.substr(openParen + 1, closeParen - openParen - 1);

    // Parse arguments, handling quoted strings
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

    // Add the last argument
    if (!currentArg.empty()) {
        currentArg.erase(0, currentArg.find_first_not_of(" \t"));
        currentArg.erase(currentArg.find_last_not_of(" \t") + 1);
        args.push_back(currentArg);
    }

    std::cout << "Parsed operation: " << operation << std::endl;
    std::cout << "Arguments: ";
    for (const auto& a : args) {
        std::cout << "'" << a << "' ";
    }
    std::cout << std::endl;

    return true;
}

// Function to check if a string is a valid integer
bool isInteger(const std::string& s) {
    if (s.empty()) return false;

    size_t i = 0;
    if (s[0] == '-' || s[0] == '+') i = 1;

    for (; i < s.length(); ++i) {
        if (!std::isdigit(s[i])) return false;
    }

    return true;
}

// Function to check if a string is a valid float
bool isFloat(const std::string& s) {
    if (s.empty()) return false;

    try {
        size_t pos;
        std::stof(s, &pos);
        return pos == s.length();
    } catch (...) {
        return false;
    }
}

// Function to parse a string value (remove quotes if present)
std::string parseStringValue(const std::string& s) {
    if (s.length() >= 2 && (s[0] == '"' && s[s.length()-1] == '"' ||
                            s[0] == '\'' && s[s.length()-1] == '\'')) {
        return s.substr(1, s.length() - 2);
    }
    return s;
}

int main(int argc, char** argv) {
    std::string target_str = argv[1];

    // Create del canal de GRPC
    auto channel = grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials());
    MemoryManagerClient client(channel);

    std::cout << "Memory Manager Client conectado a " << target_str << std::endl;
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

        // Comando de salida
        if (lowerCmd == "exit") {
            running = false;
            continue;
        }

        // Peticiones que escuchara el memory manager
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
                    std::cout << "Error con los argumentoss del create " << e.what() << std::endl;
                }
            } else if (operation == "Set" && args.size() == 2) {
                try {
                    int id = std::stoi(args[0]);
                    std::string valueStr = args[1];

                    // Determina el tipo de valor
                    std::variant<int, float, std::string> value;
                    if (isInteger(valueStr)) {
                        value = std::stoi(valueStr);
                    } else if (isFloat(valueStr)) {
                        value = std::stof(valueStr);
                    } else {
                        // Se trata la entrada como un string
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
            }
        }
        std::cout << "Comando no identificado" << std::endl;
    }
    return 0;
}
