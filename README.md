# ProjectModernCpp

## Echipa VandGolf4

Membri:

* Pirău Tudor-Ioan (GitHub user: TudorPirauI)
* Octavian-Eduard Pantazi (GitHub user: eduard214)
* Radu-Leonard Mavrodin (GitHub user: LeoMavri)
* Miruna-Cristina Lupu (GitHub user: miru28)

## Running

Open this project in either `Visual Studio 2022` or `Clion 2024.3.1.1`. YMMV on other versions.

Wait for the indexes to be updated.

### Windows

In `CMakeLists.txt` search for the following:

```cmake
if (WIN32)
    set(CMAKE_PREFIX_PATH "C:\\Qt\\6.8.1\\mingw_64")
    set(Qt6_DIR "C:\\Qt\\6.8.1\\mingw_64\\lib\\cmake\\Qt6")
endif ()
```

Replace the two variables so that they point to the path you installed QT in. This was tested with
`QT 6.8 for desktop development` and `QT Design Studio` installed via the QT installer. YMMV if you chose to install a
slimmed down version.

Use the built in `Run` command from your preferred IDE.

### Linux

Depending on your distribution / DE, install the dependencies accordingly.

* CMake 3.30
* QT 6.8
* Catch2 (specified in `/tests/CMakeLists.txt`)
* etc. (figure it out on you're own, you're a linux user)

Use the built in `Run` command from your preferred IDE.