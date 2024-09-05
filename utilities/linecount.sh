cat -s -n \
    *.cc *.hh *.md \
    Makefile \
    game/* utilities/*.* \
    utilities/blender_ext/Makefile utilities/blender_ext/extension_src/*  \
    | wc -l | xargs -I {} echo "There are {} lines of code in the engine!"
