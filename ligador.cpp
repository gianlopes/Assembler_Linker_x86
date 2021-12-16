#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <fstream>
#include <map>
#include "stringUtils.hpp"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Pelo menos um arquivo .asm precisa ser fornecido pela linha de comando.\n";
        return 1;
    }

    std::string s;
    std::vector<std::vector<std::string>> modulos(argc-1); // Um vetor para cada módulo
    std::ifstream inputFile{};
    for (int i = 1; i < argc ; i++) {// Guardando as linhas de cada módulo
        inputFile.open(argv[i]);
        if(!inputFile.is_open()){
            std::cout << "Falha ao abrir o arquivo de entrada" << std::endl;
            exit(1);
        }
        while (std::getline(inputFile, s))
            modulos[i-1].push_back(s);
        inputFile.close();
    }

    // Salvando o fator de Correção
    std::map<std::string, int> fatorCorrecao{};
    int contador = 0;
    for(auto &mod : modulos)
    {
        s = mod[0].substr(3, mod[0].size()); // Pega o nome do módulo
        std::cout << s << "\n";
        fatorCorrecao[s] = contador; // Salva o endereço inicial
        contador += stoi(mod[1].substr(3, mod[1].size())); // Soma o tamanho do programa ao contador
    }

    // Preencheno a tabela global de definições
    std::map<std::string, int> tabelaGlobalDefinicoes{};
    std::vector<std::string> tokens{};
    for (auto &mod : modulos){
        s = mod[0].substr(3, mod[0].size()); // Pega o nome do módulo
        for (auto l : mod){
            if (l[0] == 'D') 
            {
                tokens = tokenize(l.substr(3, l.size()), " "); // Exclui o D: e faz split()
                tabelaGlobalDefinicoes[tokens[0]] = 
                    stoi(tokens[1]) 
                    + fatorCorrecao[s]; // Salva na tabela
            }
        }
    }

    // Separando as seções de texto e de informaçãode realocação
    std::vector<std::string> mem;

    for (auto &mod : modulos){
        s = mod[0].substr(3, mod[0].size()); // Pega o nome do módulo
        auto insts = tokenize(mod[3].substr(3, mod[3].size()), " "); // Exclui o T: e faz split()
        auto rs = mod[2].substr(3, mod[2].size()); // Exclui o R:
        // Utilizando infomação de realocação
        for (size_t i = 0; i < rs.size(); i++)
        {
            if(rs[i] == '1') // Se for relativo, adiciona o fator de correção
                insts[i] = std::to_string(fatorCorrecao[s] + stoi(insts[i])); // Atualiza as variáveis relativas com o FC
        }

        mem.insert(mem.end(), insts.begin(), insts.end()); // Junta com os outros módulos
    }

    // Utilizando a tabela global de definições, tabela de uso e fator de correção para atualizar endereços
    for (auto &mod : modulos){
        s = mod[0].substr(3, mod[0].size()); // Pega o nome do módulo
        for (auto l : mod){
            if (l[0] == 'U') 
            {
                tokens = tokenize(l.substr(3, l.size()), " "); // Exclui o U: e faz split()
            // Utiliza tokens[1] (Posição da memória) com o FC para atualizar com o valor na TGD
            mem[stoi(tokens[1]) + fatorCorrecao[s]] = std::to_string(tabelaGlobalDefinicoes[tokens[0]]);
            }
        }
    }

    // Abrindo arquivo de saída
    std::ofstream outputFile{};
    std::string outPath {argv[1]};
    // Aquivo ligado terá um "Ligado" antes do .obj
    outPath.insert(outPath.size()-4, "Ligado"); 
    outputFile.open(outPath);
    if(!outputFile.is_open()){
        std::cout << "Falha ao abrir o arquivo de saída" << std::endl;
        exit(1);
    }

    for (auto x : mem){
        outputFile << x << " ";
    }

    outputFile.close();

    std::cout << "Programa ligado com sucesso: " << outPath << std::endl;
    return 0;
}

