# NL_TILE_MAP
.png demo data from REGION_FINDER for use with PROJECT-CGI-BTE121-LEAFLET

Demo data for use with: https://github.com/HakkaTjakka/PROJECT-CGI-BTE121-LEAFLET or https://github.com/HakkaTjakka/Leaflet-Minecraft-Region-Tiles-Map

Zoom level 1 t/m 10 = .jpg

Zoom level 11 = .png
  This for using them to zoom further zo you can see the pixels as squares with the exact concrete color used in Minecraft.
  For the leaflet is under construction, need to add conversion between tile coords (BTE121 projection) and lat / lon (/tpll) real world geo coords.
  This will be (first) done with interpolation of the edge coords of the whole holland map, created with the https://github.com/HakkaTjakka/project-obj-dos-cmd-python-slave
  or a-like repo with the Python script (project-obj) for converting BTE121 projection minecraft coords / region files to geo coords and vise versa.
  The holland map uses projection.py for every Google 3d Earth octant file (corners lat/lon) to convert them into BTE121 projection. (on the cm).
  This repo was meant for the leaflet javacscripts to try if the tile data can be onlin on a repo or other side.
  However this is being protected by google browser (press f12 in browser to see responce). This can be fixed somehow but i don't not how yet.
  


Demo on: http://83.83.222.154/nl/ (When server is running ;)
