// Amirmohammad Khosravi Esfezar
// S.N. : 810198386

#include <signal.h>
#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <map>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 2048 
const string TEST_FILES_PATH = "testcases";
const string TEST_FILE_FORMAT = ".csv";

int main(int argc, char *argv[])
{
    int toReadFd = stoi(argv[1]);
    char readMsg[BUFFER_SIZE] = {0};
    read(toReadFd, readMsg, sizeof(BUFFER_SIZE));
    string csvFilePath;
    stringstream strs;
    strs << TEST_FILES_PATH << '/' << readMsg << TEST_FILE_FORMAT << endl;
    strs >> csvFilePath;
    fstream fs(csvFilePath);
    map<string, int> wordCounts;
    string line, word;
    fs >> line;
    stringstream ss;
    ss << line;

    while (getline(ss, word, ','))
    {
        if (word == "" || word.length() == 0)
            continue;
        if (wordCounts.find(word) != wordCounts.end())
            wordCounts[word] += 1;
        else
            wordCounts[word] = 1;
    }
    stringstream ssWordCounts;
    for (const auto& x : wordCounts)
        ssWordCounts << x.first << " " << x.second << endl;

    string toSend = ssWordCounts.str();
    char buff[BUFFER_SIZE];
    for (int i = 0; i < toSend.length(); i++)
        buff[i] = toSend[i];
    buff[toSend.length()] = '\0';
        
    mkfifo(readMsg, 0666);
    int fdReduce = open(readMsg, O_WRONLY);
    write(fdReduce, buff, BUFFER_SIZE);
    close(fdReduce);
   
    return 0;
}