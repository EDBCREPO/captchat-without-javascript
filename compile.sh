export LD_LIBRARY_PATH=./lib

g++ -o main main.cpp -I./Modules -L./lib -lraylib -lssl -lcrypto -lz

#./main ?mode=generator
./main ?mode=server