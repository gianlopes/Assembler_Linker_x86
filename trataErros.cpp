#include "trataErros.hpp"

bool TrataErros::erroFlag = false;

void TrataErros::start(){
    erroFlag = false;
}

void TrataErros::erro1(std::string token, tipoLinha l)
{
    std::cout << "Erro semântico: \"" << token << "\" na linha " << l.contador_linha << " - Símbolo indefinido (1. declarações e rótulos ausentes).\n";
    erroFlag = true;
}

void TrataErros::erro2(tipoLinha l)
{
    std::cout << "Erro semántico: \"" << l.rotulo << "\" na linha " << l.contador_linha << " - Símbolo redefinido (2. declarações e rótulos repetidos).\n";
    erroFlag = true;
}

void TrataErros::erro3(tipoLinha l)
{
    std::cout << "Erro sintático: \"" << l.operacao << "\" na linha " << l.contador_linha << " - Diretiva ou instrução inválida(3. diretivas e instruções inválidas).\n";
    erroFlag = true;
}

void TrataErros::erro4(tipoLinha l)
{
    std::cout << "Erro sintático: \"" << l.operacao << "\" na linha " << l.contador_linha << " - Quantidade de operandos inválida(4. instruções e diretivas com a quantidade de operando errada)\n"; 
    erroFlag = true;
}

void TrataErros::erro5(std::string token, tipoLinha l)
{
    std::cout << "Erro sintático: \"" << token << "\" na linha " << l.contador_linha << " - Tipo de operando inválido(5. instruções com o tipo de operando inválido).\n";
    erroFlag = true;
}

void TrataErros::erro6(tipoLinha l){
    std::cout << "Erro léxico: \"" << l.rotulo << "\" na linha " << l.contador_linha << " - Não é um identificador válido (6. tokens inválidos).\n";
    erroFlag = true;
}

void TrataErros::erroEspacosEntreOp(tipoLinha l)
{
    std::cout << "Erro sintático: Na linha " << l.contador_linha << " - Operandos devem ser separados por vírgula, sem espaços.\n";
    erroFlag = true;
}

void TrataErros::erroNumeroFracionario(std::string token, tipoLinha l)
{
    std::cout << "Erro sintático: \"" << token << "\" na linha " << l.contador_linha << " - Apenas números inteiros são validos.\n";
    erroFlag = true;
}
