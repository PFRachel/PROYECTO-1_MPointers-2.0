#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <regex>
#include <variant>
#include <grpcpp/grpcpp.h>
#include "GRPC_Client.h"
#include "Mpointers_lib/Mpointer.h"

// Function to parse command and arguments
bool parseCommand(const std::string& cmd, std::string& operation, std::vector<std::string>& args) {
    // Debug output
    std::cout << "Parsing command: " << cmd << std::endl;

    // Verificar si es una operación de asignación con operador *
    std::regex assignPattern(R"(\s*\*([a-zA-Z0-9_]+)\s*=\s*(.+))");
    std::smatch assignMatch;
    if (std::regex_search(cmd, assignMatch, assignPattern)) {
        std::string varName = assignMatch[1].str();
        std::string value = assignMatch[2].str();

        // Eliminar espacios en blanco al inicio y final
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t;") + 1); // También elimina posible punto y coma al final

        operation = "MPointerAssign";
        args.push_back(varName);
        args.push_back(value);

        std::cout << "Operacion: " << operation << std::endl;
        std::cout << "Argumentos: ";
        for (const auto& a : args) {
            std::cout << "'" << a << "' ";
        }
        std::cout << std::endl;

        return true;
    }

    // Verificar si es una operación de obtención de valor con operador *
    std::regex getValuePattern(R"(\s*([a-zA-Z0-9_]+)\s+([a-zA-Z0-9_]+)\s*=\s*\*([a-zA-Z0-9_]+))");
    std::smatch getValueMatch;
    if (std::regex_search(cmd, getValueMatch, getValuePattern)) {
        std::string type = getValueMatch[1].str();
        std::string newVarName = getValueMatch[2].str();
        std::string pointerName = getValueMatch[3].str();

        operation = "MPointerGetValue";
        args.push_back(type);
        args.push_back(newVarName);
        args.push_back(pointerName);

        std::cout << "Operacion: " << operation << std::endl;
        std::cout << "Argumentos: ";
        for (const auto& a : args) {
            std::cout << "'" << a << "' ";
        }
        std::cout << std::endl;

        return true;
    }

    // Verificar si es un comando de MPointer
    if (cmd.find("MPointer<") != std::string::npos && cmd.find("::New()") != std::string::npos) {
        // Extraer el tipo entre < y >
        size_t typeStart = cmd.find('<') + 1;
        size_t typeEnd = cmd.find('>');
        if (typeStart != std::string::npos && typeEnd != std::string::npos) {
            std::string type = cmd.substr(typeStart, typeEnd - typeStart);

            // Extraer el nombre de la variable
            size_t nameStart = cmd.find_first_not_of(" \t", typeEnd + 1);
            size_t nameEnd = cmd.find('=');
            if (nameStart != std::string::npos && nameEnd != std::string::npos) {
                std::string varName = cmd.substr(nameStart, nameEnd - nameStart);
                varName.erase(0, varName.find_first_not_of(" \t"));
                varName.erase(varName.find_last_not_of(" \t") + 1);

                operation = "MPointerNew";
                args.push_back(type);
                args.push_back(varName);

                std::cout << "Operacion: " << operation << std::endl;
                std::cout << "Argumentos: ";
                for (const auto& a : args) {
                    std::cout << "'" << a << "' ";
                }
                std::cout << std::endl;

                return true;
            }
        }
        return false;
    }

    // Busca el parentesis que abre
    size_t openParen = cmd.find('(');
    if (openParen == std::string::npos) {
        return false;
    }

    // Obtiene el nombre de la operacion
    operation = cmd.substr(0, openParen);
    operation.erase(0, operation.find_first_not_of(" \t"));
    operation.erase(operation.find_last_not_of(" \t") + 1);

    // Busca el parentesis de cierre (el último paréntesis)
    size_t closeParen = cmd.find_last_of(')');
    if (closeParen == std::string::npos) {
        return false;
    }

    // Extra los argumentos
    std::string argsStr = cmd.substr(openParen + 1, closeParen - openParen - 1);

    // Analizar los argumentos tomando en cuenta los parentesis
    std::string currentArg;
    bool inQuotes = false;
    int parenLevel = 0;

    for (size_t i = 0; i < argsStr.length(); ++i) {
        char c = argsStr[i];

        if (c == '"' || c == '\'') {
            inQuotes = !inQuotes;
            currentArg += c;
        } else if (c == '(' && !inQuotes) {
            parenLevel++;
            currentArg += c;
        } else if (c == ')' && !inQuotes) {
            parenLevel--;
            currentArg += c;
        } else if (c == ',' && !inQuotes && parenLevel == 0) {
            // End of argument
            currentArg.erase(0, currentArg.find_first_not_of(" \t"));
            currentArg.erase(currentArg.find_last_not_of(" \t") + 1);
            args.push_back(currentArg);
            currentArg.clear();
        } else {
            currentArg += c;
        }
    }

    // Añade el último argumento
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

// Función para obtener el tamaño de un tipo
size_t getSizeOfType(const std::string& type) {
    if (type == "int") return sizeof(int);
    if (type == "float") return sizeof(float); //Solo funcionan int, float, char, string
    if (type == "double") return sizeof(double);
    if (type == "char") return sizeof(char);
    if (type == "bool") return sizeof(bool);
    if (type == "short") return sizeof(short);
    if (type == "long") return sizeof(long);
    if (type == "long long") return sizeof(long long);
    if (type == "unsigned int") return sizeof(unsigned int);
    if (type == "unsigned char") return sizeof(unsigned char);
    if (type == "unsigned short") return sizeof(unsigned short);
    if (type == "unsigned long") return sizeof(unsigned long);
    if (type == "unsigned long long") return sizeof(unsigned long long);
    // Para string, se determinará en el momento del Set
    if (type == "string") return 1;

    // Tipo desconocido
    std::cerr << "Tipo desconocido: " << type << std::endl;
    return 0;
}

// Estructura para almacenar valores locales
struct LocalValue {
    enum Type { INT, FLOAT, STRING, CHAR } type;
    union {
        int intVal;
        float floatVal;
        char charVal;
    };
    std::string stringVal; // No puede estar en la union

    LocalValue(int val) : type(INT) {
        intVal = val;
    }

    LocalValue(float val) : type(FLOAT) {
        floatVal = val;
    }

    LocalValue(const std::string& val) : type(STRING), stringVal(val) {}

    LocalValue(char val) : type(CHAR) {
        charVal = val;
    }

    std::string toString() const {
        switch (type) {
            case INT: return std::to_string(intVal);
            case FLOAT: return std::to_string(floatVal);
            case STRING: return "\"" + stringVal + "\"";
            case CHAR: return "'" + std::string(1, charVal) + "'";
            default: return "unknown";
        }
    }
};

// Estructura para almacenar los MPointers
struct MPointerVariant {
    enum Type { INT, FLOAT, STRING, CHAR } type;
    union {
        MPointer<int>* intPtr;
        MPointer<float>* floatPtr;
        MPointer<std::string>* stringPtr;
        MPointer<char>* charPtr;
    };

    MPointerVariant(MPointer<int> ptr) : type(INT) {
        intPtr = new MPointer<int>(ptr);
    }

    MPointerVariant(MPointer<float> ptr) : type(FLOAT) {
        floatPtr = new MPointer<float>(ptr);
    }

    MPointerVariant(MPointer<std::string> ptr) : type(STRING) {
        stringPtr = new MPointer<std::string>(ptr);
    }

    MPointerVariant(MPointer<char> ptr) : type(CHAR) {
        charPtr = new MPointer<char>(ptr);
    }

    ~MPointerVariant() {
        switch (type) {
            case INT: delete intPtr; break;
            case FLOAT: delete floatPtr; break;
            case STRING: delete stringPtr; break;
            case CHAR: delete charPtr; break;
        }
    }
};

std::map<std::string, MPointerVariant*> mpointers;
std::map<std::string, LocalValue*> localValues;

int main(int argc, char** argv) {

    std::string target_str = argv[1];

    // Create a gRPC channel
    auto channel = grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials());
    MemoryManagerClient client(channel);

    // Inicializar MPointer con la dirección del servidor
    MPointer<int>::Init(target_str);
    MPointer<float>::Init(target_str);
    MPointer<std::string>::Init(target_str);
    MPointer<char>::Init(target_str);

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

        if (lowerCmd == "exit") {
            std::cout << "Saliendo del cliente.." << std::endl;
            running = false;
            continue;
        }

        std::string operation;
        std::vector<std::string> args;
        if (parseCommand(command, operation, args)) {
            try {
                if (operation == "MPointerNew" && args.size() == 2) {
                    std::string type = args[0];
                    std::string varName = args[1];

                    if (type == "int") {
                        MPointer<int> ptr = MPointer<int>::New();
                        mpointers[varName] = new MPointerVariant(ptr);
                        std::cout << "MPointer<int> " << varName << " creado con ID: " << ptr.getId() << std::endl;
                    } else if (type == "float") {
                        MPointer<float> ptr = MPointer<float>::New();
                        mpointers[varName] = new MPointerVariant(ptr);
                        std::cout << "MPointer<float> " << varName << " creado con ID: " << ptr.getId() << std::endl;
                    } else if (type == "string") {
                        MPointer<std::string> ptr = MPointer<std::string>::New();
                        mpointers[varName] = new MPointerVariant(ptr);
                        std::cout << "MPointer<string> " << varName << " creado con ID: " << ptr.getId() << std::endl;
                    } else if (type == "char") {
                        MPointer<char> ptr = MPointer<char>::New();
                        mpointers[varName] = new MPointerVariant(ptr);
                        std::cout << "MPointer<char> " << varName << " creado con ID: " << ptr.getId() << std::endl;
                    } else {
                        std::cout << "Tipo no soportado: " << std::endl;
                        std::cout << "Solo se aceptan: int, float, string, char" << std::endl;
                    }
                    continue;
                } else if (operation == "MPointerAssign" && args.size() == 2) {
                    std::string varName = args[0];
                    std::string valueStr = args[1];

                    // Chequea si el MPointer existe
                    if (mpointers.find(varName) == mpointers.end()) {
                        std::cout << "Error: MPointer '" << varName << "' no encontrado" << std::endl;
                        continue;
                    }

                    // Obteniete el tipo de Mpointer
                    MPointerVariant* mpv = mpointers[varName];

                    try {
                        // Manejo de los diferentes tipos
                        switch (mpv->type) {
                            case MPointerVariant::INT: {
                                int value = std::stoi(valueStr);
                                // Uso del set
                                int id = mpv->intPtr->getId();
                                std::string response = client.Set(id, value);
                                std::cout << response << std::endl;
                                break;
                            }
                            case MPointerVariant::FLOAT: {
                                float value = std::stof(valueStr);
                                // Uso del set
                                int id = mpv->floatPtr->getId();
                                std::string response = client.Set(id, value);
                                std::cout << response << std::endl;
                                break;
                            }
                            case MPointerVariant::STRING: {
                                // Parse string value (remove quotes if present)
                                std::string value = parseStringValue(valueStr);
                                // Use Set directly with the memory ID
                                int id = mpv->stringPtr->getId();
                                std::string response = client.Set(id, value);
                                std::cout << response << std::endl;
                                break;
                            }
                            case MPointerVariant::CHAR: {
                                char value = valueStr.length() > 0 ? valueStr[0] : '\0';
                                std::string charStr(1, value);
                                int id = mpv->charPtr->getId();
                                std::string response = client.Set(id, charStr);
                                std::cout << response << std::endl;
                                break;
                            }
                        }
                    } catch (const std::exception& e) {
                        std::cout << "Error al asignar valor a *" << varName << ": " << e.what() << std::endl;
                    }
                    continue;
                } else if (operation == "MPointerGetValue" && args.size() == 3) {
                    std::string type = args[0];
                    std::string newVarName = args[1];
                    std::string pointerName = args[2];

                    // Chequea si el MPointer existe
                    if (mpointers.find(pointerName) == mpointers.end()) {
                        std::cout << "Error: MPointer '" << pointerName << "' no encontrado" << std::endl;
                        continue;
                    }

                    // Obtiene variante
                    MPointerVariant* mpv = mpointers[pointerName];

                    try {
                        // majena diferentes tipos
                        switch (mpv->type) {
                            case MPointerVariant::INT: {
                                // Get memory ID
                                int id = mpv->intPtr->getId();
                                // Get the value using the client
                                std::string response = client.Get(id);

                                // Parse la respuesta para extraer el value
                                size_t pos = response.find(": ");
                                if (pos != std::string::npos) {
                                    std::string valueStr = response.substr(pos + 2);
                                    int value = std::stoi(valueStr);

                                    // Almacena el value localmente
                                    if (localValues.find(newVarName) != localValues.end()) {
                                        delete localValues[newVarName];
                                    }
                                    localValues[newVarName] = new LocalValue(value);

                                    std::cout << "Valor " << value << " obtenido de *" << pointerName
                                              << " y almacenado en " << newVarName << std::endl;
                                } else {
                                    std::cout << "Error al parsear la respuesta: " << response << std::endl;
                                }
                                break;
                            }
                            case MPointerVariant::FLOAT: {
                                // Get memory ID
                                int id = mpv->floatPtr->getId();
                                std::string response = client.Get(id);

                                // Parse la respuesta para extraer el value
                                size_t pos = response.find(": ");
                                if (pos != std::string::npos) {
                                    std::string valueStr = response.substr(pos + 2);
                                    float value = std::stof(valueStr);

                                    // Almacena value localmente
                                    if (localValues.find(newVarName) != localValues.end()) {
                                        delete localValues[newVarName];
                                    }
                                    localValues[newVarName] = new LocalValue(value);

                                    std::cout << "Valor " << value << " obtenido de *" << pointerName
                                              << " y almacenado en " << newVarName << std::endl;
                                } else {
                                    std::cout << "Error al parsear la respuesta: " << response << std::endl;
                                }
                                break;
                            }
                            case MPointerVariant::STRING: {
                                // Get memory ID
                                int id = mpv->stringPtr->getId();
                                std::string response = client.Get(id);

                                // Parse
                                size_t pos = response.find(": \"");
                                if (pos != std::string::npos) {
                                    std::string valueStr = response.substr(pos + 3);
                                    // Elimina la )
                                    valueStr = valueStr.substr(0, valueStr.length() - 1);

                                    // almacena value local
                                    if (localValues.find(newVarName) != localValues.end()) {
                                        delete localValues[newVarName];
                                    }
                                    localValues[newVarName] = new LocalValue(valueStr);

                                    std::cout << "Valor \"" << valueStr << "\" obtenido de *" << pointerName
                                              << " y almacenado en " << newVarName << std::endl;
                                } else {
                                    std::cout << "Error al parsear la respuesta: " << response << std::endl;
                                }
                                break;
                            }
                            case MPointerVariant::CHAR: {
                                // Get memory ID
                                int id = mpv->charPtr->getId();
                                std::string response = client.Get(id);

                                // Parse de la respuesta
                                size_t pos = response.find(": \"");
                                if (pos != std::string::npos) {
                                    std::string valueStr = response.substr(pos + 3);
                                    valueStr = valueStr.substr(0, valueStr.length() - 1);
                                    char value = valueStr.length() > 0 ? valueStr[0] : '\0';

                                    // almacena value local
                                    if (localValues.find(newVarName) != localValues.end()) {
                                        delete localValues[newVarName];
                                    }
                                    localValues[newVarName] = new LocalValue(value);

                                    std::cout << "Valor '" << value << "' obtenido de *" << pointerName
                                              << " y almacenado en " << newVarName << std::endl;
                                } else {
                                    std::cout << "Error al parsear la respuesta: " << response << std::endl;
                                }
                                break;
                            }
                        }
                    } catch (const std::exception& e) {
                        std::cout << "Error al obtener valor de *" << pointerName << ": " << e.what() << std::endl;
                    }
                    continue;
                } else if (operation == "Create" && args.size() == 2) {
                    try {
                        // Verificar si el primer argumento es sizeof(tipo)
                        std::string sizeArg = args[0];
                        std::string type = args[1];
                        int size;

                        if (sizeArg.find("sizeof") != std::string::npos) {
                            // Extraer el tipo entre paréntesis
                            size_t typeStart = sizeArg.find('(') + 1;
                            size_t typeEnd = sizeArg.find(')');
                            if (typeStart != std::string::npos && typeEnd != std::string::npos) {
                                std::string sizeType = sizeArg.substr(typeStart, typeEnd - typeStart);
                                size = getSizeOfType(sizeType);
                                if (size == 0) {
                                    std::cout << "Error: No se pudo determinar el tamaño para " << sizeType << std::endl;
                                    continue;
                                }
                            } else {
                                std::cout << "Error: Formato incorrecto para sizeof" << std::endl;
                                continue;
                            }
                        } else if (sizeArg.find("strlen") != std::string::npos) {
                            // Para strings, usar tamaño mínimo inicial que se ajustará en el Set
                            if (type == "string") {
                                size = 1; // Tamaño mínimo inicial
                                std::cout << "Creando string con tamaño inicial mínimo (se ajustará al asignar un valor)" << std::endl;
                            } else {
                                std::cout << "Error: strlen solo se puede usar con tipo string" << std::endl;
                                continue;
                            }
                        } else {
                            // Intentar convertir directamente a entero
                            size = std::stoi(sizeArg);
                        }

                        std::string response = client.Create(size, type);
                        std::cout << response << std::endl;
                        continue;
                    } catch (const std::exception& e) {
                        std::cout << "Error con los argumentos del create: " << e.what() << std::endl;
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
                            // Si no tiene comillas y no es un número, se asume que es un string
                            std::string parsedValue = parseStringValue(valueStr);
                            value = parsedValue;
                            std::cout << "Interpretando '" << valueStr << "' como string: \"" << parsedValue << "\"" << std::endl;
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
                } else if (operation == "IncreaseRefCount" && args.size() == 1) {
                    try {
                        int id = std::stoi(args[0]);
                        std::string response = client.IncreaseRefCount(id);
                        std::cout << response << std::endl;
                        continue;
                    } catch (const std::exception& e) {
                        std::cout << "Error con los argumentos del IncreaseRefCount "<< e.what() << std::endl;
                    }
                } else if (operation == "DecreaseRefCount" && args.size() == 1) {
                    try {
                        int id = std::stoi(args[0]);
                        std::string response = client.DecreaseRefCount(id);
                        std::cout << response << std::endl;
                        continue;
                    } catch (const std::exception& e) {
                        std::cout << "Error con los argumentos del DecreaseRefCount: " << e.what() << std::endl;
                    }
                } else if (operation == "Free" && args.size() == 1) {
                    try {
                        int id = std::stoi(args[0]);
                        std::string response = client.Free(id);
                        std::cout << response << std::endl;
                        continue;
                    } catch (const std::exception& e) {
                        std::cout << "Error con los argumentos del free " << e.what() << std::endl;
                    }
                } else {
                    std::cout << "Comando no reconocido o número incorrecto de argumentos" << std::endl;

                }
            } catch (const std::exception& e) {
                std::cout << "Error al procesar el comando: " << e.what() << std::endl;
            }
        } else {
            std::cout << "Comando no identificado. " << std::endl;
        }
    }

    // Liberar la memoria de los MPointers
    for (auto& pair : mpointers) {
        delete pair.second;
    }
    mpointers.clear();

    // Liberar la memoria de los valores locales
    for (auto& pair : localValues) {
        delete pair.second;
    }
    localValues.clear();

    return 0;
}