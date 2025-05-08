# Empty OpenGL Renderer

![Sponza](screenshots/sponza.png)

## Build

Build in debug:

```bash
cmake -D CMAKE_BUILD_TYPE=Debug -B build/
cmake --build build/ -j $(nproc)
```

Build and run in debug:

```bash
./run.sh
```

## Features

* Model loading using Assimp library
* Blinn-Phong lighting
* Text rendering using Freetype library
* Billboarding
* Dear ImGui integration

## Learning resources

* [LearnOpenGL](https://learnopengl.com)
