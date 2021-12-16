#ifndef _MONTADOR_HPP
#define _MONTADOR_HPP

#include <map>
#include <tuple>
#include <string>

typedef struct
{
    std::string rotulo, operacao, operandos;
    int contador_linha;
} tipoLinha;

typedef unsigned int instOpcode, instSize, _externo;
typedef int _valor;

static std::map<std::string, std::tuple<instOpcode, instSize>> tabelaInstrucoes {
    {"add", std::make_tuple(1, 2)},
    {"sub", std::make_tuple(2, 2)},
    {"mul", std::make_tuple(3, 2)},
    {"div", std::make_tuple(4, 2)},
    {"jmp", std::make_tuple(5, 2)},
    {"jmpn", std::make_tuple(6, 2)},
    {"jmpp", std::make_tuple(7, 2)},
    {"jmpz", std::make_tuple(8, 2)},
    {"copy", std::make_tuple(9, 3)},
    {"load", std::make_tuple(10, 2)},
    {"store", std::make_tuple(11, 2)},
    {"input", std::make_tuple(12, 2)},
    {"output", std::make_tuple(13, 2)},
    {"stop", std::make_tuple(14, 1)}};

static std::map<std::string, instSize> tabelaDiretivas{
    {"space", 1},
    {"const", 1},
    {"begin", 0},
    {"end", 0},
    {"public", 0},
    {"extern",0}};

#endif
