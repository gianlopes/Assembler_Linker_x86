#ifndef _TRATAERROS_HPP
#define _TRATAERROS_HPP

#include "montador.hpp"
#include <iostream>

class TrataErros
{
public:
    static bool erroFlag;

    static void start();

    static void erro1(std::string token, tipoLinha l);

    static void erro2(tipoLinha l);

    static void erro3(tipoLinha l);

    static void erro4(tipoLinha l);

    static void erro5(std::string token, tipoLinha l);

    static void erro6(tipoLinha l);

    static void erroEspacosEntreOp(tipoLinha l);

    static void erroNumeroFracionario(std::string token, tipoLinha l);
};

#endif
