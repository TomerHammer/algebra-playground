#include "CLI.h"
#include <sstream>
#include <iostream>
#include <algorithm>



CLI::CLI()
    : workspace(),
      available_commands{
          "create", "load", "help", "exit"
      },
      commands{
          {"create",
              { [this](std::istringstream& iss){ return executeCreateCommand(iss); },
                "Create a new matrix with optional initial value.",
                "create <matName> <rows> <cols> [initValue]" }},
          {"delete",
              { [this](std::istringstream& iss){ return executeDeleteCommand(iss); },
                "Delete a matrix from the workspace.",
                "delete <matName>" }},
          {"assign",
              { [this](std::istringstream& iss){ return executeAssignCommand(iss); },
                "Assign values to a matrix interactively.",
                "assign <matName>" }},
          {"list",
              { [this](std::istringstream& iss){ return executeListCommand(iss); },
                "List all matrices in the workspace.",
                "list" }},
          {"show",
              { [this](std::istringstream& iss){ return executeShowCommand(iss); },
                "Display the contents of a matrix.",
                "show <matName>" }},
          {"add",
              { [this](std::istringstream& iss){ return executeAddCommand(iss); },
                "Add two matrices and store the result.",
                "add <resultName> <mat1Name> <mat2Name>" }},
          {"subtract",
              { [this](std::istringstream& iss){ return executeSubtractCommand(iss); },
                "Subtract one matrix from another and store the result.",
                "subtract <resultName> <mat1Name> <mat2Name>" }},
          {"multiply",
              { [this](std::istringstream& iss){ return executeMultiplyCommand(iss); },
                "Multiply two matrices and store the result.",
                "multiply <resultName> <mat1Name> <mat2Name>" }},
          {"scalar_multiply",
              { [this](std::istringstream& iss){ return executeScalarMultiplyCommand(iss); },
                "Multiply a matrix by a scalar and store the result.",
                "scalar_multiply <resultName> <matName> <scalar>" }},
          {"transpose",
              { [this](std::istringstream& iss){ return executeTransposeCommand(iss); },
                "Transpose a matrix.",
                "transpose <matName>" }},
          {"help",
              { [this](std::istringstream& iss) { return printHelp(iss); },
                "Display this help message.",
                "help" }},
          {"exit",
              { [this](std::istringstream& iss){ return exitCLI(iss); },
                "Exit the CLI.",
                "exit" }},
          {"save",
              { [this](std::istringstream& iss){ return executeSaveCommand(iss); },
                "Save the current workspace to a file.",
                "save <filename>" }},
          {"load",
              { [this](std::istringstream& iss){ return executeLoadCommand(iss); },
                "Load a workspace from a file.",
                "load <filename>" }},
          {"rank",
              { [this](std::istringstream& iss){ return executeRankCommand(iss); },
                "Get the rank of a matrix.",
                "rank <matName>" }},
          {"det",
              { [this](std::istringstream& iss){ return executeDeterminantCommand(iss); },
                "Get the determinant of a matrix.",
                "det <matName>" }},
          {"inverse",
              { [this](std::istringstream& iss){ return executeInverseCommand(iss); },
                "Get the inverse of a matrix and store it.",
                "inverse <resultName> <matName>" }},
          {"solve",
              { [this](std::istringstream& iss){ return executeSolveCommand(iss); },
                "Solve the linear system Ax=b and store the result.",
                "solve <resultName> <matrixA> <columnB>" }}
      },
      running(RUNNING)
{};

void CLI::startCLI() {
    std::cout << "Algebraic Matrix CLI v1.0" << std::endl;
    while (running) {
        printAvailableCommands();
        std::string input;
        std::cout << "> ";
        std::getline(std::cin, input);
        if (input.empty()) continue;

        if (!executeCommand(input)) {
            std::cout << "Command execution failed. Please try again. "
                         "Type 'help' for commands and formats" << std::endl;
        }
    }
}

