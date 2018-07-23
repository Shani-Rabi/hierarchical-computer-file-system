#include "../include/Commands.h"
#include "../include/GlobalVariables.h"
#include "../include/Files.h"
#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>

std::vector<std::string> splitArgs (std::string args){
    // split the args we get to the sourcePath, the name of the directory that includes the last file/Directory,
    //and the desination path

    std::string tempArgs = removeCommand(args); // /removes the command from the string
    std::string sourchPath = "";
    std::string destinationPath = "";




    char currentLetter = tempArgs.at(0); // get the source path
    while (currentLetter!=' ') {
        sourchPath = sourchPath + currentLetter;
        tempArgs=tempArgs.substr(1);
        currentLetter = tempArgs.at(0);
    }
    while (currentLetter == ' ') { // get the destination path
        tempArgs=tempArgs.substr(1);
        currentLetter = tempArgs.at(0);

    }

    destinationPath=tempArgs;

    std::string NameOfLastFileInSourcePath; // removes and saves the file/directory itself from the source path

    currentLetter = sourchPath.at(sourchPath.size()-1);
    while (!sourchPath.empty()&&currentLetter!='/') {
        NameOfLastFileInSourcePath = currentLetter + NameOfLastFileInSourcePath;
        sourchPath=sourchPath.substr(0,sourchPath.size()-1);
        if(!sourchPath.empty())
            currentLetter = sourchPath.at(sourchPath.size()-1);

    }



    std::vector<std::string> toReturn;
    toReturn.push_back(sourchPath);
    toReturn.push_back(NameOfLastFileInSourcePath);
    toReturn.push_back(destinationPath);
    return  toReturn;

}

std::string removeCommand (std::string stringWithCommand) {//remove the first word from the string
    char letter = stringWithCommand.at(0);
    while (!stringWithCommand.empty()&&letter != ' '){
        stringWithCommand = stringWithCommand.substr(1);
        if(!stringWithCommand.empty())
            letter = stringWithCommand.at(0);
    }
    while (!stringWithCommand.empty()&&letter == ' '){
        stringWithCommand=stringWithCommand.substr(1);
        if(!stringWithCommand.empty())
            letter = stringWithCommand.at(0);
    }
    return stringWithCommand;
}

BaseCommand::BaseCommand(std::string args):args(args){ //constructor
}

BaseCommand::~BaseCommand() {}

std::string BaseCommand::getArgs()  {
    return args;

}
Directory* findDirectory (std::string Path ,Directory* Begin) {// get a path of directories.
    // return a pointer to the final directory of the path, or null if it isn't isnt exists
    if(Path=="/")
        return Begin;
    std::string currentPath;
    unsigned int i; //  the numbers of letters in the the next directory/file
    if (Path.substr(0, 2) == "..") {
        if (Begin->getParent() != nullptr)
            return findDirectory(Path.substr(2), Begin->getParent());
        else return nullptr;

    }


    if (Path.length() != 0) {  // get the number of letters of the next directory/file
        if (Path.at(0) == '/') {
            i = 1;
            while (i < Path.length() && ((Path.at(i)) != '/')) {

                i++;
            }
        }
        currentPath = Path.substr(1, i - 1); // the name of the current directory to enter
        if (currentPath == "..") {
            if(Begin->getParent()!= nullptr)
                return findDirectory(Path.substr(i), Begin->getParent());
            else
                return nullptr;
        }
        else {
            for (unsigned int j = 0; j < Begin->getChildren().size(); j++) {
                if (!Begin->getChildren().at(j)->isFile()&&Begin->getChildren().at(j)->getName() == currentPath) {

                    return findDirectory(Path.substr(i), (Directory *) (Begin->getChildren().at(j)));
                }


            }
            return nullptr;

        }
    }
    else
        return Begin;


}
PwdCommand::PwdCommand(std::string args):BaseCommand(args) { //constructor
}

void PwdCommand ::execute(FileSystem & fs){ // print the fs working directory path
    std::cout<<fs.getWorkingDirectory().getAbsolutePath()<<std::endl;


}
std::string PwdCommand :: toString() {
    return "pwd";
}



