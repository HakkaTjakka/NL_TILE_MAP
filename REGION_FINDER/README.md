# REGION_FINDER

COPY OF REGION_FINDER REPO

Here are two versions of the REGION_FINDER
You can use it for a quick map (in pixels) of every region file in a dir.
You can change extensions for mapping other tiles. With like r.x.y.jpg, or r.x.y.mca
The 'swapped' is used where tiles like r.yyyyyy.xxxxxx.png are being used.
Its used for the MinecraftWorldEditor repo canvas poster maker.
From the large canvas in MWE (MinecraftWorldEditor), that consists of (sometimes tens of thousands) .png images on 1920x1080,
you can 'posterize' the whole canvas into tiles in a format you like. 
I will adapt this function so it can be called from the command line. 
Something like: canvas_directory (usally ../level/XXX.YYY/NNN/picture.y.x.png) and options like number x/y and size x/y.
Like: pacman.exe <source directory> <tiles x> <tiles y> <size x> <size x> [overlap] [rotation angle].
For now you can use it in MWE with the SHIFT-c key selecting one of the fixed sizes. 
1 = 5120 x 5120 pixels. For mapping 10 x 10 region files into one single .jpg file
2 = 512 x 512 pixels. For mapping region files into one single .jpg file, for later use with the REGION_FINDER

You need to sort it out for your self. The poster function(s) are in the PACMAN_CUBERITE\src\key_functions.cpp file of the MWE program.
But when selecting like 200x200 hd screens the plotted squares shown will get to slow. 
On line 8777 (when pressing ALT-'c' for posterizing) you can edit the lines:

//            rectangle_paper_count_x=750;

//            rectangle_paper_count_y=420;

to set the number of tiles horizontal and vertical directly. Of course you need codeblocks the seh compiler and compile a new pacman.exe executable.
That way you can convert the whole canvas with ultra large images into tiles of your size. They appear in the directory ../paper
(it was used to cut (large) posters out of the (larger) canvas).
Its all under construction but i publish it for the real freaks. Maybe it can assist you somehow)

So: You can use the MWE to create large canvas images (canvas will be soon endless...).

Use the MWE to cut it into the source tiles in the size you want for the zoomable leaflet script (256x256 or 512x512).

Then use the REGION_FINDER to use the ../paper/r.yyyyyy.xxxxx.jpg tiles (you can adapt the MWE and make it like r.x.y.jpg or r.x.y.png if you like)
and make the upper zoomable tile directories. like <ZOOMLEVEL>/<XDIV10>/<XDIV10>/R.X.Y.PNG

Then use and adapt the leaflet index.html script so it matches the directory structure created with the REGION_FINDER.
Matchin the paths in your webserver like in def.conf

So you have the MWE as a drawing board for ultra large canvas, like (max. now like 400 x 400 or so), giving you lots of number of canvas area's to draw on.
The MWE to cut it into tiles.
The REGION_FINDER to create the 2x2 uppper tiles per dir.
And the leaflet script for managing the website displaying the tiles.

From there you can build a website around it, like put functions and data to the tiles displayed in the browser.
I'm not a Javascript or PHP or html/css script kiddy, but for me it works fine until now.

There are a lot of repos mixing all together, that is why i wrote this file... 

Good luck. Need help? Msg me. Asking takes you 5 minutes and messing arround a week. 

Pacman Graphics.



Demo:

http://83.83.222.154/amsterdam/

http://83.83.222.154/nl/

C/C++ FAST REGIONS READER/CHECKER

![clipboard_small](https://github.com/HakkaTjakka/REGION_FINDER/blob/main/output.png)

Can be used in combination with https://github.com/HakkaTjakka/MinecraftWorldEditor (pacman.exe geo2index holland).
And https://github.com/HakkaTjakka/project-obj-dos-cmd-python-slave/blob/main/README.md
To find missing region files, create a list of marked spots in out.txt, create lat/lon list by inserting it in the project-obj TEST_LIST_TO_GEO.BAT script, then run it (result.txt) with the pacman.exe geo2index holland command, resulting in a object_array.res file, for loading the octants from object_array.txt. Msg me for help.


Usage:

region_finder.exe path_to_region_dir

or

region_finder.exe path_to_region_dir path_to_voxels_dir

(Used for comparing .mca / .vox files) BTE121 Holland).

Result is in out.txt

A '.' marks there is NO region file.

A '*' marks a region file.

When replacing a dot or asterix with another character, and then run:
 
region_finder.exe out.txt

You get a list with the r.x.z files marked. (And showing if there is a .mca or .vox file)
 
