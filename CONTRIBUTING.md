# Contributing

So you wanna contribute to this repo, don't ya. Well, thanks for even considering contributing to this repo, thanks! But, there is still rules you have to folow to contribute, so the codebase doesn't turn into complete mess.<br>
Here is a few of them:<br>

## Only working pull requests

If your edit doesn't compile, don't create a pull request. Fix any complier issues, and make sure the engine(and your example project to test the features) works as it should with your edits. <br>
- However, this doesn't apply if the thing hasn't been already working before. If you improve something that didn't work(e. g. the windows port) then go ahead, and press that submit pull request button

## No uselless dependencies

Don't add any dependencies to the project unless its really necesarry. For example if you found a more lightweight way to do 3D audio than openal, go change it for sure, but just don't add libraries like is-odd and stuff.

## Keeping the syntax

Under no circuimstances should you add a pull request if the syntax isn't right. The syntax of the blender extension can't change because of.. interesting language design choices, but these rules apply mainly to C++, so here they are:

### Inline curly braces

All functons, struct etc. needs to have inline curly braces like this:
```
void function() {
    // Code
}

matrix4x4 m{
    // Data
}
```
### Spacing

This one is preety simple. Everything is using spaces at start of the line, with each level being 4 spaces. <br>
Also good to note, if you have multiple variables defined somewhere, keep them aligned, what do I mean?:
```
ulong var0 = 100;
int var1 = 10;
short var2 = 5;
long var3 = 100;
```
Shold instead be:

```
ulong var0 = 100;
short var1 =   5;
long  var3 = 100;
int   var1 =  10;
```
This keeps to code readable for quick edits! 
Note that the spacing shouldn't be too extreme, so if you're having 20spaces between two variables to do this propertly,
you should think about rather changing your naming scheme

### Newlines

There shouldn't be just 500newlines in middle of a file, aswell as newline on the end of each file.

### Global vars with the same type 

When defining global variables with the same type(commonly constants), there should always be aligning with the same variable, or newline at the end of the type, like this:

1. scenario<br>

```
const very_long_struct_name
    var1 = {},
    var2 = {};
```

2. scenario<br>

```
const int var1 = 10,
          var2 = 20,
          var3 = 30;
```

### Naming everything

Functions, structs etc. should be named with big letters at the start, like this: 
`struct GameData{};` 
<br>

Their instances should be named with first letter small and any other start of word big, like so: 
`GameData* gameDataPtr = 0;`
<br>

For iterators in a loop, you shold use the `i` as the variable name, then `j`, then `k` or you can also do `i` and then `n`. 
<br>

For game data instance pointers in scripts, you should use `d`, or if that is allocated for something else use `gd` or `gd2`, and the index script should be named `index` or `si`. 
<br>

Everything that is inside a struct accesible to the scripts, should be a constant pointer with any value, so the scripts cannot break the whole game by changing the value of those to something else.
<br>

### Commit names

Commit name should be relatively descriptive, like: `added gpu.cc` file. The name should not be something like: <br>
`LETS GOOOO`       <br>
`It finally works` <br>
`Fixed!!!`         <br>
`Very very long commit name with over ~40characters` <br>

If your attempt was not successful or you improved your previous commit, you can add (NUMBER), like this: <br>

`added gpu.cc` <br>
`fixed gpu.cc` <br>
`fixed gpu.cc(1)` <br>
`fixed gpu.cc(2)` <br>

Where first commit with no index is considered as (0). So if you already know you will be needing multiple commits to fix it, add (0) to the commit.
