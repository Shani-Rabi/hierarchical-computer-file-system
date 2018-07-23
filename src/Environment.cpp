#include "../include/Environment.h"
#include "../include/GlobalVariables.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

Environment::Environment():commandsHistory(),fs(){//constructor

}

BaseCommand* Environment::findCommand(std::string input){// get input and retrun basecommand according to the input
    std::string currentPath=input;
    unsigned int i=0;
    while(i<input.length()&&input.at(i)!=' '){
        i++;

    }
    currentPath=currentPath.substr(0,i);
    if(currentPath=="pwd")
        return new PwdCommand(input);
    if(currentPath=="cd")
        return new CdCommand(input);
    if(currentPath=="ls")
        return new LsCommand(input);
    if(currentPath=="mv")
        return new MvCommand(input);
    if(currentPath=="mkdir")
        return new MkdirCommand(input);
    if(currentPath=="mkfile")
        return new MkfileCommand(input);
    if(currentPath=="cp")
        return new CpCommand(input);
    if(currentPath=="rename")
        return new RenameCommand(input);
    if(currentPath=="rm")
        return new RmCommand(input);
    if(currentPath=="history")
        return new HistoryCommand(input,this->commandsHistory);
    if(currentPath=="verbose")
        return new  VerboseCommand(input);
    if(currentPath=="exec")
        return new ExecCommand(input,this->commandsHistory);
    return new ErrorCommand(input);




}

void Environment:: start(){ //start the environment
    std::cout <<fs.getWorkingDirectory().getName()<< ">";
    std:: string args = "";
    std::getline(std::cin, args); //get the command from the user
    while (args!="exit"){
        if (args!="") {
            BaseCommand *temp = this->findCommand(args);
            if(verbose==2||verbose==3)
                std::cout<<args<<std::endl;
            temp->execute(fs);

            commandsHistory.push_back(temp);
        }
        std::cout<<fs.getWorkingDirectory().getAbsolutePath() + ">";

        std::getline(std::cin, args);
    }

}
Environment::Environment(Environment& other):commandsHistory(),fs(){//copy constructor
    if(verbose==1||verbose==3)
        std::cout<<"Environment::Environment(Environment& other)"<<std::endl;
    fs=other.fs;
    for(unsigned int i=0;i<commandsHistory.size();i++)
    {
        commandsHistory.push_back(&other.commandsHistory.at(i)->getNew());
    }

}
Environment::Environment(Environment&& other):commandsHistory(),fs(){ //move constructor
    if(verbose==1||verbose==3)
        std::cout<<"Environment::Environment(Environment&& other)"<<std::endl;
    fs=other.fs;
    for(unsigned int i=0;i<commandsHistory.size();i++)
    {
        commandsHistory.push_back(&other.commandsHistory.at(i)->getNew());
    }
    delete &other;
}
Environment::~Environment(){ //distructor
    if(verbose==1||verbose==3)
        std::cout<<"Environment::~Environment()"<<std::endl;
    clear();
}
void Environment::clear(){ //clear the fields of the environment
    for(unsigned int i=0;i<commandsHistory.size();i++)
        delete(commandsHistory.at(i));
}
Environment& Environment::operator=(Environment& other){ //copy operator
    if(verbose==1||verbose==3)
        std::cout<<"Environment& operator=(Environment& other)"<<std::endl;
    if(this!=&other){
        clear();
        fs=other.fs;
        for(unsigned int i=0;i<commandsHistory.size();i++)
        {
            commandsHistory.push_back(&other.commandsHistory.at(i)->getNew());
        }

    }
    return *this;
}
Environment& Environment:: operator=(Environment&& other){ //move operator
    if(verbose==1||verbose==3)
        std::cout<<"Environment& operator=(Environment&& other)"<<std::endl;
    if(this!=&other){
        clear();
        fs=other.fs;
        for(unsigned int i=0;i<commandsHistory.size();i++)
        {
            commandsHistory.push_back(&other.commandsHistory.at(i)->getNew());
        }
        delete &other;
    }
    return *this;
}

FileSystem& Environment::getFileSystem() {// Get a reference to the file system
    return fs;
}
void Environment:: addToHistory(BaseCommand *command){ //add command to history
    commandsHistory.push_back(command);

} // Add a new command to the history


const  vector<BaseCommand*>& Environment::getHistory() const{ //return history vector
    const std::vector <BaseCommand *> & temp= commandsHistory;
    return temp;
} // Return a reference to the history of commands