void CLI::printAvailableCommands() {
    CLI::checkAvailableCommands();

    std::cout << "Available commands:\n";
    for (const auto& cmd : available_commands) {
        std::cout << "  - " << cmd << std::endl;
    }
}

void CLI::checkAvailableCommands() {
    const size_t matrix_count = workspace.getMatrixCount();

    if (matrix_count == 1) {
        available_commands = {
            "create","delete","assign","scalar_multiply",
            "transpose","rank","det","inverse",
            "list", "show","save","load","help","exit"
        };
    } else if (matrix_count >= 2) {
        available_commands = {
            "create", "delete","assign",
            "scalar_multiply","transpose","rank",
            "det","inverse","add","subtract","multiply",
            "solve","list","show","save","load",
            "help", "exit"
          };
    } else {
        available_commands = {
            "create","load",
            "help", "exit"
        };
    }
}

bool CLI::executeCommand(const std::string& input) {
    std::istringstream iss(input);
    std::string command;
    iss >> command;
    if (std::find(available_commands.begin(),available_commands.end(),command) == available_commands.end()) {
        std::cout << "Unknown command: " << command << std::endl;
        return false;
    }

    return commands[command].action(iss);
}

bool CLI::executeCreateCommand(std::istringstream& iss) {
    std::string name;
    int rows = 0, cols = 0;
    double initValue = 0.0; // default

    iss >> name >> rows >> cols;
    if (iss.fail()) {
        std::cout << "Invalid arguments for create command." << std::endl;
        return false;
    }

    // Try reading optional initValue
    if (!(iss >> initValue)) {
        iss.clear();     // clear fail
    }

    // Check for trailing input
    if (!checkForTrailingInput(iss)) {
        std::cout << "Invalid arguments for create command." << std::endl;
        return false;
    }

    return workspace.createMatrix(name, rows, cols, initValue);
}

bool CLI::executeTransposeCommand(std::istringstream &iss) {
    return executeSingleMatrixCommand(iss,
            [this](const std::string& name) { return workspace.transposeMatrix(name); },
            "Invalid arguments for transpose command.");
}

bool CLI::executeDeleteCommand(std::istringstream &iss) {
    return executeSingleMatrixCommand(iss,
            [this](const std::string& name) { return workspace.deleteMatrix(name); },
            "Invalid arguments for delete command.");
}

bool CLI::executeAssignCommand(std::istringstream &iss) {
    return executeSingleMatrixCommand(iss,
            [this](const std::string& name) { return workspace.assignMatrix(name); },
            "Invalid arguments for assign command.");
}

bool CLI::executeListCommand(std::istringstream &) const {
    return workspace.listMatrices();
}

bool CLI::executeShowCommand(std::istringstream &iss) const{
    return executeSingleMatrixCommand(iss,
            [this](const std::string& name) { return workspace.showMatrix(name); },
            "Invalid arguments for show command.");
}

bool CLI::executeAddCommand(std::istringstream& iss) {
    return executeBinaryMatrixCommand(iss,
        [this](const std::string& r, const std::string& m1, const std::string& m2) {
            return workspace.addMatrices(r, m1, m2);
        },
        "Invalid arguments for add command.");
}

bool CLI::executeSubtractCommand(std::istringstream& iss) {
    return executeBinaryMatrixCommand(iss,
        [this](const std::string& r, const std::string& m1, const std::string& m2) {
            return workspace.subtractMatrices(r, m1, m2);
        },
        "Invalid arguments for subtract command.");
}

bool CLI::executeMultiplyCommand(std::istringstream& iss) {
    return executeBinaryMatrixCommand(iss,
        [this](const std::string& r, const std::string& m1, const std::string& m2) {
            return workspace.multiplyMatrices(r, m1, m2);
        },
        "Invalid arguments for multiply command.");
}

bool CLI::executeScalarMultiplyCommand(std::istringstream& iss) {
    std::string resultName,matName;
    double scalar;
    iss >> resultName >> matName >> scalar;
    if (iss.fail() || !checkForTrailingInput(iss)) {
        std::cout << "Invalid arguments for scalar_multiply command." << std::endl;
        return false;
    }
    return workspace.scalarMultiplyMatrix(resultName, matName, scalar);
}

