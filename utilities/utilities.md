# Utilities
Utilities don't rely on other engine files, but sometimes do rely on external libraries!(e. g. libpng)

### model2opt
Optimizes/(deoptimizes) obj files, with the second gen of stiglengine model optimizer.

Arguments:<br>
`-h`: Displays help<br>
`-d`: If this argument is placed first, it will be in decompile mode <br>
`-u`: Provide an UID when compiling <br>

After this, provide a file to compile/decompile and you should be good to go!

### scenemkr
Converts raw-text scene file into its binary form<br>

You will create 3 files, one specifying models, second scripts, and third objects, then you can convert them back

Arguments:<br>
`-h` : Display help <br>
`-m` : Specify each model file on 1 line, and provide the file as second arg <br>
`-s` : Specify each script file on 1 line, and provide the file as second arg <br>
`-o` : Specify gameobjects. One gameobject on each line, and each property(number) separated by spaces, in this order: position, rotation, scale, model, script, etc. <br>
`-c` : Specify 3 arguments as filepaths, model, script and finally object, in this exact order. That will result in the final scene file <br>

### sceneview
Allows you to view contents of a scene file(stf format)<br><br>

Simply provide it with the scene file(in the binary form) and it will display you all the data it contains.

### ppm2png
Converts PPM3/PPM6(the format the engine saves screenshots as) to normal(lossless), png format.<br>

You can provide the ppm file(regardless of if its ppm3 or ppm6) and it will output it out as `output.png`. Keep in mind that it will overwrite any other `output.png` in the same directory as you ran it in!<br>

### sst2stf
Converts the blender scene format to normal scene format<br>

Arguments:<br>
`-h`: Display help <br>
`<sst file> <directory>` - Converts the obj file<br><br>

sst file: File that you exported from blender<br>
directory: Directory that **all** your obj files are in, and you imported them like that from blender. If you will provide nothing as the directory argument, it will only run on your machine, and nobody elses.

### Blender extension

Extension on top of blender to help with designing scenes in 3D space.

#### How to install?

Simply, you need to go to blender -> edit -> preferences -> addons. From there, you can simply select install addon and select the file itself.
Then, restart blender, and you should be good to do!
After this process, new import and export features will apear in your blender options.
    
#### How to use?

To use the extension, you first need to model some 3D objects like normal, and export them as the OBJ format, I recomend you to store them directly in the assets folder inside your game directory. After that, reopen blender, and import your object file with the new option(the bottom .obj option in File -> Import), importantly, you have to import your files ONE AT THE TIME, so it doesn't break, which would make this entire process useless. If you want to save your scene so you can work on it later, save it normally as .blend with C-s.<br>
Now, you will see 2 variables in the custom object properties section when selecting an object you imported this way. First is filename, which you don't have to worry about, as long as you imported the object files from the assets directory, if you didn't, you will have to specify the path of all models directory, which will be removed by the convertor utility. What you can do with the second one is change the RGB color of the objects, 0 0 0 being  black and 255 255 255 being white. Its also important to note that I didn't yet figured out how to make limits from 0 to 255 on the color. So please, don't make them negative or over positive, etc. That will break the convertor<br>

After you're happy with your scene and want to export it, you simply export the scene as .sst in File -> Export. Simply export it in any folder you like, because you won't need this file in production. To use it in the game, you will need the util file called sst2stf, which comes compiled when you do `make utils` or just download the build off of github action artifact. You now need to provide 2 arguments, the path to the .sst file we exported earlier, and directory path, that you need to enter your assets directory absolute path into. What this essentially does is convert /home/.../.../bin/assets/objs/obj1.obj to objs/obj1.obj, so the engine can access it, so in this case the directory would be `/home/.../.../bin/assets` and the .sst path the path to the file. <br>
This util will leave you with a single `scene.bin` file, which you can simply put into the assets, and load whenever you feel like!
