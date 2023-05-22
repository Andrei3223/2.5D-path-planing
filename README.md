# 2.5D-path-planing

## General description
  This project can process GeoTIFF files, visualize landscape (terrain) data. And there is a path planner on 2.5D environment.
  
  This repo has two parts: 

  - A* algorithm for the path planning on terrain data (c++)
    
  - Preparing and visualizing data (Jupyter notebook)
    
  You can also find `Data` folder with prepared data arrays (200 x 200 arrays, where $arr_{ij}$ is the height of the ij point). There are three types of arrays: flat (with name *data_elev_arr*), medium (*medium*) and arrays with very sharp height changes (*mount_arr*). You can use these arrays as examples of terrain data.
  
  If you want some specific data, you can download it as *.tif* file and get the elevation data using the first part of Jupyter notebook.
  
## Preparing the data

  (\* first part in notebook*)
  
  If you want to use a real terrain data you can download it from https://apps.nationalmap.gov/downloader/#/ or https://www.pgc.umn.edu/data/elevation/ (all the data form folder *Data* was taken from first one).
  
  Quick guide how to download anything from https://apps.nationalmap.gov/downloader/#/ :
  
  1. In the database address bar, search the place you want.
  2. Zoom to the region of interest.
  3. On the Datasets tab, under Data, select Elevation Products (3-DEP) > 1 meter DEM, and click Search Products.
  4. Download the file covering the region for which you want elevation data.

Now using code in notebook you can get elevation data and split it to several .txt files if you need.

Be ready that sometimes in .tiff files, some data can be missed. This data is marked as some small numbers like -9999. or less (depend on the file). If you want to generate missed data, you can use `gen(arr)` function (implemented using BFS).


## Visualizing
 
  In the first part of notebook, you can find the implementation of the function that makes a plot of the surface `draw_plt(arr)` from the numpy array (which you can get from GEOTIFF file in previous step) or load data from text file (lower in notebook). You can use `gen(arr)` function to generate missed values. 
  
  In the second part there is a function `draw_plt_with_path(arr, path)` which takes two numpy arrays (surface data and the path) and visualize these data both in usual plot and the wireframed one (sometimes the path can be covered by surface, for this reason the wireframed plot is used).
  
  
## Path planner

Parameters:

`-s` [int x, int y] source (the start coordinates of the searched path) 
`-t` [int x, int y] target (the target coordinates of the searched path) 

`-w` [double value] weight of height changing (default =1, same as changing x and y coordinates) (the higher - the less height changes in the result path)

`-lb` [double] or `--lower_bound` [double] the lowest possible height

`-ub` [double] or `--upper_bound`[double] the highest possible height

`-f` with this parameter for each node there are only four neighbors (vertical and horizontal) as in the first picture

  without it (default) you can also move diagonally as in the second picture

     *  |   *                      \  |   /
     - node -                      - node -  
     *  |   *                      /  |   \

`-l` [double] or `--cell_len` [double] the length of one node in the input array

`-av` [double] or `--angle_vertical` [double] angle $\in$ [0, 90]  - the maximum angle between two neighbor points in the path (specify cell length to use this parameter)

## Getting started
  <h4> In case you what to get data from GEOTIFF files: </h4>
  Install GDAl packages (ubuntu guide: https://mothergeo-py.readthedocs.io/en/latest/development/how-to/gdal-ubuntu-pkg.html)
  
  <h4> For both visualizing and data preparing: </h4>
  
  You need to have installed matplotlib and numpy.
  
  <h4> For path planning: </h4>
  
  TODO
