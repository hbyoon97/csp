g++ -Iusr/local/include -c helloworld.cpp

g++ -o helloworld -Lusr/local/include helloworld.o -lgecodedriver -lgecodeminimodel -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport

./helloworld input.txt