CdCommand::CdCommand(std::string args):BaseCommand(args) { //constructor
}
void CdCommand ::execute(FileSystem & fs) { //Change the current directory
    std::string args= removeCommand(getArgs());
    Directory* Result= nullptr;
    if (args=="/")
        Result = &fs.getRootDirectory();
    else {
        if (args.empty() || args.at(0) != '/') {
            args = '/' + args;
            Result = findDirectory(args, &fs.getWorkingDirectory());

        } else {
            Result = findDirectory(args, &fs.getRootDirectory());
        }
    }
    if (Result != nullptr)
        fs.setWorkingDirectory(Result);
    else {
        std::cout << "The system cannot find the path specified" << std::endl;

    }


}



std::string CdCommand :: toString() {
    return "cd";
}

LsCommand::LsCommand(std::string args):BaseCommand(args) { //constructor

}
void LsCommand ::execute(FileSystem & fs) {

    std::string toCheck = removeCommand(getArgs());


    bool sortByName = true;
    if (!toCheck.empty()) {
        if (toCheck.substr(0, 2) == "-s") {
            toCheck = removeCommand(toCheck);
            sortByName = false;
        }
    }
    Directory *Result;

    if (toCheck.empty() || toCheck.at(0) != '/') {
        toCheck = '/' + toCheck;
        Result = findDirectory(toCheck, &fs.getWorkingDirectory());
    } else {

        Result = findDirectory(toCheck, &fs.getRootDirectory());

    }
    if (Result == nullptr) {
        std::cout << "The system cannot find the path specified" << std::endl;


    }

    else {
        if (!sortByName)
            (*Result).sortBySize();
        else
            (*Result).sortByName();
        std::vector<BaseFile *>  TempVector = Result->getChildren();

        for (unsigned int i = 0; i < TempVector.size(); i++) {

            if ((*TempVector.at(i)).isFile()) {

                std::cout << "FILE\t";


            }
            else{
                std::cout << "DIR\t";

                 }
            std::cout << TempVector.at(i)->getName() << "\t" << TempVector.at(i)->getSize() << std::endl;

        }
    }
}
std::string LsCommand::toString() {
    return "ls";
}
MkdirCommand::MkdirCommand(std::string args):BaseCommand(args) { //constructor


}

void mkDirInsert(std::string Path,Directory* Begin) {// make new Directory according to path. If needed, create intermediate
    //directories in the path
    if (Path.substr(0, 3) == "/..")
        Path = Path.substr(1);
    if (Path.substr(0, 2) == "..")
        if (Begin->getParent() != nullptr)
            mkDirInsert(Path.substr(0, 2), Begin->getParent());
        else {
            std::cout << "The system cannot find the path specified" << std::endl;


        }
    else {
        unsigned int i;
        bool isFound = false;
        std::string currentPath;
        if (Path.length() != 0) {
            i = 1;
            while (i < Path.length() && Path.at(i) != '/')
                i++;
            currentPath = Path.substr(1, i - 1);
            for (unsigned int j = 0; j < Begin->getChildren().size(); j++) {
                if (Begin->getChildren().at(j)->getName() == currentPath) {

                    if (i == Path.length()) { // checks if it's the last name in the path, and the name exists

                        std::cout << "The directory already exists" << std::endl;
                        isFound = true;
                    } else {
                        if (!Begin->getChildren().at(j)->isFile()) {
                            mkDirInsert(Path.substr(i), (Directory *) Begin->getChildren().at(j));
                            isFound = true;
                        }
                        else {

                            std::cout << "The directory already exists" << std::endl;
                            isFound = true;
                        }

                    }
                }
            }
            if (!isFound) {
                Directory *toAdd = new Directory(currentPath, Begin);
                bool canAddFile = checkName(toAdd->getName(),toAdd->getParent());

                if (canAddFile) {
                    Begin->addFile(toAdd);
                    mkDirInsert(Path.substr(i), toAdd);
                }
                else {
                    delete toAdd;
                }
            }


        }
    }

}
void MkdirCommand ::execute(FileSystem & fs){
    Directory* toStartDir;
    std::string toSend=removeCommand(getArgs());

    if (toSend.empty()||toSend.at(0)!='/') {
        toStartDir = &fs.getWorkingDirectory();
        toSend = '/' + toSend;
    }

    else {
        toStartDir = &fs.getRootDirectory();


    }
    mkDirInsert(toSend,toStartDir);
}

std::string MkdirCommand ::toString(){
    return "mkdir";
}


