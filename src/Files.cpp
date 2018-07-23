#include "../include/Files.h"
#include "../include/GlobalVariables.h"
#include <typeinfo>
#include <iostream>
#include <string>
#include <vector>

bool checkName (std::string nameToCheck, Directory * parent) {//check if the name is legal and doesnt already exist
    bool isOkayFile = true;

    if(parent!= nullptr){
        for (unsigned int i = 0; isOkayFile && i < parent->getChildren().size(); i++) {
            if (parent->getChildren().at(i)->getName() == nameToCheck)
                isOkayFile = false;
        }
    }

        for (unsigned int i = 0; isOkayFile && i < nameToCheck.length(); i++) {
            char temp = nameToCheck.at(i);
            if (!((temp >= 'a' && temp <= 'z') ||(temp >= 'A' && temp <= 'Z') || (temp >= '0' && temp <= '9')))
                isOkayFile = false;


        }


    return isOkayFile;
}

BaseFile::BaseFile(std::string name):name(name){ //constructor
}

std::string BaseFile::getName() const{
    return name;
}
void BaseFile::setName(std::string newName){ // set a new name
    name=newName;
}


File::File(std::string name, int size):BaseFile(name),size(size){ //constructor

}


File::File(File& other):BaseFile(other.getName()),size(other.getSize()) { //copy constructer
    if(verbose==1 || verbose==3)
        std::cout<<"File::File(File& other)"<<std::endl;

}

File::File(File && other):BaseFile(other.getName()),size(other.getSize()) { // move constructer
    if(verbose==1||verbose==3)
        std::cout<<"File::File(File && other)"<<std::endl;

}

File& File:: operator=(File &&other){//move operator

    if(this!=&other){
        this->setName(other.getName());
        this->size=other.size;
    }
    if(verbose==1||verbose==3)
        std::cout<<"File& File:: operator=(File &&other)"<<std::endl;
    return  *this;
}

bool File::isFile() {// return true- it is file
    return true;
}

int File::getSize() {// Return the size of the file
    return size;
}


File:: ~File(){//distructor
    if(verbose==1||verbose==3)
        std::cout<<"File:: ~File()"<<std::endl;

};

//
File& File::operator=(File & other){ //copy operator
    if(verbose==1||verbose==3)
        std::cout<<"File& File::operator=(File & other)"<<std::endl;
    setName(other.getName());
    size= other.size;

    return *this;
}

void File:: clear(){

}

Directory:: Directory(Directory && other):BaseFile(other.getName()),children(move(other.children)),parent(nullptr) {//move consturctor directory
    other.parent= nullptr;
    if(verbose==1||verbose==3)
        std::cout<<"Directory:: Directory(Directory && other)"<<std::endl;
}


Directory::Directory(std::string name, Directory *parent):BaseFile(name),children(),parent(parent)// Constructor
{

}

Directory* Directory:: copyDirectory(Directory& other ){ //recursivly
    this->setName(other.getName());
    this->children = std::vector<BaseFile *>();
    if (other.children.size() == 0) {
        return this;
    }


    for (unsigned int i = 0; i < other.children.size(); i++) {
        if (other.children.at(i)->isFile()) {
            File *toAdd = new File(other.children.at(i)->getName(),((File*)(other.children.at(i)))->getSize());
            this->addFile(toAdd);
        }
        else {
            Directory * toAdd=new Directory(*(Directory*)other.children.at(i));
            toAdd->setParent(this);

            this->addFile(toAdd);

        }

    }

    return this;
}


Directory::Directory(Directory & other):BaseFile(other.getName()),children(),parent(nullptr) { //copy constructor
    copyDirectory(other);
    if(verbose==1||verbose==3)
        std::cout<<"Directory::Directory(Directory & other)"<<std::endl;

}

Directory::~Directory() {//distructor
    clear();
    if(verbose==1||verbose==3)
        std::cout<<"Directory::~Directory()"<<std::endl;



}





Directory& Directory:: operator=(Directory & other) {//operator = change this to be like other
    if (this != &other)
    {
        clear();
        copyDirectory(other);
    }
    if(verbose==1||verbose==3)
        std::cout<<"Directory& Directory:: operator=(Directory & other)"<<std::endl;
    return *this;


}

