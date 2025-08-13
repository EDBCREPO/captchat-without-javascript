export LD_LIBRARY_PATH=./lib

if [ ! -d "Build" ]; then
     mkdir "Build"
fi

g++ -o ./Build/main ./Service/main.cpp -I./Modules -L./lib -lraylib -lssl -lcrypto -lz

echo -e "generating captchat \n"
./Build/main ?mode=generator

echo -e "testing captchat \n"
./Build/main ?mode=server