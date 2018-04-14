////////////////
// HOW TO RUN //
////////////////

1. EXTRACT the project folder 'CPSC453Boilerplate' 
2. COMPILE program using the command line 'sh compile.sh'
    // had to part from using the make file due to the usage opengl
    // version 4.3 instead of 4.1. this caused some problems with
    // the provided make file.
    // this command line makes a .o file called 'solar.o'
3. RUN the program using the command line './solar.o'

////////////////
// HOW TO USE //
////////////////

SPACEBAR                    PAUSE animation
Mouse Left Click + Drag     Pan camera up/down/left/right on centered object
Scroll wheel                Zoom in/out on centered object
UP/DOWN ARROW       Speed up or slow down the animation         (2nd BONUS : 1/2)
1, ..., 0, '-'      Center camera on sun to pluto respectively  (2nd BONUS : 2/2)

////////////////
// ADDITIONAL //
////////////////

1st BONUS (realistic earth)
1/3 Specular Oceans    (checked when specular map is white and applied specular,
                        this shows the specular on oceans but not on land)
2/3 Animated Clouds    (moves slightly slower than earths rotation, might have 
                        to compare one the clouds at some position as days pass 
                        on earth)
3/3 Day/Night textures (replaced defuse near black colour with colour of night map,
                        checked if the average is > than some value to make the city
                        lights brighter)
4/3 Shadows            (wait for the moon to go infront of the earth and see what
                        happens, OR wait for the earth to be infront of the moon)

////////////////
//    OTHER   //
////////////////
- was planning on doing all moons, satellites, and comets, but too much textures to load and stats to find
- Might use my last late day to implement saturn's rings
- FOR FUNSIES: my favorite anime character travels the earth

