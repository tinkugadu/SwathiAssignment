#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <map>
using namespace std;

#define PWD_FILE "/passwds.pwd"

enum AuthResponse
{
    ERROR,
    UNKNOWN_USER,
    WRONG_PASSWORD,
    SUCCESS
};

bool GetUsersFromFile(std::map<std::string, std::string> &userMap)
{
    char *path = getenv("HOME");
    std::string fileName = PWD_FILE;
    fileName = path + fileName;
    std::fstream pwd_file(fileName.c_str());
    if(!pwd_file.is_open())
    {
        perror("Error opening password file");
        return false;
    }
    std::string line;
    while(getline(pwd_file, line))
    {
        int ind = line.find('\t');
        if(ind == 0 || ind == EOF)
        {
            continue;
        }
        userMap.insert(std::pair<std::string, std::string>(line.substr(0, ind),
                                    line.substr(ind+1, line.length()-1-ind)));
    }
    pwd_file.close();
    if(userMap.size() == 0)
    {
        return false;
    }
    return true;
}

//creates a default password file if none exists
void CreatePasswordFile()
{
    char *path = getenv("HOME");
    std::string fileName = PWD_FILE;
    fileName = path + fileName;
    std::ofstream pwd_file(fileName.c_str());
    if(!pwd_file.is_open())
    {
        perror("Error opening password file");
        return;
    }
    pwd_file << "hercule\tpasswd1\n";
    pwd_file << "zeus\tpasswd2\n";
    pwd_file << "thor\tpasswd3\n";
    pwd_file.close();
}

//checks if the password file exists
bool IsPasswordFileExist()
{
    bool retVal = false;
    char *path = getenv("HOME");
    std::string fileName = PWD_FILE;
    fileName = path + fileName;
    std::ifstream pwdFile;
    pwdFile.open(fileName.c_str());
    if(pwdFile.is_open())
    {
        retVal = true;
    }
    pwdFile.close();
    return retVal;
}

void UpdatePasswordFile(std::map<std::string, std::string> userMap)
{
    std::map<std::string, std::string>::iterator it;
    //open password file
    char *path = getenv("HOME");
    std::string fileName = PWD_FILE;
    fileName = path + fileName;
    std::ofstream pwd_file(fileName.c_str());
    if(!pwd_file.is_open())
    {
        perror("Error opening password file");
        return;
    }
    for(it = userMap.begin(); it!=userMap.end(); ++it)
    {
        pwd_file << it->first<<"\t"<<it->second<<"\n";
    }
    pwd_file.close();
}

AuthResponse ChangePassword(std::string userName, std::string passwd)
{
    std::map<std::string, std::string> myMap;
    if(!GetUsersFromFile(myMap))
    {
        return ERROR;
    }
    std::map<std::string, std::string>::iterator it = myMap.find(userName);
    if(it == myMap.end())
    {
        return UNKNOWN_USER;
    }
    myMap[userName] = passwd;
    UpdatePasswordFile(myMap);
    return SUCCESS;
}


#endif //UTILS_H
