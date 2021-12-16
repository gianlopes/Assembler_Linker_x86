/* Gianlucas Dos Santos Lopes - 180041991 */

#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <algorithm> // std::transform
#include <regex>
#include <vector>
#include <map>
#include <tuple>
#include "stringUtils.hpp"
#include "montador.hpp"
#include "trataErros.hpp"

void montador(std::string path, int nModulos);

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        std::cout << "Pelo menos um arquivo .asm precisa ser fornecido pela linha de comando.\n";
    }
    else
    {
        for(int i = 1; i < argc ; i++) // O montador será chamado uma vez para cada argumento passado
            montador(argv[i], argc-1);
    }
}

void montador(std::string path, int nModulos) {
    /* File IO */
    std::ifstream inputFile{};
    inputFile.open(path);
    if(!inputFile.is_open()){
        std::cout << "Falha ao abrir o arquivo de entrada" << std::endl;
        exit(1);
    }
    std::ofstream outputFile{};
    outputFile.open(path.replace(path.end() - 4, path.end(), ".obj"));
    if(!outputFile.is_open()){
        std::cout << "Falha ao abrir o arquivo de saída" << std::endl;
        exit(1);
    }

    /* Lista para armazenar linhas do arquivo*/
    std::list<tipoLinha> lista{};
    std::list<tipoLinha> listaData{};

    /* Aux */
    tipoLinha linha;
    int flagBegin = 0;
    tipoLinha linhaBegin;
    tipoLinha linhaEnd;
    std::string s;
    std::smatch m;
    std::vector<std::string> tokens;

    TrataErros::start();
    int contador_linha = 0;

    /*
    Passagem 0
    Salva as linhas do arquivo asm em uma lista de linhas, onde as linhas da seção de dados vem depois
    da seção de texto. Cada linha contém um rótulo, uma instrução, um campo de operandos (separados por vígulas
    sem espaços) e um número da linha original no arquivo. Os campos podem ser vazios "".
    */
    while (std::getline(inputFile, s)) // Lê uma linha do arquivo
    {
        contador_linha++;
        s = std::regex_replace(s, std::regex{";.*"}, "");     // Desconsiderando comentários
        s = std::regex_replace(s, std::regex{"[\\s]+"}, " "); // Desconsiderando espaços desnecéssarios
        trim(s);                                              // Corta espaços no começo o no final da linha
        s = strToLower(s);

        if (s == "section data")
            continue; // Assume que a sessão de dados sempre vem primeiro
        if (s == "section text")
        {
            listaData = std::move(lista); // Move a sessão de dados para outra lista
            continue;
        }
        if (s == " " || s == "") // Se a linha estiver vazia, é desconsiderada.
            continue;

        if (std::regex_search(s, m, std::regex{"^.*(?=:)"})) // Caso exista rótulo
        {
            linha.rotulo = m.str();           // Salva o rótulo
            trim(linha.rotulo);               // Guardar o rótulo sem espaços
            s.erase(0, m.str().length() + 1); // Apaga rótulo da string original
            trim(s);

            if (s == " " || s == "") // Caso o rótulo esteja sozinho na linha
            {
                linha.operacao = "";
                linha.operandos = "";
                linha.contador_linha = contador_linha;
                lista.push_back(linha);
                continue;
            }
        }
        else // Caso não exista rótulo
            linha.rotulo = "";

        tokens = std::move(tokenize(s, " ")); // Separa o resto da linha em tokens

        linha.operacao = tokens[0];
        linha.contador_linha = contador_linha;

        if (tokens.size() >= 2) // Se algum operando foi fornecido
            linha.operandos = tokens[1];
        else
            linha.operandos = ""; // Caso contrário

        if (tokens.size() > 2) // Mais de dois tokens significa que tem coisa separada por espaço
        {
            TrataErros::erroEspacosEntreOp(linha);
        }

        if (linha.operacao == "begin")
        {
            flagBegin = 1;
            linhaBegin = linha;
        }
        else if (linha.operacao == "end")
            linhaEnd = linha;
        else
            lista.push_back(linha); // Coloca a linha formada no final da lista
    }
    lista.splice(lista.end(), listaData); // Adiciona a sessão de dados depois da sessão de texto
    inputFile.close();                    // Fecha o arquivo de entrada
    if (flagBegin == 1)
    {
        lista.push_front(linhaBegin); // Colocando a diretiva begin no início
        lista.push_back(linhaEnd); // e a diretiva end no final
    }


    /* 
    Checando uso de BEGIN e END
    Se apenas um módulo for fornecido, o código não pode usar begin e end
    Se dois ou três módulos forem fornecidos, o uso de begin e end é obrigatório
    */
    bool cond = (lista.front().operacao == "begin" && lista.front().rotulo != ""  && lista.back().operacao == "end");
    if(nModulos == 1 && cond){
        std::cout << "Programa com apenas um módulo não deve possuir BEGIN e END\n";
        return;
    }
    if(nModulos > 1 && !cond){
        std::cout << "Programa com mais de um módulo deve possuir BEGIN e END obrigatóriamente\n";

    }


    /*
    Primeira passagem:
    Roda o programa todo para montar a tabela de símbolos
    */
    int contador_posicao = 0;

    std::map<std::string, std::tuple<_valor, _externo>> tabelaSimbolos{};
    std::map<std::string, _valor> tabelaDefinicoes{};

    for (auto l : lista)
    {
        if (l.rotulo != "")
        {
            if (!validIdentifier(l.rotulo))
                TrataErros::erro6(l); // Indetificador inválido
            if (tabelaSimbolos.count(l.rotulo) > 0) // Se o rótulo já existe.
                TrataErros::erro2(l); // Símbolo redefinido
            else if (l.operacao == "extern")
                tabelaSimbolos[l.rotulo] = std::make_tuple(0, 1); // É externo
            else
                tabelaSimbolos[l.rotulo] = std::make_tuple(contador_posicao, 0); // Não é externo
        }

        if (l.operacao == "") // Sem instrução
            continue;
        else if (l.operacao == "public")
            tabelaDefinicoes[l.operandos] = 0;          // Insere na tabela de definiçoes
        else if (tabelaInstrucoes.count(l.operacao) > 0)                   // Com instrução
            contador_posicao += std::get<1>(tabelaInstrucoes[l.operacao]); // Soma o tamanho da instrução
        else if (tabelaDiretivas.count(l.operacao) > 0) // Diretivas
            contador_posicao += tabelaDiretivas[l.operacao];
        else
            TrataErros::erro3(l); // Diretiva ou inst inválida
    }

    // Percorre todas as entradas da Tabela de Definições copiando os respectivos atributos da tabela de símbolos
    for(auto &e : tabelaDefinicoes)
    {
        e.second = std::get<0>(tabelaSimbolos[e.first]);
    }

    /*
    Segunda passagem
    Monta o arquivo .obj, além de checar os outros tipos de erros.
    */

    std::list<std::tuple<std::string, _valor>> TabelaUso{};
    std::stringstream codigoStream;
    contador_posicao = 0;
    for (auto l : lista)
    {
        tokens = std::move(tokenize(l.operandos, ","));

        if (l.operacao == "")
            continue;
        else if (tabelaInstrucoes.count(l.operacao) > 0) // Se for uma instrução
        {
            contador_posicao += 1; // Soma o tamanho da instrução

            if (std::get<1>(tabelaInstrucoes[l.operacao]) - 1 != tokens.size()) // Número de tokens for diferente do esperado
                TrataErros::erro4(l); // Qtd de op inválida
            else
            {
                codigoStream << std::get<0>(tabelaInstrucoes.at(l.operacao)) << " "; // Escreve o opcode
                for (auto token : tokens)
                {
                    if (!validIdentifier(token))
                        TrataErros::erro5(token, l); // Tipo de op inválido
                    else if (tabelaSimbolos.count(token) == 0)
                        TrataErros::erro1(token, l); // Símbolo indefinido
                    codigoStream << std::get<_valor>(tabelaSimbolos[token]) << " "; // Escreve o operando

                    if (std::get<_externo>(tabelaSimbolos[token])) // Se o operando for externo
                    {
                        // Adiciona na tabela de uso (o -1 é porque espaço desse operando já foi contabilizado 
                        //quando somou o tamanho da instrução)
                        TabelaUso.emplace_back(std::make_tuple(token, contador_posicao));
                    }
                    contador_posicao += 1;
                }
            }
        }
        else // Se for uma diretiva
        {
            if (l.operacao == "space")
            {
                contador_posicao += 1;
                codigoStream << "0"
                           << " ";

                if (tokens.size() != 0)
                    TrataErros::erro4(l); // Quantidade de op inválida
            }
            else if (l.operacao == "const")
            {
                contador_posicao += 1;

                if (tokens.size() != 1)
                    TrataErros::erro4(l); // Quantidade de op inválida
                else if (std::regex_match(tokens[0], std::regex{"^-?[0-9]+$"}))          // Se for um número inteiro positivo ou negativo
                    codigoStream << tokens[0] << " ";                                      // Imprime o número na saída.
                else if (std::regex_match(tokens[0], std::regex{"^-?[0-9]+\\.[0-9]+$"})) // Números com casas decimais proibidos
                    TrataErros::erroNumeroFracionario(tokens[0], l);
                else // Não é um número
                    TrataErros::erro5(tokens[0], l); // Tipo de op inválido
            }
        }
    }
    codigoStream << '\n';
    path.erase(path.end() - 4, path.end());
    outputFile << "H: " << path << '\n';
    outputFile << "H: " << contador_posicao << '\n'; // Adiciona tamanho do módulo

    std::vector<int> mapaDeBits(contador_posicao); // Informaçõo de realocação

    // Calculando mapa de bits
    int iter = 0;
    for (auto l : lista) 
    {
        if (l.operacao == "")
            continue;
        if (l.operacao == "space" || l.operacao == "const")
            mapaDeBits[iter++] = 0; // Absoluto
        if (tabelaInstrucoes.count(l.operacao) > 0)
        {
            mapaDeBits[iter++] = 0; // Opcode absoluto

            tokens = std::move(tokenize(l.operandos, ","));
            for (auto token : tokens)
            {
                if (std::get<_externo>(tabelaSimbolos[token])) // Se o operando for externo
                    mapaDeBits[iter++] = 0; // Opcode absoluto
                else
                    mapaDeBits[iter++] = 1; // Operando relativo caso contrário
            }
        }
    }

    // Imprimindo mapa de bits
    outputFile << "R: ";
    for (auto v : mapaDeBits)
        outputFile << v;
    outputFile << '\n';

    outputFile << "T: "; // Preparando para imprimir código máquina

    outputFile << codigoStream.rdbuf(); // Imprimindo código máquina no arquivo

    for (auto x : tabelaDefinicoes)
    {
        outputFile << "D: " << x.first << " " << x.second << '\n';
    }

    for (auto x : TabelaUso) // Adiciona informação de uso
    {
        outputFile << "U: " << std::get<std::string>(x) << ' ' << std::get<_valor>(x)<< '\n';

    }

    outputFile.close();

    if (TrataErros::erroFlag)
        std::cout << "Falha ao montar\n";
    else
        std::cout << "Montado com sucesso.\n";
}