
// ============================================
// FILE: functions.cpp
// ============================================
#include "functions.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <functional>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>

using namespace std;
namespace fs = filesystem;

// Hash function implementation
size_t hashString(const string& str) {
    hash<string> hasher;
    return hasher(str);
}

// FileInfo implementations
FileInfo::FileInfo() : hash(0), isDeleted(false), isDirectory(false) {}

FileInfo::FileInfo(const string& name, size_t h, const string& c, bool isDir) 
    : filename(name), hash(h), content(c), isDeleted(false), isDirectory(isDir) {}

// CommitNode implementation
CommitNode::CommitNode() : commitId(0), parent(nullptr) {}

// MiniGit implementations
MiniGit::MiniGit() : initialized(false), head(nullptr), nextCommitId(1) {}

MiniGit::~MiniGit() {
    // Clean up commit history
    CommitNode* current = head;
    while (current != nullptr) {
        CommitNode* temp = current;
        current = current->parent;
        delete temp;
    }
}

string MiniGit::getCurrentTimestamp() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    stringstream ss;
    ss << put_time(ltm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

string MiniGit::readFileContent(const string& filepath) {
    ifstream file(filepath);
    if (!file.is_open()) return "";
    
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

vector<string> MiniGit::splitLines(const string& content) {
    vector<string> lines;
    stringstream ss(content);
    string line;
    while (getline(ss, line)) {
        lines.push_back(line);
    }
    return lines;
}

FileDelta MiniGit::computeDelta(const string& filename, 
                                const string& oldContent, 
                                const string& newContent) {
    FileDelta delta;
    delta.filename = filename;
    
    auto oldLines = splitLines(oldContent);
    auto newLines = splitLines(newContent);
    
    // Simple diff algorithm
    size_t minSize = min(oldLines.size(), newLines.size());
    
    // Find modified lines
    for (size_t i = 0; i < minSize; i++) {
        if (oldLines[i] != newLines[i]) {
            delta.modifiedLineNumbers.push_back(i);
            delta.deletedLines.push_back(oldLines[i]);
            delta.addedLines.push_back(newLines[i]);
        }
    }
    
    // Handle added lines
    if (newLines.size() > oldLines.size()) {
        for (size_t i = minSize; i < newLines.size(); i++) {
            delta.addedLines.push_back(newLines[i]);
        }
    }
    
    // Handle deleted lines
    if (oldLines.size() > newLines.size()) {
        for (size_t i = minSize; i < oldLines.size(); i++) {
            delta.deletedLines.push_back(oldLines[i]);
        }
    }
    
    return delta;
}

void MiniGit::scanRepository() {
    stagingArea.clear();
    
    if (!fs::exists(repoPath)) {
        cout << "Repository path does not exist!\n";
        return;
    }
    
    // Recursive function to scan directory and subdirectories
    function<void(const string&, const string&)> scanDir = [&](const string& path, const string& relativePath) {
        for (const auto& entry : fs::directory_iterator(path)) {
            string filename = entry.path().filename().string();
            
            // Skip .minigit directory
            if (filename.find(".minigit") != string::npos) continue;
            
            string relPath = relativePath.empty() ? filename : relativePath + "/" + filename;
            
            if (entry.is_directory()) {
                // Track the directory itself
                stagingArea[relPath] = FileInfo(relPath, 0, "", true);
                
                // Recursively scan subdirectories
                scanDir(entry.path().string(), relPath);
            }
            else if (entry.is_regular_file()) {
                string content = readFileContent(entry.path().string());
                size_t hash = hashString(content);
                
                stagingArea[relPath] = FileInfo(relPath, hash, content, false);
            }
        }
    };
    
    // Start scanning from root
    scanDir(repoPath, "");
}

void MiniGit::init(const string& path) {
    repoPath = path;
    
    // Create repository directory if it doesn't exist
    if (!fs::exists(repoPath)) {
        fs::create_directories(repoPath);
        cout << "Created directory: " << repoPath << "\n";
    }
    
    gitDir = repoPath + "/.minigit";
    
    // Create .minigit directory
    if (!fs::exists(gitDir)) {
        fs::create_directory(gitDir);
    }
    
    initialized = true;
    cout << "Initialized mini-git repository in " << repoPath << "\n";
}

void MiniGit::status() {
    if (!initialized) {
        cout << "Repository not initialized! Use 'init' first.\n";
        return;
    }
    
    scanRepository();
    
    cout << "\n=== Repository Status ===\n";
    
    // Check for new files and directories
    vector<string> newFiles;
    vector<string> newDirs;
    for (const auto& [filename, fileInfo] : stagingArea) {
        if (lastCommitState.find(filename) == lastCommitState.end()) {
            if (fileInfo.isDirectory) {
                newDirs.push_back(filename);
            } else {
                newFiles.push_back(filename);
            }
        }
    }
    
    // Check for modified files
    vector<string> modifiedFiles;
    for (const auto& [filename, fileInfo] : stagingArea) {
        auto it = lastCommitState.find(filename);
        if (it != lastCommitState.end() && !fileInfo.isDirectory && 
            it->second.hash != fileInfo.hash) {
            modifiedFiles.push_back(filename);
        }
    }
    
    // Check for deleted files and directories
    vector<string> deletedFiles;
    vector<string> deletedDirs;
    for (const auto& [filename, fileInfo] : lastCommitState) {
        if (stagingArea.find(filename) == stagingArea.end()) {
            if (fileInfo.isDirectory) {
                deletedDirs.push_back(filename);
            } else {
                deletedFiles.push_back(filename);
            }
        }
    }
    
    if (newFiles.empty() && newDirs.empty() && modifiedFiles.empty() && 
        deletedFiles.empty() && deletedDirs.empty()) {
        cout << "Nothing to commit, working tree clean\n";
    } else {
        if (!newDirs.empty()) {
            cout << "\nNew directories:\n";
            for (const auto& dir : newDirs) {
                cout << "  + " << dir << "/\n";
            }
        }
        
        if (!newFiles.empty()) {
            cout << "\nNew files:\n";
            for (const auto& file : newFiles) {
                cout << "  + " << file << "\n";
            }
        }
        
        if (!modifiedFiles.empty()) {
            cout << "\nModified files:\n";
            for (const auto& file : modifiedFiles) {
                cout << "  M " << file << "\n";
            }
        }
        
        if (!deletedDirs.empty()) {
            cout << "\nDeleted directories:\n";
            for (const auto& dir : deletedDirs) {
                cout << "  - " << dir << "/\n";
            }
        }
        
        if (!deletedFiles.empty()) {
            cout << "\nDeleted files:\n";
            for (const auto& file : deletedFiles) {
                cout << "  - " << file << "\n";
            }
        }
    }
    cout << "\n";
}

void MiniGit::commit(const string& message) {
    if (!initialized) {
        cout << "Repository not initialized! Use 'init' first.\n";
        return;
    }
    
    scanRepository();
    
    CommitNode* newCommit = new CommitNode();
    newCommit->commitId = nextCommitId++;
    newCommit->message = message;
    newCommit->timestamp = getCurrentTimestamp();
    newCommit->parent = head;
    
    if (head == nullptr) {
        // First commit - store full file contents
        cout << "Creating initial commit...\n";
        for (const auto& [filename, fileInfo] : stagingArea) {
            newCommit->files[filename] = fileInfo;
            if (fileInfo.isDirectory) {
                cout << "  Added: " << filename << "/ (directory)\n";
            } else {
                cout << "  Added: " << filename << " (hash: " << fileInfo.hash << ")\n";
            }
        }
    } else {
        // Subsequent commits - compute and store deltas
        cout << "Creating commit...\n";
        
        // Find new files and directories
        for (const auto& [filename, fileInfo] : stagingArea) {
            if (lastCommitState.find(filename) == lastCommitState.end()) {
                newCommit->addedFiles.push_back(filename);
                newCommit->files[filename] = fileInfo;
                if (fileInfo.isDirectory) {
                    cout << "  Added: " << filename << "/ (directory)\n";
                } else {
                    cout << "  Added: " << filename << "\n";
                }
            }
        }
        
        // Find modified files and compute deltas
        for (const auto& [filename, fileInfo] : stagingArea) {
            auto it = lastCommitState.find(filename);
            if (it != lastCommitState.end() && !fileInfo.isDirectory && 
                it->second.hash != fileInfo.hash) {
                FileDelta delta = computeDelta(filename, it->second.content, fileInfo.content);
                newCommit->deltas.push_back(delta);
                cout << "  Modified: " << filename << " (delta stored)\n";
                cout << "    Changes: " << delta.modifiedLineNumbers.size() 
                          << " lines modified, " << delta.addedLines.size() << " lines added/changed\n";
            }
        }
        
        // Find deleted files and directories
        for (const auto& [filename, fileInfo] : lastCommitState) {
            if (stagingArea.find(filename) == stagingArea.end()) {
                newCommit->deletedFiles.push_back(filename);
                if (fileInfo.isDirectory) {
                    cout << "  Deleted: " << filename << "/ (directory)\n";
                } else {
                    cout << "  Deleted: " << filename << "\n";
                }
            }
        }
    }
    
    head = newCommit;
    lastCommitState = stagingArea;
    
    cout << "\n[Commit " << newCommit->commitId << "] " << message << "\n";
    cout << "Timestamp: " << newCommit->timestamp << "\n\n";
}

void MiniGit::log() {
    if (!initialized) {
        cout << "Repository not initialized! Use 'init' first.\n";
        return;
    }
    
    if (head == nullptr) {
        cout << "No commits yet.\n";
        return;
    }
    
    cout << "\n=== Commit History ===\n\n";
    
    CommitNode* current = head;
    while (current != nullptr) {
        cout << "Commit ID: " << current->commitId << "\n";
        cout << "Message: " << current->message << "\n";
        cout << "Timestamp: " << current->timestamp << "\n";
        
        if (!current->addedFiles.empty()) {
            cout << "Added files: ";
            for (const auto& file : current->addedFiles) {
                cout << file << " ";
            }
            cout << "\n";
        }
        
        if (!current->deltas.empty()) {
            cout << "Modified files: ";
            for (const auto& delta : current->deltas) {
                cout << delta.filename << " ";
            }
            cout << "\n";
        }
        
        if (!current->deletedFiles.empty()) {
            cout << "Deleted files: ";
            for (const auto& file : current->deletedFiles) {
                cout << file << " ";
            }
            cout << "\n";
        }
        
        cout << "---\n\n";
        current = current->parent;
    }
}