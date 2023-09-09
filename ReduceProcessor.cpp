// Amirmohammad Khosravi Esfezar
// S.N. : 810198386

#include <signal.h>
#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <map>
#include <sstream>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 2048

int main(int argc, char *argv[])
{
    int toWriteFd = stoi(argv[1]);
    int fileCounts = stoi(argv[2]);
    char readMsg[BUFFER_SIZE] = {0};
    map<string, int> wordCounts;

    for (int i = 1; i < fileCounts + 1; i++)
    {
        int fdReduce = open(to_string(i).c_str(), O_RDONLY);
        char buff[BUFFER_SIZE];

        read(fdReduce, buff, BUFFER_SIZE);
        stringstream ss;
        ss << buff;
        string pair;

        while (getline(ss, pair))
        {
            string key;
            int value;
            stringstream ssTemp;
            ssTemp << pair;
            ssTemp >> key;
            ssTemp >> value;
            if (wordCounts.find(key) != wordCounts.end())
                wordCounts[key] += value;
            else
                wordCounts[key] = value;
        }
        close(fdReduce);
    }

    stringstream ssWordCounts;
    for (const auto& x : wordCounts)
        ssWordCounts << x.first << ": " << x.second << endl;

    string toSend = ssWordCounts.str();

    char buff[BUFFER_SIZE];
    for (int i = 0; i < toSend.length(); i++)
        buff[i] = toSend[i];
    buff[toSend.length()] = '\0';
    write(toWriteFd, buff, BUFFER_SIZE);
    close(toWriteFd);
    
    return 0;
}