MkfileCommand:: MkfileCommand(std::string args):BaseCommand(args){
}

void MkfileCommand:: execute(FileSystem & fs) {

    std::string tmpArgs = getArgs();
    char digit = tmpArgs.at(tmpArgs.size() - 1);
    int number = 0;
    int power = 1;
    std::string fileToCreate = "";
    bool fileAlreadyExist = false;

    while (digit <= '9' &&
           digit >= '0') {      // removing the size of the file name from the args, and saving it for later
        number = number + (digit - '0') * power;
        power = power * 10;
        tmpArgs = tmpArgs.substr(0, tmpArgs.size() - 1);
        digit = tmpArgs.at(tmpArgs.size() - 1);
    }
    tmpArgs = removeCommand(tmpArgs); // removing the commannd from the string

    while (!tmpArgs.empty() &&
           tmpArgs.at(tmpArgs.size() - 1) == ' ') // removing spaces between the size of the file to type
        tmpArgs = tmpArgs.substr(0, tmpArgs.size() - 1);
    char c;
   if(!tmpArgs.empty())
        c = tmpArgs.at(tmpArgs.size() - 1);

    while (!tmpArgs.empty() && c != '/') {                        // removing the file name, and save it for later
        fileToCreate = c + fileToCreate;
        tmpArgs = tmpArgs.substr(0, tmpArgs.size() - 1);
        if (!tmpArgs.empty())
            c = tmpArgs.at(tmpArgs.size() - 1);
    }
    Directory *dirToEnter = nullptr;
    if (tmpArgs.empty() || tmpArgs.at(0) != '/') {
        tmpArgs = '/' + tmpArgs;
        dirToEnter = findDirectory(tmpArgs, &fs.getWorkingDirectory());

    } else {

        dirToEnter = findDirectory(tmpArgs, &fs.getRootDirectory());
    }
    if (dirToEnter == nullptr) {
        std::cout << "The system cannot find the path specified" << std::endl;
    } else {
        for (unsigned int i = 0; !(fileAlreadyExist) &&i < dirToEnter->getChildren().size(); i++) {
            if (dirToEnter->getChildren().at(i)->getName() == fileToCreate ) {
                std::cout << "File already exists" << std::endl;
                fileAlreadyExist = true;
            }
        }
        if (!(fileAlreadyExist)) {
            bool canAddFile = checkName(fileToCreate,dirToEnter);
            File * newFile =  new File(fileToCreate, number);
            if (canAddFile) {
                dirToEnter->addFile(newFile);
            }
            else delete newFile;
        }

    }

}

std::string MkfileCommand::toString(){
    return "mkfile";
}

CpCommand:: CpCommand (std::string args): BaseCommand(args){}



void CpCommand:: execute(FileSystem & fs){
    std::vector <std::string> splitedArgs = splitArgs(getArgs());

    std::string sourchPath = (splitedArgs.at(0));
    std::string destinationPath = (splitedArgs.at(2));
    bool fileExist = false;
    BaseFile * toCopy; // the file to copy
    std::string NameOfLastFileInSourcePath = (splitedArgs.at(1));; // removes and saves the file/directory itself from the source path
    Directory *  directoryOfLastFile=nullptr;
    if(sourchPath.empty()||sourchPath.at(0)!='/')
    {
        sourchPath='/'+sourchPath;
        directoryOfLastFile = findDirectory(sourchPath, &fs.getWorkingDirectory());
    }
    else
        directoryOfLastFile = findDirectory(sourchPath, &fs.getRootDirectory()); // get the adress of the directory that contains the file/directory that should be copied

    if (directoryOfLastFile == nullptr) {

        std::cout << "No such file or directory" << std::endl;
    }
    else {
        for (unsigned int i=0; i<directoryOfLastFile->getChildren().size() && (!fileExist); i++){
            if (directoryOfLastFile->getChildren().at(i)->getName()== NameOfLastFileInSourcePath) {
                fileExist = true;
                toCopy = directoryOfLastFile->getChildren().at(i); //
            }
        }
        if (!(fileExist)){

            std::cout << "No such file or directory" << std::endl;

        }
        else {
            copyFileToDestination(toCopy, destinationPath, fs);
        }

    }
}


