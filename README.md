An OpenGL program displaying a sphere using ray tracing for lighting and reflections. Displays a single sphere as defined in the inputted text file.


## Compiling 
### Compile with: 
```
make 
```
### System Requirements 
- Mac OS 10.12 + 
- OpenGL

## Running 
```
./sphereRT examples/1.txt
```

## Screenshots 
- Screenshots visible in screenshots folder

## Input Files
Input files are defined as: 
```
light   X Y Z
sphere  X Y Z    Radius     (Ambient) R G B     (Diffuse) R G B    (Specular) R G B
```
With each available as a float value.

![Model 1](/screenshots/1.png?raw=true "Model 1")