bool CLI::printHelp(std::istringstream& iss) const {
    if (!checkForTrailingInput(iss)) return false;
    std::cout << "Available commands:\n";
    for (const auto& pair  : commands) {
        const auto& usage = pair.second.usage;
        const auto& description = pair.second.description;
        const auto& name = pair.first;
        std::cout << "  - " << name << " : " << usage << "\n"
                  << "      " << description << "\n\n";
    }
    return true;
}

bool CLI::exitCLI(std::istringstream& iss) {
    if (!checkForTrailingInput(iss)) return false;
    std::cout << "Exiting CLI." << std::endl;
    running = STOPPING;
    return true;
}

bool CLI::executeSaveCommand(std::istringstream &iss) const{
    return executeSaveLoadCommand(iss,
        [this](const std::string& filename) { return workspace.saveWorkspaceToFile(filename); },
        "Invalid arguments for save command.");
}

bool CLI::executeLoadCommand(std::istringstream &iss) {
    return executeSaveLoadCommand(iss,
        [this](const std::string& filename) { return workspace.loadWorkspaceFromFile(filename); },
        "Invalid arguments for load command.");
}

bool CLI::executeBinaryMatrixCommand(
        std::istringstream& iss,
        const std::function<bool(const std::string&, const std::string&, const std::string&)>& operation,
        const std::string& errorMessage
    ) {
    std::string resultName, mat1Name, mat2Name;
    iss >> resultName >> mat1Name >> mat2Name;
    if (resultName.empty() || mat1Name.empty() || mat2Name.empty() || !checkForTrailingInput(iss)) {
        std::cout <<  errorMessage << std::endl;
        return false;
    }
    return operation(resultName, mat1Name, mat2Name);
}

bool CLI::executeSingleMatrixCommand(
        std::istringstream& iss,
        const std::function<bool(const std::string&)>& operation,
        const std::string& errorMessage
    ) {
    std::string matrixName;
    iss >> matrixName;
    if (matrixName.empty() || !checkForTrailingInput(iss)) {
        std::cout << errorMessage << std::endl;
        return false;
    }
    return operation(matrixName);
}

bool CLI::executeSaveLoadCommand(std::istringstream& iss,
    const std::function<bool(const std::string &)>& operation,
    const std::string &errorMessage) {
    std::string filename;
    iss >> filename;
    if (filename.empty() || !checkForTrailingInput(iss)) {
        std::cout << errorMessage << std::endl;
        return false;
    }
    if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".txt") {
        filename += ".txt";
    }

    return operation(filename);
}

bool CLI::checkForTrailingInput(std::istringstream& iss) {
    std::string trailing;
    if (iss >> trailing) return false;

    return true;
}

bool CLI::executeRankCommand(std::istringstream& iss) const{
    return executeSingleMatrixCommand(iss,[this](const std::string& name) { return workspace.rankMatrix(name); },
            "Invalid arguments for rank command.");
}

bool CLI::executeDeterminantCommand(std::istringstream& iss) const{
    return executeSingleMatrixCommand(iss,
            [this](const std::string& name) { return workspace.determinantMatrix(name); },
            "Invalid arguments for determinant command.");
}

bool CLI::executeInverseCommand(std::istringstream& iss)  {
    std::string resultName, matName;
    iss >> resultName >> matName;
    if (resultName.empty() || matName.empty() || !checkForTrailingInput(iss)) {
        std::cout << "Invalid arguments for inverse command." << std::endl;
        return false;
    }
    return workspace.inverseMatrix(resultName, matName);
}

bool CLI::executeSolveCommand(std::istringstream& iss) {
    return executeBinaryMatrixCommand(iss,
        [this](const std::string& r, const std::string& A, const std::string& b) {
            return workspace.solveMatrix(r, A, b);
        },
        "Invalid arguments for solve command.");
}