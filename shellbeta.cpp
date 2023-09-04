#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <iterator>
#include <windows.h>

class FileSystemNode {
public:
    std::string name;
    bool isDirectory;
    FileSystemNode* parent;
    std::vector<FileSystemNode*> children;

    FileSystemNode(const std::string& _name, bool _isDirectory, FileSystemNode* _parent)
        : name(_name), isDirectory(_isDirectory), parent(_parent) {}

    std::string getName() const {
        return name;
    }
};

class CommandDB {
    std::unordered_set<std::string> commands;

public:
    CommandDB() {
        commands.insert("ls");
        commands.insert("cd");
        commands.insert("mkdir");
        commands.insert("rm");
        commands.insert("cp");
        commands.insert("pwd");
        commands.insert("sudo");
        commands.insert("ifconfig");
        commands.insert("neofetch");
        commands.insert("nmap");
    }

    bool commandExists(const std::string& command) {
        return commands.find(command) != commands.end();
    }
};

class TerminalShell {
private:
    std::string name;
    FileSystemNode* root;
    FileSystemNode* currentDirectory;
    std::string currentUser;
    bool isRootMode;
    std::string rootPassword;
    CommandDB database;
    std::unordered_set<std::string> validIPs;
public:
    std::string getName() const {
        return name;
    }

    TerminalShell()
        : currentDirectory(nullptr), currentUser("user"), isRootMode(false), rootPassword("admin") {
        validIPs = {"192.168.40.123", "127.0.0.1", "192.168.0.1"};

        // creating the root dir
        root = new FileSystemNode("/", true, nullptr);
        currentDirectory = root;
    }

    ~TerminalShell() {
        // Clean up the file system nodes
        deleteFileSystem(root);
    }

    void deleteFileSystem(FileSystemNode* node) {
        if (node) {
            for (auto child : node->children) {
                deleteFileSystem(child);
            }
            delete node;
        }
    }

    void run() {
        std::string promptColor = "\033[1;32m"; // Default color: Bright green

        while (true) {
            // Displaying prompt with the selected color
            std::cout << promptColor << currentUser << "\033[0m@\033[1;32m" << currentDirectory->getName() << "\033[0m$ ";

            std::string userInput;
            std::getline(std::cin, userInput);

            if (userInput == "exit") {
                std::cout << "Exiting terminal shell." << std::endl;
                break;
            } else if (userInput == "su root") {
                std::cout << "Enter root password: ";
                std::string passwordInput;
                std::getline(std::cin, passwordInput); // Read the entire password

                if (passwordInput == rootPassword) {
                    currentUser = "root";
                    isRootMode = true;
                    promptColor = "\033[1;31m"; // Changing to red color
                } else {
                    std::cout << "Incorrect password. Access Denied." << std::endl;
                }

                continue;
            } else if (userInput == "logout") {
                currentUser = "user";
                isRootMode = false;
                promptColor = "\033[1;32m"; // Changing back to green color
                continue;
            }

            // Split user input into tokens to identify commands like "sudo nano"
            std::istringstream iss(userInput);
            std::vector<std::string> tokens(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());

            if (!tokens.empty()) {
                // Check if the first token is "sudo"
                if (tokens[0] == "sudo") {
                    // Handle sudo commands
                    handleSudoCommand(tokens);
                } else {
                    // Regular command execution
                    executeCommand(userInput);
                }
            }
        }
    }

    void handleSudoCommand(const std::vector<std::string>& tokens) {
        if (tokens.size() < 2) {
            std::cout << "Usage: sudo <command> <options> <arguments>" << std::endl;
            return;
        }

        std::string subCommand = tokens[1];

        if (subCommand == "apt") {
            if (tokens.size() < 4 || tokens[2] != "install") {
                std::cout << "Usage: sudo apt install <package>" << std::endl;
            } else {
                std::string package = tokens[3];
                std::cout << "Reading package lists... Done" << std::endl;
                std::cout << "Building dependency tree" << std::endl;
                std::cout << "Reading state information... Done" << std::endl;
                std::cout << "Package " << package << " has been installed." << std::endl;
            }
        } else if (subCommand == "nano") {
            if (tokens.size() < 3) {
                std::cout << "Usage: sudo nano <file>" << std::endl;
            } else {
                std::string filename = tokens[2];
                std::cout << "Opening " << filename << " with nano text editor..." << std::endl;
                // Implement code to open the file with nano here
            }
        } else if (subCommand == "vi") {
            if (tokens.size() < 3) {
                std::cout << "Usage: sudo vi <file>" << std::endl;
            } else {
                std::string filename = tokens[2];
                std::cout << "Opening " << filename << " with vi text editor..." << std::endl;
                // Implement code to open the file with vi here
            }
        } else {
            std::cout << "Unsupported sudo command: " << subCommand << std::endl;
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
            std::cout << currentDirectory->getName() << std::endl;
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
            if (tokens.size() < 4) {
                std::cout << "Usage: sudo <command> <options> <arguments>" << std::endl;
            } else {
                std::string subCommand = tokens[1];
                if (subCommand == "apt" && tokens[2] == "install") {
                    simulateAptInstall(tokens);
                } else if (subCommand == "nano") {
                    simulateTextEditor("nano", tokens);
                } else if (subCommand == "vi") {
                    simulateTextEditor("vi", tokens);
                } else {
                    std::cout << "Unsupported sudo command: " << subCommand << std::endl;
                }
            }
        } else if (command == "ifconfig") {
            simulateIfconfig();
        } else if (command == "neofetch") {
            simulateNeofetch();
        } else if (command == "nmap") {
            simulateNmap(tokens);
        } else if (!database.commandExists(command)) {
            std::cout << "bash:" << command << ": command not found..." << std::endl;
        }
    }

