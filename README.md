# simple-neural-net

## Youtube Video About Benchmarking :

[Benchmarking my Neural Net](https://youtu.be/xAhWS8l8JZ4)


## How to use ?

Just run the exe.

To change the neural-net settings you can edit "settings.txt". **Make sure to give correct values**(Otherwise it will use default settings).


## What are these folders?
* 1 - Neural-Net-Settings : Don't mess with that folder(**DO NOT change it's name or directory**), make sure this folder is on the same directory with exe. Otherwise neural-net settings won't be saved and the plotter won't work.


* 2 - src : Source files. Both core of the net and python plotter script.


* 3 - statistics : This is my stats folder. I saved my experiments and benchmarks of neural-net to this folder. You can play with it.


* 4 - .vscode : I use Visual Studio Code as my code editor for this project. In this folder my VSCode Project settings are stored.


## The graph isn't showing up ?

To use realtime plotter you need to have :

Python3 and matplotlib 2.2.2 

If you don't have that it wouldn't be a problem to run/train the net.


## How to Compile ?
To compile under windows + mingw: ```g++ src/main.cpp -o xor neural-net -std=c++11```
