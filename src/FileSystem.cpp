#include "../include/FileSystem.h"
#include "../include/Commands.h"
#include "../include/GlobalVariables.h"
#include "../include/Files.h"
#include "iostream"
#include <vector>
#include <string>
FileSystem::FileSystem():rootDirectory(nullptr),workingDirectory(nullptr){ //constructor
    rootDirectory=new Directory("/",nullptr);
    workingDirectory=rootDirectory;
}

Directory& FileSystem::getRootDirectory() const {// Return reference to the root directory
    Directory& outPut=*rootDirectory;
    return outPut;
}

Directory& FileSystem::getWorkingDirectory() const{// Return reference to the root directory
    Directory& outPut=*workingDirectory;
    return outPut;
}


void FileSystem::setWorkingDirectory(Directory *newWorkingDirectory) { // Change the working directory of the file system
    workingDirectory=newWorkingDirectory;
}

FileSystem::~FileSystem(){//distructor
    delete rootDirectory;
    rootDirectory=nullptr;
    workingDirectory=nullptr;
    if(verbose==1||verbose==3)
        std::cout<<"FileSystem::~FileSystem()"<<std::endl;
}

FileSystem::FileSystem(FileSystem& other):rootDirectory(nullptr),workingDirectory(nullptr){ //constructor
    rootDirectory=other.rootDirectory;
    Directory *temp = findDirectory(other.getWorkingDirectory().getAbsolutePath(), rootDirectory);
    this->setWorkingDirectory(temp);
    if(verbose==1||verbose==3)
        std::cout<<"FileSystem::FileSystem(FileSystem& other)"<<std::endl;
}
FileSystem::FileSystem(FileSystem&& other):rootDirectory(nullptr),workingDirectory(nullptr){ //constructor
    rootDirectory=other.rootDirectory;
    Directory *temp = findDirectory(other.getWorkingDirectory().getAbsolutePath(), rootDirectory);
    this->setWorkingDirectory(temp);
    delete &other;
    if(verbose==1||verbose==3)
        std::cout<<"FileSystem::FileSystem(FileSystem&& other)"<<std::endl;
}

FileSystem& FileSystem::operator=(FileSystem &other) {//copy operator
    if(this!=&other) {
        rootDirectory->clear();
        rootDirectory=other.rootDirectory;
        Directory *temp = findDirectory(other.getWorkingDirectory().getAbsolutePath(), rootDirectory);
        this->setWorkingDirectory(temp);
    }
    if(verbose==1||verbose==3)
        std::cout<<"FileSystem& FileSystem::operator=(FileSystem &other) "<<std::endl;
    return *this;
}

FileSystem& FileSystem::operator=(FileSystem &&other) {//move operator
    if (this != &other) {
        rootDirectory->clear();
        rootDirectory = other.rootDirectory;
        Directory *temp = findDirectory(other.getWorkingDirectory().getAbsolutePath(), rootDirectory);
        this->setWorkingDirectory(temp);
        delete &other;
    }
    if(verbose==1||verbose==3)
        std::cout<<"FileSystem& FileSystem::operator=(FileSystem &&other)"<<std::endl;
    return *this;
}

