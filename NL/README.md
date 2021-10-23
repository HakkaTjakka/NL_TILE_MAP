This is for your webserver.
You need to adapth the paths in the index.html according to the directories on your webserver.
These can be anywhere using the the file xampp\apache\conf\def.conf in your apache webserver (xampp).
A example is included.
By using the coords.x and coord.y you can select area's in your zoomable map. 
Then point them to the according directories where the map directories are located pointed by your def.conf,
or if it resides in your xampp/htdocs dir you don't need to use the def.conf.
This way you can create several zoomable maps on different locations, and use them in the same html file.

There is also a leaflet repo that can use the cgi-bin, so the tiles (or other html data) can be delivered by a .exe executable,
that just prints the output (like .jpg's, html, or whatever) straigt to the client. 
That way an executable can create images, like fonts, or other graphical data and/or html, javascript, from a program,
or database, or whatever.

I include the tile maker. That can create the higher zoom levels from a set of tiles.
The source directory then includes all tiles in one dir. According to the filename like r.x.y.jpg, and/or r.y.x.jpg, they are read, sorted,
and combined. 4 tiles create one new tile in a lower directory number. Per zoom level the program can be called.

For programmers c/c++ i have now two versions. One that reads like r.x.y.jpg, and one that reads r.y.x.jpg.
The resulting dirs will be always r.x.y.jpg (r.y.x.jpg is used for nice directory sorting in explorer, showing x first, then y)
You can adapt them for other tile name formats. Normally the .jpg filename does not include the x/y coords, i do.
Resulting in a path that is like map/xdir/ydir/r.x.y.jpg, where two times a x and y value is used.
One for the directories xdir and ydir (like x/100 and y/100, giving 100 dirs for x and 100 for y inside the x), and a x and y for the tile.
That way i use the .extend leaflet function, where you can control a path to the tile and where you can used more calculations on the final directory.

