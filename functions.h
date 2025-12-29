// ============================================
// FILE: functions.h
// ============================================
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>
#include <map>

using namespace std;

// Simple hash function for file content
size_t hashString(const string& str);

// Structure to store file delta (changes)
struct FileDelta {
    string filename;
    vector<string> addedLines;
    vector<string> deletedLines;
    vector<int> modifiedLineNumbers;
};

// Structure to store file information
struct FileInfo {
    string filename;
    size_t hash;
    string content;
    bool isDeleted;
    
    FileInfo();
    FileInfo(const string& name, size_t h, const string& c);
};

// Commit node structure
struct CommitNode {
    int commitId;
    string message;
    string timestamp;
    map<string, FileInfo> files;  // Stores full content for first commit
    vector<FileDelta> deltas;          // Stores deltas for subsequent commits
    vector<string> addedFiles;
    vector<string> deletedFiles;
    CommitNode* parent;
    
    CommitNode();
};

// Mini-Git class
class MiniGit {
private:
    string repoPath;
    string gitDir;
    bool initialized;
    CommitNode* head;
    int nextCommitId;
    map<string, FileInfo> stagingArea;
    map<string, FileInfo> lastCommitState;
    
    // Helper functions
    string getCurrentTimestamp();
    string readFileContent(const string& filepath);
    vector<string> splitLines(const string& content);
    FileDelta computeDelta(const string& filename, 
                          const string& oldContent, 
                          const string& newContent);
    void scanRepository();
    
public:
    MiniGit();
    ~MiniGit();
    
    // Public interface
    void init(const string& path);
    void status();
    void commit(const string& message);
    void log();
};

#endif // FUNCTIONS_H