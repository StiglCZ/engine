# Utilities
Utilities cannot rely on other gameengine files, but sometimes do rely on external libraries!(e. g. libpng)

### model2opt
Optimizes/(deoptimizes) obj files, with the second gen of stiglengine model optimizer.

### scenemkr
Converts raw-text scene file into its binary form

### sceneview
Allows you to view contents of a scene file(stf format)

### ppm2png
Converts PPM3/PPM6(also known as the format the engine saves screenshots as) to normal, png format.

### sst2stf
Converts the blender scene format to normal scene format

### blender extension

#### How to install?

    Simply, you need to go to blender -> edit -> preferences -> addons. From there, you can simply copy the file itself.
    Then, restart blender, and you should be good to do!
    After this process, new import and export features will apear in your blender options.
    
#### How to use?

    To use the extension, you first need to model some 3D objects like normal, and export them as the OBJ format, I recomend you to store them directly in the assets folder inside your game directory. After that, reopen blender, and import your object file with the new option(the bootom .obj option in File -> Import), importantly, you have to import your files ONE AT THE TIME, so it doesn't break, which would make this entire process useless. If you want to save your scene so you can work on it later, save it normally as .blend with C-s. Now, you will see 2 variables in the custom object properties section when selecting an object. First is filename, which you don't have to worry about, as long as you imported the object files from the assets directory. What you can do with the second one is change the RGB color of the objects, rgb order and 0 0 0 is black(and 255 255 255 is white).
    After you're happy with your scene and want to export it, you simply export the scene as .sst in File -> Export. Simply export it in any folder you like, because you won't need this file in production. To use it in the game, you will need the util file called sst2stf, which comes compiled when you do `make utils` or just download the build off of github action artifact. You now need to provide 2 arguments, the path to the .sst file we exported earlier, and directory path, that you need to enter your assets directory absolute path into. What this essentially does is convert /home/.../.../bin/assets/objs/obj1.obj to objs/obj1.obj, so the engine can access it, so in this case the directory would be `/home/.../.../bin/assets` and the .sst path the path to the file. 
    This util will leave you with a single `scene.bin` file, which you can simply put into the assets, and load whenever you feel like!
