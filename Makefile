CC= clang++
ARGS0= $(CC) -Wall -Wextra -g -Ofast -march=native -lX11 -ftree-vectorize -funroll-loops
ARGS1= $(CC) -Wall -Wextra -g -Ofast -march=native -fPIC -ftree-vectorize -c
ARGS2= $(CC) -Wall -Wextra -g -Ofast -march=native -fPIC -ftree-vectorize -shared bin/o/types.o
ARGS3= $(CC) -Wall -Wextra -s -Ofast

# Object, script and util file output arguments
D1= -o bin/o
D2= -o bin/scripts
D3= -o bin/utils

# Some shortcuts
S0 = utilities
S1 = utilities/blender_ext

build: setup objects scripts
	$(ARGS0) program.cpp native.cc renderer.cc scripting.cc bin/o/logging.o bin/o/types.o bin/o/file.o -o bin/a.out
	strip bin/a.out
	strip bin/scripts/*
	rm -rf bin/o
setup:
	utilities/linecount.sh
	mkdir -p bin
	mkdir -p bin/o bin/assets bin/scripts
objects:
	$(ARGS1) net.cc         $(D1)/net.o
	$(ARGS1) file.cc        $(D1)/file.o
	$(ARGS1) types.cc       $(D1)/types.o
	$(ARGS1) logging.cc     $(D1)/logging.o
	$(ARGS1) game/saving.cc $(D1)/saving.o
scripts:
	$(ARGS2) game/mscript.cc   bin/o/logging.o    $(D2)/main.so
	$(ARGS2) game/audio.cc     -lopenal -lsndfile $(D2)/audio.so bin/o/logging.o
	$(ARGS2) game/physics.cc                      $(D2)/physics.so
	$(ARGS2) game/scene.cc     bin/o/logging.o    $(D2)/scenemgr.so
	$(ARGS2) game/movement.cc                     $(D2)/movement.so
	$(ARGS2) game/collision.cc                    $(D2)/collision.so
utils:
	mkdir -p bin/utils
	$(ARGS3) $(S0)/sst2stf.cpp   $(D3)/sst2stf
	$(ARGS3) $(S0)/ppm2png.cpp   $(D3)/ppm2png -lpng
	$(ARGS3) $(S0)/scenemkr.cpp  $(D3)/scenemkr
	$(ARGS3) $(S0)/modelopt.cpp $(D3)/modelopt
	$(ARGS3) $(S0)/netserver.cpp $(D3)/netserver
	$(ARGS3) $(S0)/sceneview.cpp $(D3)/sceneview
	make -C $(S1)
	mv $(S1)/scene_extension.zip bin/
run: build
	cd bin && ./a.out
all: build utils
