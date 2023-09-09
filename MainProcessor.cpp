// Amirmohammad Khosravi Esfezar
// S.N. : 810198386

#include <signal.h>
#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <filesystem>
#include <stdio.h>
#include <sstream>

using namespace std;

#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 2048 
const string TEST_FILES_PATH = "testcases";
const string MAP_PROCESS = "MapProcess";
const string REDUCE_PROCESS = "ReduceProcess";

int countTestFiles(string filePath)
{
    int count{};
    filesystem::path p1{filePath};
    for (auto& p : std::filesystem::directory_iterator(p1))
      ++count;
   return count;
}

void makeMapProcesses(int fileCounts, int pipeFds[][2])
{
    for (int i = 0; i < fileCounts; i++)
    {
        pid_t pid;
        pid = fork();
        if (pid < 0)
        {
            cout << "ERR --> Fork failed" << endl;
            exit(1);
        }
        if (pid == 0)
        {
            // stringstream ssFileName;
            // ssFileName << TEST_FILES_PATH << '/' << i + 1 << ".csv";
            // string fileName = ssFileName.str();
            // char buff[BUFFER_SIZE];
            // for (int i = 0; i < fileName.length(); i++)
            //     buff[i] = fileName[i];
            // buff[fileName.length()] = '\0';
            // we could pass buff as second argument below

            write(pipeFds[i][WRITE_END], to_string(i + 1).c_str(), BUFFER_SIZE);
            char* argvs[] = { (char*)MAP_PROCESS.c_str(), (char*)to_string(pipeFds[i][READ_END]).c_str(), NULL };
            execv(argvs[0], argvs);     
        }
        else
            close(pipeFds[i][READ_END]);
    }
}

void makeReduceProcess(int fileCounts, int pipeFds[][2])
{
    pid_t pid;
    pid = fork();
    if (pid == -1)
    {
        cout << "ERR --> Fork failed" << endl;
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // close(pipeFds[fileCounts][READ_END]);
        char temp[5];
        for (int i = 0; i < to_string(pipeFds[fileCounts][WRITE_END]).length(); i++)
            temp[i] = to_string(pipeFds[fileCounts][WRITE_END])[i];
        temp[to_string(pipeFds[fileCounts][WRITE_END]).length()] = '\0';
        char *argv_list[] = {(char*)REDUCE_PROCESS.c_str(),
            (char *)temp,
            (char *)to_string(fileCounts).c_str(), NULL};
        execv(argv_list[0], argv_list);
    }
    // else
    //     close(pipeFds[fileCounts][WRITE_END]);
}

int main(int argc, char *argv[])
{
    int fileCounts = countTestFiles(TEST_FILES_PATH);
  
    int pipeFds[fileCounts][2];
    for (int i = 0; i < fileCounts + 1; i++)
        if (pipe (pipeFds[i]) == -1)
        {
            cout << "ERR --> Pipe failed" << endl;
            exit(1);
        }
    
    makeMapProcesses(fileCounts, pipeFds);
    makeReduceProcess(fileCounts, pipeFds);

    char answer[BUFFER_SIZE];
    read(pipeFds[fileCounts][READ_END], answer, BUFFER_SIZE);

    stringstream ssAnswer;
    ssAnswer << answer;
    cout << answer;
    
    for (int i = 0; i < fileCounts + 1; i++)
    {
        close(pipeFds[i][WRITE_END]);
        close(pipeFds[i][READ_END]);
    }

    fstream fs("output.csv");
    fs << answer;
    fs.close();

    return 0;
}