    void simulateLs() {
        std::cout << "Contents of directory '" << currentDirectory->name << "':" << std::endl;
        for (auto child : currentDirectory->children) {
            std::cout << child->name;
            if (child->isDirectory) {
                std::cout << "/";
            }
            std::cout << "  ";
        }
        std::cout << std::endl;
    }

    void simulateCd(const std::vector<std::string>& tokens) {
        if (tokens.size() < 2) {
            std::cout << "Usage: cd <directory>" << std::endl;
        } else {
            std::string newDirName = tokens[1];
            FileSystemNode* newDir = nullptr;

            // Handle special case: ".." to move up one level
            if (newDirName == "..") {
                if (currentDirectory->parent) {
                    newDir = currentDirectory->parent;
                } else {
                    std::cout << "Already at the root directory." << std::endl;
                }
            } else {
                // Search for the directory in the current directory's children
                for (auto child : currentDirectory->children) {
                    if (child->name == newDirName && child->isDirectory) {
                        newDir = child;
                        break;
                    }
                }
            }

            if (newDir) {
                currentDirectory = newDir;
                std::cout << "Changed directory to: " << currentDirectory->name << std::endl;
            } else {
                std::cout << "Directory not found: " << newDirName << std::endl;
            }
        }
    }

    void simulateMkdir(const std::vector<std::string>& tokens) {
        if (tokens.size() < 2) {
            std::cout << "Usage: mkdir <directory>" << std::endl;
        } else {
            std::string newDirName = tokens[1];
            // Check if a directory with the same name already exists
            bool exists = false;
            for (auto child : currentDirectory->children) {
                if (child->name == newDirName && child->isDirectory) {
                    exists = true;
                    break;
                }
            }
            if (exists) {
                std::cout << "Directory already exists: " << newDirName << std::endl;
            } else {
                FileSystemNode* newDir = new FileSystemNode(newDirName, true, currentDirectory);
                currentDirectory->children.push_back(newDir);
                std::cout << "Created directory: " << newDirName << std::endl;
            }
        }
    }

    void simulateRm(const std::vector<std::string>& tokens) {
        if (tokens.size() < 2) {
            std::cout << "Usage: rm <file/directory>" << std::endl;
        } else {
            std::string targetName = tokens[1];
            // Search for the target in the current directory's children
            for (auto it = currentDirectory->children.begin(); it != currentDirectory->children.end(); ++it) {
                if ((*it)->name == targetName) {
                    FileSystemNode* target = *it;
                    currentDirectory->children.erase(it);
                    delete target;
                    std::cout << "Removed: " << targetName << std::endl;
                    return;
                }
            }
            std::cout << "File or directory not found: " << targetName << std::endl;
        }
    }

    void simulateCp(const std::vector<std::string>& tokens) {
        if (tokens.size() < 3) {
            std::cout << "Usage: cp <source> <destination>" << std::endl;
        } else {
            std::cout << "Copied " << tokens[1] << " to " << tokens[2] << std::endl;
        }
    }

    void simulateAptInstall(const std::vector<std::string>& tokens) {
        if (tokens.size() < 5 || tokens[2] != "install") {
            std::cout << "Usage: sudo apt install <package>" << std::endl;
        } else {
            std::string package = tokens[3];
            std::cout << "Reading package lists... Done" << std::endl;
            std::cout << "Building dependency tree" << std::endl;
            std::cout << "Reading state information... Done" << std::endl;
            std::cout << "Package " << package << " has been installed." << std::endl;
        }
    }

    void simulateTextEditor(const std::string& editor, const std::vector<std::string>& tokens) {
        if (tokens.size() < 3) {
            std::cout << "Usage: sudo " << editor << " <file>" << std::endl;
        } else {
            std::string filename = tokens[2];
            std::cout << "Opening " << filename << " with " << editor << " text editor..." << std::endl;

            std::string editorInput;
            std::ostringstream fileContent;

            std::cout << "Type your text below. Enter 'exit' on a new line to finish editing." << std::endl;

            while (true) {
                std::getline(std::cin, editorInput);
                if (editorInput == "exit") {
                    break;
                }
                fileContent << editorInput << std::endl;
            }

            std::cout << "Saving changes to " << filename << "..." << std::endl;
            // You can save the 'fileContent' to the specified file here.
        }
    }

