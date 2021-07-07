g++ -Iusr/local/include -c helloworld.cpp nqueen_propagator.cpp

g++ -o helloworld -Lusr/local/include helloworld.o nqueen_propagator.o -lgecodedriver -lgecodeminimodel -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport

./helloworld

