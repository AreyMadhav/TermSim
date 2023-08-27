#include <iostream>
#include <string>
#include <unordered_set>
#include <sstream>
#include <vector>
#include <iterator>
#include <windows.h>

struct CommandDB {
    std::unordered_set<std::string> commands;

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
        // Add more commands
    }

    bool commandExists(const std::string& command) {
        return commands.find(command) != commands.end();
    }
};

struct TerminalShell {
    std::string currentDirectory;
    std::string currentUser;
    bool isRootMode;
    std::string rootPassword;
    CommandDB database;

    TerminalShell() : currentDirectory("/home/user"), currentUser("user"), isRootMode(false), rootPassword("admin") {}

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
                std::cout<<"Enter root password: ";
                std::string passwordInput;
                std::cin >> passwordInput;

                if (passwordInput == rootPassword) {
                currentUser = "root";
                isRootMode = true;
                continue;
            } else {
            std::cout << "Incorrect password. Access Denied." << std::endl;
            }

            continue;
            }
            if (userInput == "logout") {
                currentUser = "user";
                isRootMode = false;
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
        }
        else if (command == "ls") {
            simulateLs();
        }
        else if (command == "cd") {
            simulateCd(tokens);
        }
        else if (command == "mkdir") {
            simulateMkdir(tokens);
        }
        else if (command == "rm") {
            simulateRm(tokens);
        }
        else if (command == "cp") {
            simulateCp(tokens);
        }
        else if (command == "sudo") {
            simulateSudo(tokens);
        }
        else if (command == "ifconfig") {
            simulateIfconfig();
        }
        else if (command == "neofetch") {
            simulateNeofetch();
        }
        else if (command == "nmap") {
            simulateNmap(tokens);
        }
        else if (!database.commandExists(command)) {
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
        }
        else {
            std::cout << "Changed directory to: " << tokens[1] << std::endl;
            currentDirectory = tokens[1];
        }
    }

    // Simulate mkdir command
    void simulateMkdir(const std::vector<std::string>& tokens) {
        if (tokens.size() < 2) {
            std::cout << "Usage: mkdir <directory>" << std::endl;
        }
        else {
            std::cout << "Created directory: " << tokens[1] << std::endl;
        }
    }

    // Simulate rm command
    void simulateRm(const std::vector<std::string>& tokens) {
        if (tokens.size() < 2) {
            std::cout << "Usage: rm <file/directory>" << std::endl;
        }
        else {
            std::cout << "Removed: " << tokens[1] << std::endl;
        }
    }

    // Simulate cp command
    void simulateCp(const std::vector<std::string>& tokens) {
        if (tokens.size() < 3) {
            std::cout << "Usage: cp <source> <destination>" << std::endl;
        }
        else {
            std::cout << "Copied " << tokens[1] << " to " << tokens[2] << std::endl;
        }
    }

    // Simulate sudo apt install {package} command
    void simulateSudo(const std::vector<std::string>& tokens) {
        if (tokens.size() < 4 || tokens[1] != "apt" || tokens[2] != "install") {
            std::cout << "Usage: sudo apt install {package}" << std::endl;
        }
        else {
            std::string package = tokens[3];
            std::cout << "Reading package lists... Done" << std::endl;
            std::cout << "Building dependency tree" << std::endl;
            std::cout << "Reading state information... Done"<< std::endl;
            std::cout << "Package " << package << " has been installed." << std::endl;
        }
    }

    // Simulate nmap command (need a fix)
    void simulateNmap(const std::vector<std::string>& tokens) {
        if (tokens.size() < 4 || tokens[1] != "-p-" || tokens[2] != "localhost") {
            std::cout << "Usage(as of now): nmap -p- localhost" << std::endl;
        }
        else {
            std::cout << "Starting Nmap 7.70 (https://nmap.org) at India Standard Time" << std::endl;
            std::cout << "Nmap scan report for localhost (127.0.0.1)" << std::endl;
            std::cout << "Host is up (0.075s latency)" << std::endl;
            std::cout << "Other addresses for localhost (not scanned): ::1" << std::endl;
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
    }


    void simulateIfconfig() {
        std::cout << "eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500" << std::endl;
        std::cout << "        inet 192.168.1.2  netmask 255.255.255.0  broadcast 192.168.1.255" << std::endl;
        std::cout << "        inet6 fe80::a00:27ff:fe45:fa07  prefixlen 64  scopeid 0x20<link>" << std::endl;
        std::cout << "        ether 08:00:27:45:fa:07  txqueuelen 1000  (Ethernet)" << std::endl;
        std::cout << "        RX packets 175  bytes 14093 (13.7 KiB)" << std::endl;
        std::cout << "        RX errors 0  dropped 0  overruns 0  frame 0" << std::endl;
        std::cout << "        TX packets 133  bytes 21119 (20.6 KiB)" << std::endl;
        std::cout << "        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0" << std::endl;
    }
    void simulateNeofetch() {
    std::cout << "         .-/+oossssoo+/-.               " << "OS: Ubuntu" << std::endl;
    std::cout << "     `:+ssssssssssssssssss+:`           " << "Host: user" << std::endl;
    std::cout << "   -+ssssssssssssssssssyyssss+-         " << "Kernel: Linux" << std::endl;
    std::cout << " .ossssssssssssssssssdMMMNysssso.       " << "Uptime: 1 hour" << std::endl;
    std::cout << "/ssssssssssshdmmNNmmyNMMMMhssssss/      " << "Packages: 1000" << std::endl;
    std::cout << "+sssshhhyNMMMyhhyyyyhmNMMMNhssss+      " << "Shell: Shell" << std::endl;
    std::cout << "ossyNMMMNyMMh     /mMMMNyNMMNoss      " << "Resolution: 1920x1080" << std::endl;
    std::cout << "+sssshhhyNMMMyhhyyyyhdNMMMNhssss+      " << "DE: Gnome" << std::endl;
    std::cout << "/sssssssssssdmmNNNNNNmmNMMMMhssss/      " << "WM: Gnome Manager" << std::endl;
    std::cout << ".ossssssssssssssssssdMMMNysssso.       " << "Theme: Gnome Black Theme" << std::endl;
    std::cout << " -+sssssssssssssssssyyyssss+-         " << "Icons: Gnome Black Icons" << std::endl;
    std::cout << "   -+ssssssssssssssssssyyssss+-         " << "Terminal: xterm" << std::endl;
    std::cout << "     `:+ssssssssssssssssss+:`           " << "CPU: Ryzen 9 5960x" << std::endl;
    std::cout << "         .-/+oossssoo+/-               " << "GPU: RTX 4090" << std::endl;
    std::cout << "                                        " << "Memory: 64 GB " << std::endl;
    std::cout << "                                        " << "Disk: 1 TB" << std::endl;
}
};

int main() {

    //Title of the terminal window
    SetConsoleTitle(TEXT("Terminal"));

    TerminalShell shell;
    shell.run();
    return 0;
};