    void simulateNmap(const std::vector<std::string>& tokens) {
        if (tokens.size() != 3 || tokens[1] != "-p-") {
            std::cout << "Usage: nmap -p- IPAddress" << std::endl;
        } else {
            std::string netstat = tokens[2];

            if (validIPs.find(netstat) != validIPs.end()) {
                std::cout << "Starting Nmap 7.70 (https://nmap.org) at India Standard Time" << std::endl;
                std::cout << "Nmap scan report for the given address" << std::endl;
                std::cout << "Host is up (0.075s latency)" << std::endl;

                if (netstat == "192.168.40.123") {
                    std::cout << "This is a special IP address." << std::endl;
                } else if (netstat == "127.0.0.1") {
                    std::cout << "Loopback address detected." << std::endl;
                } else {
                    std::cout << "Other addresses for the given address" << std::endl;
                    std::cout << "PORT      STATE  SERVICE" << std::endl;
                    std::cout << "22/tcp    open   ssh" << std::endl;
                    std::cout << "25/tcp    open   smtp" << std::endl;
                    std::cout << "53/tcp    open   domain" << std::endl;
                    std::cout << "80/tcp    open   http" << std::endl;
                    std::cout << "443/tcp   open   msrpc" << std::endl;
                    std::cout << "8080/tcp  open   http-proxy" << std::endl;
                    std::cout << "5432/tcp  open   postgresql" << std::endl;
                    std::cout << "61783/tcp open   unknown" << std::endl;
                    std::cout << "65001/tcp open   unknown" << std::endl;
                }
            } else {
                std::cout << "IP Address not recognized in the network. Please check the network and try again..." << std::endl;
            }
        }
    }

    void simulateIfconfig() {
        std::cout << "eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500" << std::endl;
        std::cout << "inet 192.168.1.2  netmask 255.255.255.0  broadcast 192.168.1.255" << std::endl;
        std::cout << "inet6 fe80::a00:27ff:fe45:fa07  prefixlen 64  scopeid 0x20<link>" << std::endl;
        std::cout << "ether 08:00:27:45:fa:07  txqueuelen 1000  (Ethernet)" << std::endl;
        std::cout << "RX packets 175  bytes 14093 (13.7 KiB)" << std::endl;
        std::cout << "RX errors 0  dropped 0  overruns 0  frame 0" << std::endl;
        std::cout << "TX packets 133  bytes 21119 (20.6 KiB)" << std::endl;
        std::cout << "TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0" << std::endl;
    }

    void simulateNeofetch() {
        std::cout << "         .-/+oossssoo+/-.               " << "OS: Ubuntu" << std::endl;
        std::cout << "     `:+ssssssssssssssssss+:`           " << "Host: user" << std::endl;
        std::cout << "   -+ssssssssssssssssssyyssss+-         " << "Kernel: Linux" << std::endl;
        std::cout << " .ossssssssssssssssssdMMMNysssso.       " << "Uptime: 1 hour" << std::endl;
        std::cout << "/ssssssssssshdmmNNmmyNMMMMhssssss/      " << "Packages: 1000" << std::endl;
        std::cout << "+sssshhhyNMMMyhhyyyyhmNMMMNhssss+       " << "Shell: Shell" << std::endl;
        std::cout << "ossyNMMMNyMMh     /mMMMNyNMMNoss        " << "Resolution: 1920x1080" << std::endl;
        std::cout << "+sssshhhyNMMMyhhyyyyhdNMMMNhssss+       " << "DE: Gnome" << std::endl;
        std::cout << "/sssssssssssdmmNNNNNNmmNMMMMhssss/      " << "WM: Gnome Manager" << std::endl;
        std::cout << ".ossssssssssssssssssdMMMNysssso.        " << "Theme: Gnome Black Theme" << std::endl;
        std::cout << " -+sssssssssssssssssyyyssss+-           " << "Icons: Gnome Black Icons" << std::endl;
        std::cout << "   -+ssssssssssssssssssyyssss+-         " << "Terminal: xterm" << std::endl;
        std::cout << "     `:+ssssssssssssssssss+:`           " << "CPU: Ryzen 9 5960x" << std::endl;
        std::cout << "         .-/+oossssoo+/-                " << "GPU: RTX 4090" << std::endl;
        std::cout << "                                        " << "Memory: 64 GB " << std::endl;
        std::cout << "                                        " << "Disk: 1 TB" << std::endl;
    }
};

int main() {
    // Title of the terminal window
    SetConsoleTitle(TEXT("Terminal"));

    TerminalShell shell;
    shell.run();
    return 0;
}
