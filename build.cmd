set CC0= clang++ -s -Ofast -shared bin\scripts\o\types.o
set CC1= clang++ -s -Ofast -c

mkdir bin
mkdir bin\o bin\assets bin\scripts

%CC0% net.cc -o bin\o\net.o
%CC0% file.cc -o bin\o\file.o
%CC0% types.cc -o bin\o\types.o
%CC0% logging.cc -o bin\o\logging.o
%CC0% saving.cc -o bin\o\saving.o

%CC1% game\mscript.cc -o bin\scripts\main.so
%CC1% game\physics.cc -o bin\scripts\physics.so
%CC1% game\scene.cc bin\o\logging.o -o bin\scripts\scene.so
%CC1% game\movement.cc -o bin\scripts\movement.so
%CC1% game\collision.cc -o bin\scripts\collision.so