void copyFileToDestination(BaseFile * toCopy, std::string destinationPath, FileSystem & fs) {  // get base file and copy to destination if possible
    bool isOkeyFile=true;
    if (!destinationPath.length() == 0) {
        Directory *destination = nullptr;
        if (destinationPath.at(0) != '/'){
            destinationPath='/'+destinationPath;
            destination = findDirectory(destinationPath, &fs.getWorkingDirectory());
        }
        else
            destination = findDirectory(destinationPath, &fs.getRootDirectory());

        if (destination == nullptr) {

            std::cout << "No such file or directory" << std::endl;
        }
        else {

            for ( unsigned i=0; i < destination->getChildren().size() && isOkeyFile; i++)
                if (destination->getChildren().at(i)->getName() == toCopy->getName())
                    isOkeyFile = false;

            if (isOkeyFile) {
                if (toCopy->isFile()) {
                    File *toAdd = new File((*(File *) toCopy));
                    if (checkName(toAdd->getName(),destination) )
                        destination->addFile(toAdd);
                    else
                        delete toAdd;

                } else {
                    Directory *toAdd = new Directory((*(Directory *) toCopy));
                    if (checkName(toAdd->getName(),destination) )
                        destination->addFile(toAdd);
                    else
                        delete toAdd;
                }
            }
        }
    }
}

std::string CpCommand::toString(){
    return "cp";

}


MvCommand::MvCommand(std::string args):BaseCommand(args) {}


void MvCommand::execute(FileSystem & fs) {
    ////  spliting the args the sourch path and destination path, and the name of file to move
    std::vector<std::string> splitedArgs = splitArgs(getArgs());
    std::string sourchPath = (splitedArgs.at(0));
    std::string destinationPath = (splitedArgs.at(2));
    bool fileExist = false;
    BaseFile *toCopy = nullptr; // the file to copy
    std::string NameOfLastFileInSourcePath = (splitedArgs.at(1));



    // removes and saves the file/directory itself from the source path
    Directory *directoryOfLastFile = nullptr;

    if (sourchPath.empty() || sourchPath.at(0) != '/') {
        sourchPath = '/' + sourchPath;
        directoryOfLastFile = findDirectory(sourchPath,
                                            &fs.getWorkingDirectory()); // get the adress of the directory that contains the file/directory that should be copied
    } else
        directoryOfLastFile = findDirectory(sourchPath, &fs.getRootDirectory());
    if (directoryOfLastFile == nullptr) {

        std::cout << "No such file or directory" << std::endl;
    }
    else {
        if (NameOfLastFileInSourcePath == "..") { //when we should move the father's directory
            toCopy = directoryOfLastFile->getParent();
            if (toCopy == nullptr) {

                std::cout << "No such file or directory" << std::endl;
            }
            else {
                if (((Directory *) (toCopy))->getParent() == nullptr) {

                    std::cout << "Can't move directory" << std::endl;
                }
                else {
                    directoryOfLastFile = ((Directory *) (toCopy))->getParent();
                    MoveFileToDestination(directoryOfLastFile, toCopy, destinationPath, fs);
                }
            }
        } else {
            unsigned int i = 0;

            while (i < directoryOfLastFile->getChildren().size() && (!fileExist)) {
                if (directoryOfLastFile->getChildren().at(i)->getName() == NameOfLastFileInSourcePath) {
                    fileExist = true;
                    toCopy = directoryOfLastFile->getChildren().at(i);

                } else
                    i++;
            }


            if (!(fileExist)) {

                std::cout << "No such file or directory" << std::endl;
            }
            else
                MoveFileToDestination(directoryOfLastFile, toCopy, destinationPath, fs); // handle the destination path and the moving itself

        }
    }
}


