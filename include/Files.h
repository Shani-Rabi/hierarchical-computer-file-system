#ifndef FILES_H_
#define FILES_H_

#include <string>
#include <vector>


class BaseFile {
private:
    std::string name;


public:
    BaseFile(std::string name);
    std::string getName() const;
    void setName(std::string newName);
    virtual int  getSize()=0;
    virtual bool isFile()=0;
    virtual ~BaseFile(){};
    virtual void clear()=0;





};

class File : public BaseFile {
private:
    int size;

public:
    File(std::string name, int size); // Constructor
    File(File& other);
    int getSize(); // Return the size of the file
    bool isFile();//return true;
    ~File();
    File& operator=(File & other);
    virtual void clear();
    File(File && other);
    File& operator=(File &&other);


};



class Directory : public BaseFile {
private:
    std::vector<BaseFile*> children;
    Directory *parent;

public:
    Directory(std::string name, Directory *parent); // Constructor
    Directory(Directory & other);
    Directory *getParent() const; // Return a pointer to the parent of this directory
    void setParent(Directory *newParent); // Change the parent of this directory
    void addFile(BaseFile* file); // Add the file to children
    void removeFile(std::string name); // Remove the file with the specified name from children
    void removeFile(BaseFile* file); // Remove the file from children
    void sortByName(); // Sort children by name alphabetically (not recursively)
    void sortBySize(); // Sort children by size (not recursively)
    std::vector<BaseFile*> getChildren(); // Return children
    int getSize(); // Return the size of the directory (recursively)
    std::string getAbsolutePath();  //Return the path from the root to this
    bool isFile();//return false;
    ~Directory();
    Directory& operator=(Directory & other);
    Directory* copyDirectory(Directory & other);
    void clear();
    std::string getAbsolutePathhelp();
    Directory (Directory && other);
    Directory& operator=(Directory && other);



};
bool checkName (std::string nameToCheck, Directory *parent);
#endif
