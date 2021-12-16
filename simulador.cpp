/* Gianlucas Dos Santos Lopes - 180041991 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int main(int, char *argv[])
{
    /* File IO */
    std::string path{argv[1]};
    std::ifstream inputFile{path};
    if(!inputFile.is_open()){
        std::cout << "Falha ao abrir o arquivo de entrada" << std::endl;
        exit(1);
    }
    std::ofstream outputFile{path.replace(path.end() - 4, path.end(), ".out")};
    if(!outputFile.is_open()){
        std::cout << "Falha ao abrir o arquivo de saída" << std::endl;
        exit(1);
    }

    std::vector<int> mem{};
    int data;

    while (inputFile >> data)
    {
        mem.push_back(data);
    }
    inputFile.close();

    int ACC = 0, PC = 0;
    int output;
    bool outputFlag = false;

    while (1)
    {
        switch (mem[PC])
        {
        case 1: // add
            ACC += mem[mem[PC + 1]];
            PC += 2;
            break;
        case 2: // sub
            ACC -= mem[mem[PC + 1]];
            PC += 2;
            break;
        case 3: // mul
            ACC *= mem[mem[PC + 1]];
            PC += 2;
            break;
        case 4: // div
            ACC /= mem[mem[PC + 1]];
            PC += 2;
            break;
        case 5: // jmp
            PC = mem[PC + 1];
            break;
        case 6: // jmpn
            if (ACC < 0)
                PC = mem[PC + 1];
            else
                PC += 2;
            break;
        case 7: // jmpp
            if (ACC > 0)
                PC = mem[PC + 1];
            else
                PC += 2;
            break;
        case 8: // jmpz
            if (ACC == 0)
                PC = mem[PC + 1];
            else
                PC += 2;
            break;
        case 9: // copy
            mem[mem[PC + 2]] = mem[mem[PC + 1]];
            PC += 3;
            break;
        case 10: // load
            ACC = mem[mem[PC + 1]];
            PC += 2;
            break;
        case 11: // store
            mem[mem[PC + 1]] = ACC;
            PC += 2;
            break;
        case 12: // input
            std::cin >> mem[mem[PC + 1]];
            PC += 2;
            break;
        case 13: // output
            output = mem[mem[PC + 1]];
            outputFlag = true;
            PC += 2;
            break;
        case 14: // stop
            PC = -999;
            break;
        default:
            break;
        }

        if (PC == -999)
            break;

        std::cout << "\nPC <- " << PC << "\nACC <- " << ACC << '\n';
        if(outputFlag)
        {
            std::cout << "Output: "<< output << '\n';
            outputFile << output << '\n';
            outputFlag = false;
        }
    }

    return 0;
}
