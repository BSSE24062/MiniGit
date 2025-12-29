// ============================================
// FILE: main.cpp
// ============================================
#include "functions.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main() {
    MiniGit git;
    string command;
    
    cout << "=================================\n";
    cout << "    Mini-Git Version Control     \n";
    cout << "=================================\n";
    cout << "Commands: init, status, commit, log, help, exit\n\n";
    
    while (true) {
        cout << "mini-git> ";
        getline(cin, command);
        
        if (command.empty()) continue;
        
        stringstream ss(command);
        string cmd;
        ss >> cmd;
        
        if (cmd == "init") {
            string path;
            ss >> path;
            if (path.empty()) {
                cout << "Usage: init <path>\n";
            } else {
                git.init(path);
            }
        }
        else if (cmd == "status") {
            git.status();
        }
        else if (cmd == "commit") {
            string message;
            getline(ss, message);
            if (message.empty()) {
                cout << "Usage: commit <message>\n";
            } else {
                // Remove leading space
                if (message[0] == ' ') message = message.substr(1);
                git.commit(message);
            }
        }
        else if (cmd == "log") {
            git.log();
        }
        else if (cmd == "help") {
            cout << "\nAvailable commands:\n";
            cout << "  init <path>     - Initialize repository at path\n";
            cout << "  status          - Show working tree status\n";
            cout << "  commit <msg>    - Commit changes with message\n";
            cout << "  log             - Show commit history\n";
            cout << "  help            - Show this help message\n";
            cout << "  exit            - Exit the program\n\n";
        }
        else if (cmd == "exit") {
            cout << "Goodbye!\n";
            break;
        }
        else {
            cout << "Unknown command: " << cmd << "\n";
            cout << "Type 'help' for available commands.\n";
        }
    }
    
    return 0;
}