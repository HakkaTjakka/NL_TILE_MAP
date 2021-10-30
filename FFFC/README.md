# FFFC
Fast Threaded Image loader/converter (test phase)

Porting the threaded image loader (used on MinecraftWorldEditor) to load images simultaniously with threading. (Test phase).

See source.

While the directories are scanned for images, its already starting threads to deal with the images.
In this case its only converting them to another type, like .jpg .png .bmp
There need to do some work on how the printf output to stdout is holding back the directory scanning, because of slow output, when tiles are loaded and saved in different format. It needs to be tuned for that. In the MinecraftWorldEditor its only loading like 100 to 800 .jpg tiles for rendering/conversion etc. Here it also is being saved, holding back other threads and stdout output to the console. Like when not printing to the console in the directory reading (main) thread is speeding up the directory scanning.

meant for large bulk conversions. preserving directory structure of the tiles. 
Use -t flag to adapt number of threads. Small for large images, higher for small images and/or tiles.

Like Irfanview is using one day to convert 40.000 1920x1080 images from .jpg to .png.
This is like 10 to 20 times faster.

Under construction. All used libs used in codeblocks included. 
Compile with mingw-seh compiler in Codeblocks (check the toolchain settings on compiler used).
