#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "Files.h"
#include "Commands.h"

#include <string>
#include <vector>

using namespace std;

class Environment {
private:
    vector<BaseCommand*> commandsHistory;
    FileSystem fs;

public:
    Environment();
    void start();
    FileSystem& getFileSystem() ; // Get a reference to the file system
    void addToHistory(BaseCommand *command); // Add a new command to the history
    const vector<BaseCommand*>& getHistory() const; // Return a reference to the history of commands
    Environment(Environment& other);
    Environment(Environment&& other);
    ~Environment();
    Environment& operator=(Environment& other);
    Environment& operator=(Environment&& other);
    void clear();
    BaseCommand* findCommand(std::string input);

};

#endif