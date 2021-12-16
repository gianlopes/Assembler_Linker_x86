Trabalho 2 Software Básico
Gianlucas Dos Santos Lopes - 18/0041991

O trabalho está dividido entre os seguintes arquivos:
    montador.xpp: implementação geral das passagens do montador
    trataErros.xpp: uma classe para tratar do output dos erros encontrados no montador
    stringUtils.xpp: funções para lidar com strings (trim, toLower, etc...)
    ligador.cpp: implementação geral do ligador
    simulador.cpp: implementação geral do simulador


O compilador usado foi o g++ versão 9, em um sistema linux (ubuntu)
Para compilar os dois executáveis, um makefile foi incluído, use:
    make


Para executar o montador use:
    ./montador aquivo.asm
    ./montador prog1.asm prog2.asm prog3.asm

(Será gerado "arquivo.obj")


Para executar o ligador use
    ./ligador arquivo.obj
    ./ligador prog1.obj prog2.obj prog3.obj

(Será gerado "arquivoLigado.obj")

Para executar o simulador use:
    ./simulador arquivoLigado.obj
    ./simulador prog1Ligado.obj

(Será gerado "arquivo.out")

*As extensões são importantes*