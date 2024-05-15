# WireFrame engine

This game engine is completely wireframe and runs only on CPU!
Good to note that the engine may be modified by the specific game!

### Scripting and modding

All communication in the scripts is handled by the GameData variable.
Script Example:
```c++
#include "../types.hh"
extern "C"{
    void Init(GameData* gd, uint scriptIndex){
        // This function works across all gameobjects 
        // and is primarily used to copy the gameData
        // pointer to different location for later 
        // use
        
        // ScriptIndex is the index of the script in the scriptBuffer
    }
    void Start(uint index){
        // This function runs for each gameObject when its created
        // Also good to note the index is index of the gameObject
        // invoking the function
    }
    void Update(uint index){
        // This runs for every object everyframe
    }
}
```

Compiling the script:
`clang++ -shared -fPIC <SCRIPT>.cc types.cc -o .../bin/scripts/<SCRIPT>.so`<br>
Note: You dont have to include the types.cc if you are not planning to use
functions included in it (Vector3 operations, matrix rotations, etc.)

### Data stream

The engine includes a data stream for communication between multiple scripts.
Data Stream is actually just a array of nothing, shared accross all scripts, and 
should be used to give pointers to custom objects, not directly share data on it.

Size of the stream will range in kilobytes, depending on the size of your game
Default is 8K

How to resize this data stream you may ask? Well, you can just point another data stream at the end of the first one,
which will extend the data capacity of the stream.

### Video settings

The engine currently uses x11 to display graphics and the resolution is not currently hot-changable!
The aspect ratio should be working however!

To change the framerate, you can just change the sleep value in program.hh
Currently capped at ~960FPS!

### Porting

Porting the wireframe engine should be really, really easy compared to other engines. Thats because majority of the engine does not relly on clib or c++lib.
You only need to port few files, depending on the platform.

So these are the files you will propably need to edit.                     <br>
`scripting.cc`   - Includes the scriptloading etc.                         <br>
`logging.cc`     - Includes cout and string                                <br>
`native.cc`      - Includes the graphics                                   <br>
`file.cc`        - Includes fileloading(should be universal for most cases)<br>
`net.cc`         - Includes UDP/IP implementation                          <br>

<br>

`game/saving.cc` - Includes fileloading and filesaving                     <br>
`game/scene.cc`  - Includes fileloading                                    <br>
`game/audio.cc`  - Includes native audio(right now using openal)           <br>

<br>

What libraries do you need to change exactly(commonly)?<br>
- `dirent.h` - In file.hh     <br>
- `dlfnc.h`  - In scripting.hh<br>
- `libx11`   - In native.cc   <br>
- `signal.h` - In native.cc   <br>
- `socket.h` - In net.cc      <br>
- `typeof`   - Preety much everywhere :D <br>
<br>

### Optimilization

Basically, renderer.cc is optimized very well, and its really hard to read it. If you're wondering why I didn't use loops in it,
and instead written #pragma unrool in front of everyone of them, its because loops are slow. And renderer is something that needs to be really really fast,
so its best if no if or similiar are even required. Everything else is not very very optimized, but I would say its still optimized quite a bit. <br>

So if you don't know what you're doing, do not enter renderer.cc. It doesn't require to be edited if ported, so no need there etc. 
Thanks!

#### Uses for the data stream
0 - Scene management  <br>
1 - Currently unused  <br>
2 - Collision manager <br>
3 - Audio manager     <br>
4 - Physics manager   <br>

#### Audio

Including audio.hh in your script allows you to allow audio in your game. 
You just need to get the AudioControl struct from the pointer located at the above mentioned stream, which allows you to access all the important functions.
Functions themselves are self explanatory!
Newly, audiosource has been added for easier manupulation! Feel free to now edit the audio while running!

#### Saving

For easier porting, the saving system has created in saving.cc. You can simply add a saveinforequest which will trigger the referenced function when its time to save, and your function will simply return all the data it needs to save, as well as the location(node number) where to store them in, and the the system will automatically order and save all the packets.

#### Scene managing