Change for other usage.

.exe compiled under Ubuntu for Windows with codeblocks and cross compiler.

Use SynWrite.6.41.2780 for column based search and replace (missing in notepad++)

***
Leaflet HTML/Javascript Minecraft Region Mapper BTE121 For Browser Plus C/C++ Zoom Level 2x2x512 Tilemaker

For use with: https://github.com/HakkaTjakka/Leaflet-Minecraft-Region-Tiles-Map

***
![clipboard_small](https://github.com/HakkaTjakka/Leaflet-Minecraft-Region-Tiles-Map/blob/main/NL/Untitled.png)
***
[![Demo CountPages alpha](https://github.com/HakkaTjakka/Leaflet-Minecraft-Region-Tiles-Map/blob/main/NL/Untitled.jpg)](https://www.youtube.com/embed/m01M_vSGTMw)
(Click to play vid)
***
  Use: region_finder.exe tiles <tiledir>
  Where <tiledir> is a directory with r.*.*.png tiles (512pxx512px) of Minecraft region files.
  Tiles can be made with the MinecraftWorldEditor program (pacman.exe from the pacman_cuberite dir)
    Using dir /saves/test/region/done0 for floor 0 (0/255), /saves/test/region/done-1 for floor -1 (-256/-1)
    /saves/test/region/done1 for floor 1 (256/511) etc.
    Then call from pacman_cuberite (start the qsetpath_seh_64_v7 - kopie.bat for dos shell with path set to pacman_cuberite/bin dir for calling pacman.exe)
    pacman.exe plot (Enter)
  Then all the tiles create from all the region files (r.x.z.mca) top view will be in pacman_cuberite/../cut (level up -> down to cut)
  If ok you end up with all tiles from your world. (Remember placing them in /saves/test/region/done0 etc...)

  Then:
  Goto the region_finder directory and call: DO_ALL.BAT
  Then first all the tiles from pacman_cuberite/../cut/r.*.*.png will be copied into seperate dirs into 18 (dir 18).
  At the same time it merges 2x2 (4) tiles together for ZOOM LEVEL 17.
  From there the program region_finder.exe is called with region_finder tiles 17
  To create the ZOOM LEVEL directory 16 out of 17, by also combining 4 512x512 textures together and place them into seperate dirs.
  The leaflet.js script the calculates the right directory and tile name, for display them in a browser that runs javascript (tm).
  You need a webserver to test it. Like https://www.apachefriends.org/
  The script is set to: http://my.ip.address/nl/
  You can add a alias dir in: /xampp/apache/conf/alias/def.conf like:

<Directory "g:/web/map">
    Options Indexes FollowSymLinks Includes ExecCGI
    AllowOverride All
    Require all granted
</Directory>
Alias /map/ "g:/web/map/"

<Directory "g:/web/nl">
    Options Indexes FollowSymLinks Includes ExecCGI
    AllowOverride All
    Require all granted
</Directory>
Alias /nl/ "g:/web/nl/"
	
  Where in my case the directories 1 to 18 reside in /web/map and the script/web dir in /web/nl. Or you have to put it in the default dir /xampp/htdocs
  In that case (also for your/the ip address/domain url) you need to adapt the script.
  When accessing from outside your router (need to be port forwared on some ports to use the webserver for access from outside on your ip address)
  you also need to adapt the ip addresses (2 pieces) or url, AND the SHA256 key.
  To get the keys easily:
    Just use a key. When webserver runs, router port forwarded, access the http://ip-address-or_domain-name/nl/ to load the index.html
    Press F12 to get the CONSOLE window, and look what chrome expects for a RIGHT SHA256 key, THEN FILL THAT IN THE index.html Javascript stuff.
    Like:
    	<script src="./leaflet2.js"
      		integrity="sha256-qV281PHHRLgHc2Yh8xiUUYArOdLIjSu4DJKvOjuUNqk="
      		crossorigin="">
	</script>
    For using internal ip (like 192.168.178.28) or external ip the SHA keys seems differen.
    Also an older version of leaflet is in the repo, there the shader area can be adapted. The newer leaflet.js has 2x 1/2 pixel smooth so lines between
    the tiles pop up. Fix it in the newer one. The older one zooms from 18 to 16 to 14 etc. So both have some bugs to keep you busy like always.

When all ok it should work. If not: MSG ME.

Its the start, its all under construction. 
Zoomable orthographic mega renders in 3d perspective of super large minecraft 1.12.2 worlds will follow. Like some google earth 3d posters i will upload
soon, just like with the tiles from this proggie.
	
	

