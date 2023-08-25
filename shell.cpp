#include <iostream>
#include <string>
#include <unordered_set>
#include <sstream>
#include <vector>
#include <iterator>

struct CommandDB {
    std::unordered_set<std::string> commands;

    CommandDB() {
        commands.insert("ls");
        commands.insert("cd");
        commands.insert("mkdir");
        commands.insert("rm");
        commands.insert("cp");
        commands.insert("pwd");
        commands.insert("sudo"); // Add the sudo command
        // Add more commands
    }

    bool commandExists(const std::string& command) {
        return commands.find(command) != commands.end();
    }
};

struct TerminalShell {
    std::string currentDirectory;
    std::string currentUser;
    CommandDB database;

    TerminalShell() : currentDirectory("/home/user"), currentUser("user") {}

    void run() {
        while (true) {
            std::cout << currentUser << "@" << currentDirectory << "$ ";
            std::string userInput;
            std::getline(std::cin, userInput);

            if (userInput == "exit") {
                std::cout << "Exiting terminal shell." << std::endl;
                break;
            }

            if (userInput == "su root") {
                currentUser = "root";
                continue;
            }

            executeCommand(userInput);
        }
    }

    void executeCommand(const std::string& userInput) {
        std::istringstream iss(userInput);
        std::vector<std::string> tokens(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());

        if (tokens.empty()) {
            return;
        }

        std::string command = tokens[0];

        if (command == "pwd") {
            std::cout << currentDirectory << std::endl;
        } else if (command == "ls") {
            simulateLs();
        } else if (command == "cd") {
            simulateCd(tokens);
        } else if (command == "mkdir") {
            simulateMkdir(tokens);
        } else if (command == "rm") {
            simulateRm(tokens);
        } else if (command == "cp") {
            simulateCp(tokens);
        } else if (command == "sudo") {
            simulateSudo(tokens);
        } else if (!database.commandExists(command)) {
            std::cout << "bash:" << command << ": command not found..." << std::endl;
        }
    }

    // Simulate ls command
    void simulateLs() {
        std::cout << "file1.txt  file2.cpp  folder1  folder2" << std::endl;
    }

    // Simulate cd command
    void simulateCd(const std::vector<std::string>& tokens) {
        if (tokens.size() < 2) {
            std::cout << "Usage: cd <directory>" << std::endl;
        } else {
            std::cout << "Changed directory to: " << tokens[1] << std::endl;
            currentDirectory = tokens[1];
        }
    }

    // Simulate mkdir command
    void simulateMkdir(const std::vector<std::string>& tokens) {
        if (tokens.size() < 2) {
            std::cout << "Usage: mkdir <directory>" << std::endl;
        } else {
            std::cout << "Created directory: " << tokens[1] << std::endl;
        }
    }

    // Simulate rm command
    void simulateRm(const std::vector<std::string>& tokens) {
        if (tokens.size() < 2) {
            std::cout << "Usage: rm <file/directory>" << std::endl;
        } else {
            std::cout << "Removed: " << tokens[1] << std::endl;
        }
    }

    // Simulate cp command
    void simulateCp(const std::vector<std::string>& tokens) {
        if (tokens.size() < 3) {
            std::cout << "Usage: cp <source> <destination>" << std::endl;
        } else {
            std::cout << "Copied " << tokens[1] << " to " << tokens[2] << std::endl;
        }
    }

    // Simulate sudo apt install {package} command
    void simulateSudo(const std::vector<std::string>& tokens) {
        if (tokens.size() < 4 || tokens[1] != "apt" || tokens[2] != "install") {
            std::cout << "Usage: sudo apt install {package}" << std::endl;
        } else {
            std::string package = tokens[3];
            std::cout << "Simulating installation of package: " << package << std::endl;
            std::cout << "Package " << package << " has been installed." << std::endl;
        }
    }
};

int main() {
    TerminalShell shell;
    shell.run();
    return 0;
}