void MoveFileToDestination(Directory* directoryOfLastFile  ,BaseFile * toCopy, std::string destinationPath, FileSystem & fs) {// move to copy to destination if possible

    unsigned int i=0;
    bool isOkeyFile = true;
    if (!(toCopy->isFile())) {
        Directory *runner = &fs.getWorkingDirectory();
        while (runner->getParent() != nullptr && isOkeyFile) { // cheking the directory to move isn't the
            // working directory or one of its parents
            if (runner == toCopy)
                isOkeyFile = false;
            else
                runner = runner->getParent();
        }

    }

    if (!(isOkeyFile)) {

        std::cout << "Can't move directory" << std::endl;
    }
    else {
        Directory *destination = nullptr;
        if (destinationPath.at(0) != '/') { // differing between the cases of starting from root or working directory
            destinationPath = '/' + destinationPath;
            destination = findDirectory(destinationPath, &fs.getWorkingDirectory());
        } else
            destination = findDirectory(destinationPath, &fs.getRootDirectory());
        if (destination == nullptr) {

            std::cout << "No such file or directory" << std::endl;
        }
        else {
            for ( i=0; i < destination->getChildren().size() && isOkeyFile; i++) {
                if (destination->getChildren().at(i)->getName() == toCopy->getName())
                    isOkeyFile = false;
            }
            if (!isOkeyFile) {
                std::cout << "Can't move directory" << std::endl;
            }
            else {
                if (toCopy->isFile()) {

                    File *toAdd = new File((*(File *) toCopy));
                    if (checkName(toAdd->getName(),destination) )
                        destination->addFile(toAdd);
                    else
                        delete toAdd;


                } else {

                    Directory *toAdd = new Directory((*(Directory *) toCopy));
                    if (checkName(toAdd->getName(),destination) )
                        destination->addFile(toAdd);
                    else
                        delete toAdd;

                }

                directoryOfLastFile->removeFile(toCopy);




            }
        }
    }
}

std::string MvCommand::toString(){
    return "mv";
}


RenameCommand::RenameCommand(std::string args):BaseCommand(args){

}

void RenameCommand::execute(FileSystem & fs){
    std::string currentPath=removeCommand(getArgs());
    std::string newName="";
    std::string oldName="";
    std::string sourcePath="";
    while(currentPath.at(0)!=' '){ // get the path of the file to rename, currentPath won't include the source path
        sourcePath=sourcePath+currentPath.at(0);
        currentPath=currentPath.substr(1);
    }



    while(!sourcePath.empty()&&sourcePath.at(sourcePath.length()-1)!='/'){ // get the old name of the file from the sorce path
        oldName=sourcePath.at(sourcePath.length()-1)+oldName;
        sourcePath=sourcePath.substr(0,sourcePath.length()-1);
    }

    while(currentPath.at(0)==' '){ // remove spaces before the new name
        currentPath=currentPath.substr(1);
    }

    newName=currentPath;
    Directory* sourceDir= nullptr;
    if(sourcePath.empty()||sourcePath.at(0)!='/'){
        sourcePath='/'+sourcePath;


        sourceDir = findDirectory(sourcePath, &fs.getWorkingDirectory());
    }
    else
        sourceDir = findDirectory(sourcePath, &fs.getRootDirectory());
    if (sourceDir == nullptr) {
        std::cout << "No such file or directory" << std::endl;

    }
    else{
        unsigned int i=0;
        bool found=false;
        while(i<sourceDir->getChildren().size()&&!found){
            if(sourceDir->getChildren().at(i)->getName()==oldName)
                found=true;
            else
                i++;
        }
        if(!found) {

            std::cout << "No such file or directory" << std::endl;
        }
        else{
            if(sourceDir->getChildren().at(i)==&fs.getWorkingDirectory()) {

                std::cout << "Can't rename the working directory" << std::endl;
            }
            else{
                if(checkName(newName,sourceDir))
                    sourceDir->getChildren().at(i)->setName(newName);
            }
        }
    }

}
std::string RenameCommand::toString(){
    return "rename";
}

RmCommand::RmCommand(std::string args):BaseCommand(args) {}

void RmCommand::execute(FileSystem &fs) {//execture rm

    std::string sourcePath = removeCommand(getArgs());
    std::string nameToRemove = "";
    while (!sourcePath.empty() && sourcePath.at(sourcePath.length() - 1) != '/') {
        nameToRemove = sourcePath.at(sourcePath.length() - 1) + nameToRemove;
        sourcePath = sourcePath.substr(0, sourcePath.length() - 1);
    }
    Directory *sourceDir= nullptr;
    if (sourcePath.empty()||sourcePath.at(0)!='/') {
        sourcePath='/'+sourcePath;
        sourceDir = findDirectory(sourcePath,&fs.getWorkingDirectory());

    }
    else

        sourceDir = findDirectory(sourcePath, &fs.getRootDirectory());
    if (sourceDir == nullptr) {

        std::cout << "No such file or directory" << std::endl;
    }

    else {
        bool found = false;
        unsigned int i = 0;
        while (i < sourceDir->getChildren().size() && !found) {
            if (sourceDir->getChildren().at(i)->getName() == nameToRemove)
                found = true;
            else
                i++;
        }

            if (( (found) && sourceDir->getChildren().at(i) == &fs.getWorkingDirectory() ) ||
                nameToRemove=="") {
                std::cout << "Can't remove directory" << std::endl;
            }
            else {
                if (!found) {
                    std::cout << "No such file or directory" << std::endl;
                }
                else {
                    BaseFile *temp = sourceDir->getChildren().at(i);
                    sourceDir->removeFile(temp);
                }

            }

        }

    }






