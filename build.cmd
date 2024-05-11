set CC0= clang++ -Wno-everything -Ofast -c
set CC1= clang++ -Wno-everything -Ofast -shared bin\o\types.obj

mkdir bin
mkdir bin\o bin\assets bin\scripts

%CC0% renderer.cc -o bin\o\renderer.obj
%CC0% native.cc -o bin\o\native.obj
%CC0% scripting.cc -o bin\o\scripting.obj

%CC0% net.cc -o bin\o\net.obj
%CC0% file.cc -o bin\o\file.obj
%CC0% types.cc -o bin\o\types.obj
%CC0% logging.cc -o bin\o\logging.obj
%CC0% game\saving.cc -o bin\o\saving.obj

%CC1% game\mscript.cc -o bin\scripts\main.dll
%CC1% game\physics.cc -o bin\scripts\physics.dll
%CC1% game\scene.cc bin\o\logging.obj -o bin\scripts\scene.dll
%CC1% game\movement.cc -o bin\scripts\movement.dll
%CC1% game\collision.cc -o bin\scripts\collision.dll
