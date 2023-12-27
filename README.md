# Limit colors in KWin

KWin plugin to stop dithering by compressing colors to 5/6 bits per component.

## Installation

### Void Linux

```sh
sudo xbps-install cmake extra-cmake-modules kwin-devel
cmake -B build -S . \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr
cmake --build build --parallel
sudo cmake --install build
```

### Arch Linux

```sh
sudo pacman -S cmake extra-cmake-modules kwin
cmake -B build -S . \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr
cmake --build build --parallel
sudo cmake --install build
```