std::string RmCommand::toString(){
    return "rm";
}


HistoryCommand:: HistoryCommand(std::string args, const std::vector<BaseCommand *> & history):BaseCommand(args),
                                                                                              history(history){}

void HistoryCommand::execute(FileSystem & fs){//execute history

    for (unsigned int i=0; i<history.size(); i++)
        std::cout << i << "\t" << history.at(i)->getArgs() << std::endl;
    if (history.empty())
        std::cout << "" << std::endl;
}


std::string HistoryCommand:: toString(){
    return "history";

}
ErrorCommand::ErrorCommand(std::string args):BaseCommand(args){}

void ErrorCommand::execute(FileSystem & fs){//  execute -error

    std::string unknownCoammnd="";
    std::string args = getArgs();
    char letter = args.at(0);
    while (letter != ' ' && (!(args.empty())) ){ //get the name of the command
        unknownCoammnd = unknownCoammnd + letter;
        args = args.substr(1);
        if (!args.empty())
            letter = args.at(0);
    }
    std::cout <<unknownCoammnd +": Unknown command" << std::endl;
}
std::string ErrorCommand::toString(){
    return "error";
}


ExecCommand:: ExecCommand(std::string args, const std::vector<BaseCommand *> & history):BaseCommand(args),
                                                                                        history(history){}
void ExecCommand::execute(FileSystem & fs){//execute exec
    std::string args = removeCommand(getArgs());
    int power=1;
    unsigned int number = 0;
    char digit = args.at(args.size()-1);
    while (!(args.empty())) {      // translating the number in the string to number
        number = number + ((digit - '0') * power);
        power = power * 10;
        args = args.substr(0, args.size() - 1);
        if (!args.empty())
            digit = args.at(args.size() - 1);
    }
    if (history.size()<=number){
        std::cout << "Command not found" << std::endl;
    }
    else {

        history.at(number)->execute(fs);

    }
}

std::string ExecCommand::toString(){
    return "exec";
}

VerboseCommand:: VerboseCommand(std::string args):BaseCommand(args){}//constructor

void VerboseCommand::execute(FileSystem & fs) { //execute-vebose

    std::string currentPath=removeCommand(getArgs());
    if(currentPath!="0"&&currentPath!="1"&&currentPath!="2"&&currentPath!="3") {
        std::cout << "Wrong verbose input" << std::endl;
    }
    else{
        verbose=currentPath.at(0)-'0';
    }
}

std::string VerboseCommand:: toString(){
    return "verbose";
}
PwdCommand& PwdCommand::getNew(){
    return *(new PwdCommand(*this));
}
CdCommand& CdCommand::getNew(){
    return *(new CdCommand(*this));
}
LsCommand& LsCommand::getNew(){
    return *(new LsCommand(*this));
}
MkdirCommand& MkdirCommand::getNew(){
    return *(new MkdirCommand(*this));
}
MkfileCommand& MkfileCommand::getNew(){
    return *(new MkfileCommand(*this));
}
CpCommand& CpCommand::getNew(){
    return *(new CpCommand(*this));
}
MvCommand& MvCommand::getNew(){
    return *(new MvCommand(*this));
}
RmCommand& RmCommand::getNew(){
    return *(new RmCommand(*this));
}
RenameCommand& RenameCommand::getNew(){
    return *(new RenameCommand(*this));
}
HistoryCommand& HistoryCommand::getNew(){
    return *(new HistoryCommand(*this));
}
VerboseCommand& VerboseCommand::getNew(){
    return *(new VerboseCommand(*this));
}
ErrorCommand& ErrorCommand::getNew(){
    return *(new ErrorCommand(*this));
}
ExecCommand& ExecCommand::getNew(){
    return *(new ExecCommand(*this));
}