You can quickly load scenes using the scene manager, instead of manually placing objects. 
You can design a simple scene template file like this:
```
.S
...
.M
...
.O
...
.U
...
```
In the .S section, you can define all the scripts, and they will be loaded.                                                        <br>
In the .M section, you can define all the models, and they will automatically be loaded, and placed into the model buffer.         <br>
In the .O section, you can define all the gameobjects that should be spawned, and they will.                                       <br>
In the .U seciton, you should define all the models that should be disposed after the scene is unloaded or replaced by another one.<br>

##### Alert: This is still usable and stuff, but theres new scene designer using blender. More info in utilities/utilities.md

#### Collision

The engine currently uses AABB collision. Every gameobject has its own collsion box(defined by position + AABB), and you can enable collision for specific object by pushing its id to the list of them in stream positon 2. The collison system will then automatically send you back the data with the collision status.

#### C++ compatibility

As for the compatibility with the C++ itself, the program is compatible with preety much everything. For audio and video C functions are used(openal and x11) and as for the C++ libs, only iostream, fstream, vector and string (and maybe something else I included after) should be used. So, if you use C++11+ then it will definetly be compatible!
Nvm, it isnt. Tried to migrate it into windows, no idea what its doing. 
Can someone explain why `typeof` doesn't exist on windows?

#### Models

This engine originally used OBJ's for encoding models, but it was replaced by an optimized binary version. If you want to optimize your models with it, just try to load the OBJ file via the engine, and it will automatically get converted to the optimized version, or use the object optimizer(v2) inside utilities, where you can also partially decompile it back to OBJ.

#### Security

Please, do not download 3rd party mods if you can't see the source without first checking with something like virustotal. 
The mods are just plain binaries, and the engine has no control over them and neighter do I, so its on your own risk to download 3rd party mods.

#### Index 0 thingies

The index 0 script = empty script <br>
The index 0 model = empty model (invisible)<br>

#### Networking

The engine provides a simple UDP interface in the net.o object, as well as its header, net.hh. It provides with such instructions as send, recieve, host, or start the server. If your server is using TCP, UDP is preety much always compatible except it may look weird from the server side, because no message recieved signals are being sent.

#### Contents of the files
 - .hh files are contained in this as the .cc files                                        <br>
`program.cpp` - Contains primarily main loop and some init code                            <br>
`scripting.cc`- Contains scripting utils(loading script, getting its subfunctions, etc.)   <br>
`renderer.cc` - Contains the rendering, such as matrix-vector multiplication etc.          <br>
`math.cc`     - Might contain custom functions if the clib shoudln't be required           <br>
`native.cc`   - Contains native rendering bindings(and somewhat input)                     <br>
`audio.cc`    - Script playing audio using OpenAL(tm)                                      <br>
`file.cc`     - Mainly model loading, and also file utils                                  <br>
`types.cc`    - Extension for `types.hh` and utilities for matrixes such as rotation       <br>
`mscript.cc`  - Startup script for the engine, it should load and set up all the other ones<br>
`Makefile`    - Simple script for building the engine                                      <br>

#### Files storing memory(bigger in memsize)

`native.cc`    - About 1KiB, mainly windowing stuff<br>
`audio.cc`     - OpenAL eats about 5.7MiB of memory<br>
`scripting.cc` - Handlers list                     <br>
`renderer.cc`  - Matrix data and linecounter       <br>

#### Method of storing

This engine has its own proprietary filetypes, each for different use, but the thing stays the same,
the file consists of:

`size of elements`               - Commonly an ushort <br>
    `size of element(if needed)` - Commonly 1 byte(X) <br>
        `element`                - X bytes long       <br>

#### Why dont I use git for this?

I.. don't want to. As cool as it would look to have everyday commits on github chart, I just dont feel its required. Eventually I will have to release to upload it on github, and therefore use git tho, so.. I hope that will go well.


#### Contributing

If you choose to contribute.. you have to go according to the (mainly)syntax standards. Keep those spaces aligned! Also, don't add 5 dependencies because you feel like it. The engine is lightweight, and personally I wouldn't use openal if there was anything else supporting good 3d audio(because it takes over 5Megs of ram), but I had to. Use the cppstdlib if you need to :)

#### Why does this project use partly python?

Sadly, blender only supports extensions made in python. If they ever release better variant, I will make sure to update this repo, if I will still be maintaining it that time. If you see this in a tar archive and the repo is long gone, I am propably not maintaining it anymore
