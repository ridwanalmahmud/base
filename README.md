## Usage
- Static && shared library
```bash
clang build/build.c nob.c -o nob
# or with gcc and msvc
./nob
```
This will create libbase.a, libbase.so in linux and libbase.dll, libbase.lib in windows.

- For system-wide installtion: [`clibs.sh`](https://github.com/ridwanalmahmud/.dotfiles/blob/master/scripts/workflow/clibs.sh) (linux-only)
```bash
curl -fLO https://raw.githubusercontent.com/ridwanalmahmud/.dotfiles/refs/heads/master/scripts/workflow/clibs.sh
chmod +x clibs.sh
./clibs.sh -h # for usage
```
```bash
# # this will install the library in /usr/local/
clibs -a base -l . -L libbase.so -d "C Base Layer" -v "0.0.1"
# # for using pkgconfig
# export PKG_CONFIG_PATH="$LOCAL_LIB/pkgconfig:$PKG_CONFIG_PATH"
```
For windows you have to move the `.dll` file to somewhere like `C:\Windows\System32\`

> [!NOTE]
To see the corresponding usage, check the end of `.h` files.

> [!WARNING]
I never really used windows or msvc so you might need to update that. \
Or Just make an issue in this repo.
