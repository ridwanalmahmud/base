## Installation
- Static && shared library
```bash
clang -Iinclude src/build/build.c nob.c -o nob
./nob
```
This will create libbase.a, libbase.so in linux and libbase.dll, libbase.lib in windows.

### System-wide installtion:
- [*`clibs.sh`*](https://github.com/ridwanalmahmud/.dotfiles/blob/master/scripts/workflow/clibs.sh) (linux-only)
```bash
curl -fLO https://raw.githubusercontent.com/ridwanalmahmud/.dotfiles/refs/heads/master/scripts/workflow/clibs.sh
chmod +x clibs.sh
./clibs.sh -h
```
```bash
clibs -a base -l include -L libbase.so -d "C Base Layer" -v "0.0.1"
```
This will install the library in `/usr/local/`

- For using pkgconfig
```bash
export LOCAL_LIB="/usr/local/lib"
export PKG_CONFIG_PATH="$LOCAL_LIB/pkgconfig:$PKG_CONFIG_PATH"
export LD_LIBRARY_PATH="$LOCAL_LIB:$LD_LIBRARY_PATH"
```

## Usage
- Check [howto](https://github.com/ridwanalmahmud/base/tree/master/howto) dir

> [!WARNING]
Not tested on windows
