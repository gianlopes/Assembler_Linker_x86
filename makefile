g++:= g++ -std=c++14 -g -Wall -Wextra -pedantic 

all: montador ligador simulador

montador: montador.o trataErros.o stringUtils.o
	$(g++) -o $@ montador.o trataErros.o stringUtils.o

montador.o: montador.cpp montador.hpp trataErros.hpp stringUtils.hpp
	$(g++) -c montador.cpp

trataErros.o: trataErros.hpp montador.hpp trataErros.cpp
	$(g++) -c trataErros.cpp

stringUtils.o: stringUtils.hpp stringUtils.cpp
	$(g++) -c stringUtils.cpp

ligador.o: ligador.cpp stringUtils.hpp
	$(g++) -c ligador.cpp

ligador: ligador.o stringUtils.o
	$(g++) -o $@ ligador.o stringUtils.o

simulador: simulador.cpp 
	$(g++) -o $@ simulador.cpp

clean:
	@rm -f *.o montador ligador simulador