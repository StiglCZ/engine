# Game folder

## Internal components
This directory is used to store all the script sources, and it may be replaced by directory named "scripts" later, because that makes a bit more sence :). <br> And also to separate the internal game components and the example game<br>
So, what can you find here?

`audio.cc(.hh)` - Script for easily running audio. How? You simply use AudioSource to determine the position of the player(/listener) relative to the position of the source, its velocity, volume etc.. as well as the track you can load using the LoadTrack function. Also, if you load the wrong filename(that doesn't exist), the whole game crashes, so be aware. It can be found at the stream with its pointer Audio control with which you can find all the functions from audio you would ever need.<br>

`collision.cc` - Simply checks for collision(but it needs to be manually enabled using the vector pointer located at the data stream). Then, the gameobject.colliding will contain the collision information, aka. the object it collided into. If there are multiple objects it collided into, only the first one will be in the variable <br>

`mscript.cc` - Entrypoint of the engine. Contains all the init code <br>

`physics.cc` - Does gravity and velocity. Also, does bouncinness, which you can set in the gameobjects properties variable. Make sure your terminal velocity isn't higher than the velocity you're giving the object tho!<br>

`saving.cc`  - With this you can retrieve save request and when user chooses to save, the save script will call your function to export all the data you need. Then you can simply load them agin using Load function. The variable called node also stores the data about where was it stored, and I would recomend keeping single node per cathegory, but not dynamically changing them depending on the save, which should all be stored eighter in the first node or in the filename.<br>

`scene.cc`   - Using this you can load scene files, and then instantly apply them using the scene loader. <br>

## Game specific scripts
These are only for the example game I am doing to verify everything works, etc.<br>
`movement.cc` - Allows WASD movement to the player<br>