void Directory:: clear() { // clear all the fields of the directory, including its children

    for (unsigned int i=0; i<children.size();i++) {

        delete children.at(i);
    }


}


Directory* Directory::getParent() const // Return a pointer to the parent of this directory
{
    return parent;
}


void Directory::setParent(Directory *newParent) // Change the parent of this directory
{    if(parent!=nullptr) {
        bool found = false;
        unsigned int i;
        for (i = 0; i < parent->children.size() && !found; i++) {
            if (parent->children.at(i) == this)
                found = true;
        }
        this->parent->children.erase(children.begin()+i);
    }
    newParent->addFile(this);
}


void Directory::addFile(BaseFile* file) // Add the file to children
{   bool isOkayFile=true;//input check

    isOkayFile = checkName(file->getName(),this);
    if(isOkayFile) {
        children.push_back(file);
    }

    if(!(*file).isFile())
        ((Directory*)(file))->parent=this;

}

void Directory::removeFile(std::string name) // Remove the file with the specified name from children
{
    bool isfound=false;
    for(unsigned int i=0;i<children.size()&&!isfound;i++) {
        if((this->children.at(i))->getName()==name) {
            delete children.at(i);
            children.erase(children.begin()+(i-1));
            isfound= true;
        }

    }

}


void Directory::removeFile(BaseFile* file) // Remove the file from children
{
    if(file!= nullptr) {
        bool isfound = false;
        for (unsigned int i = 0; i < children.size() && !isfound; i++) {
            if (children.at(i) == file) {
                delete children.at(i);
                children.erase(children.begin() + i); //
                isfound = true;

            }

        }
    }

}



void Directory:: sortByName() { // Sort children by name alphabetically (not recursively)
    for (unsigned int i = 0; i < children.size(); i++) {
        for (unsigned int j = i + 1; j < children.size(); j++) {
            if ((*children.at(i)).getName() > (*children.at(j)).getName())//exchange
            {
                BaseFile *temp = children.at(i);
                children.at(i) = children.at(j);
                children.at(j) = temp;
            }
        }
    }
}
void Directory:: sortBySize() {// Sort children by size (not recursively)

    for (unsigned int i = 0; i < children.size(); i++) {
        for (unsigned int j = i + 1; j < children.size(); j++) {
            if ((*children.at(i)).getSize() > (*children.at(j)).getSize())//exchange
            {
                BaseFile *temp = children.at(i);
                children.at(i) = children.at(j);
                children.at(j) = temp;

            }
            else if((*children.at(i)).getSize() == (*children.at(j)).getSize()){//sort aphabetically
                if ((*children.at(i)).getName() > (*children.at(j)).getName())//exchange
                {
                    BaseFile *temp = children.at(i);
                    children.at(i) = children.at(j);
                    children.at(j) = temp;
                }
            }
        }
    }
}
std::vector<BaseFile *> Directory::getChildren() {// Return children
    return children;
}


int  Directory::getSize() { // Return the size of the directory (recursively)
    int toReturnsize=0;
    for (unsigned int i = 0; i <children.size() ; i++) {
        toReturnsize=toReturnsize+(*children.at(i)).getSize();

    }
    return  toReturnsize;

}
std::string  Directory::getAbsolutePath() { //Return the path from the root to this
    if(parent== nullptr)
        return  "/";
    else {
        return this->getAbsolutePathhelp();

    }

}

std::string Directory::getAbsolutePathhelp(){//recursively function that help getAbsulute path
    std::string toReturnstring="";
    if(parent!= nullptr){
        toReturnstring=(*parent).getAbsolutePathhelp()+"/"+(*this).getName();
    }

    return  toReturnstring;
}

bool Directory::isFile() {//return false- it is Directory
    return false;
}

Directory& Directory:: operator=(Directory && other) {//copy operator
    if (this != &other) {
        clear();
        this->parent = nullptr;
        this->setName(other.getName());
        children=move(other.children);
    }
    if(verbose==1||verbose==3)
        std::cout<<"Directory& Directory:: operator=(Directory && other)"<<std::endl;
    return *this;
}
