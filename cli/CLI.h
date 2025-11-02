#pragma once
#include <string>
#include <sstream>
#include <functional>
#include <unordered_map>
#include <vector>
#include "../include/Workspace.h"

/**
 * @class CLI
 * @brief Command-line interface for interacting with the matrix workspace.
 *
 * The CLI (Command-Line Interface) class provides a user interface layer for the
 * Algebra Playground project. It interprets user commands, parses textual input,
 * and delegates operations to the underlying Workspace that manages matrices.
 *
 * Commands are dynamically registered via a command-dispatch table, each with
 * a description and usage format, allowing scalable and extensible command
 * management without hardcoded help text or logic duplication.
 */
class CLI {
private:
    using CommandFunction = std::function<bool(std::istringstream&)>; ///< Type alias for command function handlers.

    /**
     * @struct CommandInfo
     * @brief Encapsulates metadata and behavior for a CLI command.
     */
    struct CommandInfo {
        CommandFunction action;   ///< The callable function that executes the command.
        std::string description;  ///< Human-readable description of what the command does.
        std::string usage;        ///< Syntax example for how to use the command.
    };

    Workspace workspace; ///< Manages matrices and their operations.
    std::vector<std::string> available_commands; ///< Dynamically updated list of commands visible to the user.
    std::unordered_map<std::string, CommandInfo> commands; ///< Maps command names to execution info.

    bool running; ///< Controls the main CLI loop state.
    static constexpr bool RUNNING = true;  ///< Indicates the CLI is active.
    static constexpr bool STOPPING = false; ///< Indicates the CLI should stop.

    // ========================= COMMAND DISPATCH HELPERS =========================

    /**
     * @brief Prints all currently available commands to the console.
     *
     * The list adapts dynamically based on the number of matrices in the workspace:
     *  - 0 matrices: only basic commands like `create`, `load`, `help`, `exit`.
     *  - 1 matrix: adds single-matrix operations (transpose, rank, det, etc.).
     *  - 2+ matrices: adds binary operations (add, multiply, solve, etc.).
     */
    void printAvailableCommands();

    /**
     * @brief Updates the list of available commands depending on workspace state.
     */
    void checkAvailableCommands();

    /**
     * @brief Executes a parsed command line input.
     * @param input The raw command string entered by the user.
     * @return True if the command executed successfully, false otherwise.
     */
    bool executeCommand(const std::string& input);

    // ========================= SPECIFIC COMMAND EXECUTORS =========================

    bool executeCreateCommand(std::istringstream& iss);
    bool executeTransposeCommand(std::istringstream& iss);
    bool executeDeleteCommand(std::istringstream& iss);
    bool executeAssignCommand(std::istringstream& iss);
    bool executeListCommand(std::istringstream&) const;
    bool executeShowCommand(std::istringstream& iss) const;
    bool executeAddCommand(std::istringstream& iss);
    bool executeSubtractCommand(std::istringstream& iss);
    bool executeMultiplyCommand(std::istringstream& iss);
    bool executeScalarMultiplyCommand(std::istringstream& iss);
    bool executeSaveCommand(std::istringstream& iss) const;
    bool executeLoadCommand(std::istringstream& iss);
    bool executeRankCommand(std::istringstream& iss) const;
    bool executeDeterminantCommand(std::istringstream& iss) const;
    bool executeInverseCommand(std::istringstream& iss);
    bool executeSolveCommand(std::istringstream& iss);
    bool execute3DVectorRotationCommand(std::istringstream& iss);

    // ========================= GENERIC HELPER UTILITIES =========================

    /**
     * @brief Verifies that no unexpected arguments remain after parsing input.
     * @param iss Input stream containing the command arguments.
     * @return True if no trailing input exists, false otherwise.
     */
    static bool checkForTrailingInput(std::istringstream& iss);

    /**
     * @brief Displays formatted help text for all or specific commands.
     *
     * If no arguments are provided, all commands are listed.
     * Example: `help multiply` shows details only for the multiply command.
     *
     * @param iss Input stream (may contain a specific command name to filter).
     * @return Always true if executed successfully.
     */
    bool printHelp(std::istringstream& iss) const;

    /**
     * @brief Exits the CLI session gracefully.
     *
     * Prints a farewell message and signals the main loop to stop.
     * Does **not** terminate the program abruptly.
     *
     * @param iss Input stream (should be empty).
     * @return True if executed successfully.
     */
    bool exitCLI(std::istringstream& iss);

    // ========================= GENERIC COMMAND TEMPLATES =========================

    /**
     * @brief Executes a binary matrix operation (e.g., add, subtract, multiply, solve).
     *
     * Parses three names (result, operand1, operand2) and applies the provided function.
     *
     * @param iss Input stream containing user arguments.
     * @param operation Function to execute, with signature `bool(result, A, B)`.
     * @param errorMessage Error message printed if arguments are invalid.
     * @return True if operation succeeded, false otherwise.
     */
    static bool executeBinaryMatrixCommand(
        std::istringstream& iss,
        const std::function<bool(const std::string&, const std::string&, const std::string&)>& operation,
        const std::string& errorMessage
    );

    /**
     * @brief Executes a single-matrix command (e.g., transpose, rank, det, show).
     *
     * Parses one matrix name and executes the provided operation.
     *
     * @param iss Input stream containing user arguments.
     * @param operation Function to execute, with signature `bool(matrixName)`.
     * @param errorMessage Error message printed if arguments are invalid.
     * @return True if operation succeeded, false otherwise.
     */
    static bool executeSingleMatrixCommand(
        std::istringstream& iss,
        const std::function<bool(const std::string&)>& operation,
        const std::string& errorMessage
    );

    /**
     * @brief Executes file-based commands (save or load).
     *
     * Parses a filename argument and applies the provided file operation.
     * Automatically appends `.txt` if missing.
     *
     * @param iss Input stream containing arguments.
     * @param operation Function to execute, with signature `bool(filename)`.
     * @param errorMessage Error message printed if arguments are invalid.
     * @return True if operation succeeded, false otherwise.
     */
    static bool executeSaveLoadCommand(
        std::istringstream& iss,
        const std::function<bool(const std::string&)>& operation,
        const std::string& errorMessage
    );

public:
    // ========================= LIFECYCLE =========================

    /**
     * @brief Constructs the CLI and registers all supported commands.
     *
     * Each command is initialized with its action, description, and usage format.
     * The initial list of visible commands adapts based on workspace state.
     */
    CLI();

    /**
     * @brief Starts the interactive CLI loop.
     *
     * Displays the command prompt, processes user input, and executes commands
     * until the user issues the `exit` command.
     */
    void startCLI();

    /**
     * @brief Default destructor.
     */
    ~CLI() = default;
};
