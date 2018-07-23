#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <string>
#include "FileSystem.h"


class BaseCommand {
private:
    std::string args;

public:
    BaseCommand(std::string args);
    std::string getArgs();
    virtual void execute(FileSystem & fs) = 0;
    virtual std::string toString() = 0;
    virtual BaseCommand& getNew()=0;
    virtual ~BaseCommand();
};

class PwdCommand : public BaseCommand {
private:
public:
    PwdCommand(std::string args);
    void execute(FileSystem & fs); // Every derived class should implement this function according to the document (pdf)
    virtual std::string toString();
    PwdCommand& getNew();
};

class CdCommand : public BaseCommand {
private:
public:
    CdCommand(std::string args);
    void execute(FileSystem & fs);
    std::string toString();
    CdCommand& getNew();
};







class LsCommand : public BaseCommand {
private:
public:
    LsCommand(std::string args);
    void execute(FileSystem & fs);
    std::string toString();
    LsCommand& getNew();
};

class MkdirCommand : public BaseCommand {
private:
public:
    MkdirCommand(std::string args);
    void execute(FileSystem & fs);
    std::string toString();
    MkdirCommand& getNew();
};

class MkfileCommand : public BaseCommand {
private:
public:
    MkfileCommand(std::string args);
    void execute(FileSystem & fs);
    std::string toString();
    MkfileCommand& getNew();
};

class CpCommand : public BaseCommand {
private:
public:
    CpCommand(std::string args);
    void execute(FileSystem & fs);
    std::string toString();
    CpCommand& getNew();
};

class MvCommand : public BaseCommand {
private:
public:
    MvCommand(std::string args);
    void execute(FileSystem & fs);
    std::string toString();
    MvCommand& getNew();
};

class RenameCommand : public BaseCommand {
private:
public:
    RenameCommand(std::string args);
    void execute(FileSystem & fs);
    std::string toString();
    RenameCommand& getNew();
};

class RmCommand : public BaseCommand {
private:
public:
    RmCommand(std::string args);
    void execute(FileSystem & fs);
    std::string toString();
    RmCommand& getNew();
};

class HistoryCommand : public BaseCommand {
private:
    const std::vector<BaseCommand *> & history;
public:
    HistoryCommand(std::string args, const std::vector<BaseCommand *> & history);
    void execute(FileSystem & fs);
    std::string toString();
    HistoryCommand& getNew();
};


class VerboseCommand : public BaseCommand {
private:
public:
    VerboseCommand(std::string args);
    void execute(FileSystem & fs);
    std::string toString();
    VerboseCommand& getNew();
};

class ErrorCommand : public BaseCommand {
private:
public:
    ErrorCommand(std::string args);
    void execute(FileSystem & fs);
    std::string toString();
    ErrorCommand& getNew();
};

class ExecCommand : public BaseCommand {
private:
    const std::vector<BaseCommand *> & history;
public:
    ExecCommand(std::string args, const std::vector<BaseCommand *> & history);
    void execute(FileSystem & fs);
    std::string toString();
    ExecCommand& getNew();
};




Directory* findDirectory (std::string Path,Directory* Begin);
void mkDirInsert(std::string Path,Directory* Begin);
std::string removeCommand (std::string);
void copyFileToDestination(BaseFile * toCopy, std::string destinationPath, FileSystem & fs);
std::vector<std::string> splitArgs ( std::string );
void MoveFileToDestination(Directory* directoryOfLastFile,BaseFile * toCopy, std::string destinationPath, FileSystem & fs);





#endif
