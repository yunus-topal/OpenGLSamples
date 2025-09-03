This repo contains sample programs from this book:
[OpenGL Programming Book](https://www.amazon.com/Computer-Graphics-Programming-OpenGL-C/dp/1501522590)

## Setup
I used [vcpkg](https://vcpkg.io/en/) for installing additional libraries.

To install necessary libraries:
```
// bootstrap vcpkg first
./vcpkg/bootstrap-vcpkg.bat
// install packages
vcpkg install glew glfw3 glm imgui opengl-registry opengl soil2
```