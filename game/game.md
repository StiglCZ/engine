# Game folder

This directory is used to store all the script sources, and it may be replaced by directory named "scripts" later, because that makes a bit more sence :). <br> And also to separate the internal game components and the example game<br>
So, what can you find here?

`audio.cc(.hh)` - Script for easily running audio. How? You simply use AudioSource to determine the position of the player(/listener) relative to the position of the source, its velocity, volume etc.. as well as the track you can load using the LoadTrack function. Also, if you load the wrong filename(that doesn't exist), the whole game crashes, so be aware. It can be found at the stream with its pointer Audio control with which you can find all the functions from audio you would ever need. <br>
`collision.cc` - Simply checks for collision(but it needs to be manually enabled using the vector pointer located at the data stream). Then, the gameobject.colliding will contain the collision information, aka. the object it collided into. <br>
`mscript.cc` - Entrypoint of the engine <br>
`physics.cc` - Currently only responsible for gravity <br>
`saving.cc`  - With this you can retrieve save request and when user chooses to save, the save script will call your function to export all the data you need. Then you can simply load them agin using Load function. <br>
`scene.cc`   - Using this you can pre-compile scene files, and then instantly load them using the scene loader. <br>


### In progress


