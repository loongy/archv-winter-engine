# INSTALL #

## OS X ##

1. open the terminal
    1. run "cd <winter-root-dir>/projects/eclipse"
    2. run "chmod +x ./tools/build_contrib"
    3. run "./tools/build_contrib"
2. open Xcode
    1. open the project "<winter-root-dir>/project/xcode/winter.xcodeproj"

## Ubuntu ##

1. open the terminal
    1. run "cd <winter-root-dir>/projects/eclipse"
    2. run "chmod +x ./tools/build_contrib"
    3. run "./tools/build_contrib"
2. open eclipse
    1. import the project "<winter-root-dir>/project/eclipse"

## Windows ##

1. navigate to "<winter-root-dir>/contrib/zip"
2. unzip all the libraries
3. install Assimp:
	1. genereate Visual Studio 12 2013 project using CMake
	2. set Project > Properties > C++ > Code Generation > Runtime Library > MT (MTd for Debug)
	3. move the debug library to "<winter-root-dir>/contrib/lib/assimpd.lib"
	4. move the release library to "<winter-root-dir>/contrib/lib/assimp.lib"
	5. move the zlib debug library to "<winter-root-dir>/contrib/lib/zd.lib"
	6. move the zlib release library to "<winter-root-dir>/contrib/lib/z.lib"
	7. move the "<assimp-root-dir>/include/assimp/" to "<winter-root-dir>/contrib/include/assimp/"
4. install FreeImage:
	1. build FreeImage using visual studio projects
	2. move the debug library to "<winter-root-dir>/contrib/lib/freeimaged.lib"
	3. move the release library to "<winter-root-dir>/contrib/lib/freeimage.lib"
	4. move the "<freeimage-root-dir>/Dist/FreeImage.h" to "<winter-root-dir>/contrib/include/freeimage/FreeImage.h"
4. install FreeType:
	1. genereate Visual Studio 12 2013 project using CMake
	2. set Project > Properties > C++ > Code Generation > Runtime Library > MT (MTd for Debug)
	3. move the debug library to "<winter-root-dir>/contrib/lib/freetyped.lib"
	4. move the release library to "<winter-root-dir>/contrib/lib/freetype.lib"
	5. move the "<freetype-root-dir>/include/" to "<winter-root-dir>/contrib/include/freetype/"
5. install GLM:
	1. move "<glm-root-dir>/" to "<winter-root-dir>/contrib/include/glm/"
6. open Visual Studio 2013
	1. open the project "<winter-root-dir>/project/vs13/winter"
