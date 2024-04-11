# elfconv
elfconv is an experimental AOT compiler that translates a Linux ELF binary to executable WebAssembly.
elfconv converts an original ELF binary to the LLVM bitcode using [remill](https://github.com/lifting-bits/remill) (library for lifting machine code to LLVM bitcode)
and elfconv uses [emscripten](https://github.com/emscripten-core/emscripten) (for browser) or [wasi-sdk](https://github.com/WebAssembly/wasi-sdk) (for WASI runtimes) to generate the WASM binary from the LLVM bitcode file.

## Status
"**elfconv is a work in progress**" and the test is insufficient, so you may fail to compile your ELF binary or execute the generated WASM binary. Current limitations are as follows.
- Only support of aarch64 ELF binary as an input binary
    - Furthermore, a part of aarch64 instructions are not supported. If your ELF binary's instruction is not supported, elfconv outputs the message (\[WARNING\] Unsupported instruction at 0x...)
- No support for stripped binaries
- No support for shared objects
- a lot of Linux system calls are unimplemented (ref: [`runtime/syscalls`](https://github.com/yomaytk/elfconv/blob/main/runtime/syscalls))
## Quick Start
You can try elfconv using the docker container (amd64 and arm64) by executing the commands as follows and can execute the WASM application on the both browser and host environment (WASI runtimes).
> [!WARNING]
> The container generated by the Dockerfile includes the installation of [wasi-sdk](https://github.com/WebAssembly/wasi-sdk). However, wasi-sdk doesn't release the package for arm64, so we build wasi-sdk in the arm64 container image, and it takes a long time. If you try elfconv, it might be good to use elfconv [release packages](https://github.com/yomaytk/elfconv/releases), or comment out the lines of the installation of wasi-sdk (The relevant part is as follows in the Dockerfile) and try to execute only in the browser.
> ```bash
> elif [ "$( uname -m )" = "aarch64" ]; then \
>     cd /root && git clone --recursive https://github.com/WebAssembly/wasi-sdk.git; \
>     cd wasi-sdk && NINJA_FLAGS=-v make package; \
> fi
> ```
### Browser
```bash
$ git clone --recursive https://github.com/yomaytk/elfconv
$ cd elfconv
elfconv/$ docker build . -t elfconv-image
elfconv/$ docker run -it --rm -p 8080:8080 --name elfconv-container elfconv-image
### ENTRYPOINT: elfconv/scripts/container-entry-point.sh
### running build and test ...
# You can test elfconv using `bin/elfconv.sh`
~/elfconv# cd bin
~/elfconv/bin# TARGET=wasm-browser ./elfconv.sh /path/to/ELF # e.g. ../examples/eratosthenes_sieve/a.out
~/elfconv/bin# emrun --no_browser --port 8080 exe.wasm.html
Web server root directory: /root/elfconv/bin
Now listening at http://0.0.0.0:8080/
```
Now, the WASM application server has started, so that you can access it (e.g. http://localhost:8080/exe.wasm.html) from outside the container.
### Host (WASI runtimes)
```bash
$ git clone --recursive https://github.com/yomaytk/elfconv
$ cd elfconv
$ docker build . -t elfconv-image
$ docker run -it --name elfconv-container elfconv-image
### ENTRYPOINT: elfconv/scripts/container-entry-point.sh
### running build and test ...
# You can test elfconv using `bin/elfconv.sh`
~/elfconv# cd bin
~/elfconv/bin# TARGET=wasm-host ./elfconv.sh /path/to/ELF # e.g. ../examples/eratosthenes_sieve/a.out
~/elfconv/bin# wasmedge exe.wasm # wasmedge is preinstalled
```
## Source code build
### Dependencies
The libraries required for the build are almost the same as those for remill, and the main libraries are as follows. The other required libraries are automatically installed using [cxx-common](https://github.com/lifting-bits/cxx-common).

| Name | Version |
| ---- | ------- |
| [Git](https://git-scm.com/) | Latest |
| [CMake](https://cmake.org/) | 3.14+ |
| [Google Flags](https://github.com/google/glog) | Latest |
| [Google Log](https://github.com/google/glog) | Latest |
| [Google Test](https://github.com/google/googletest) | Latest |
| [LLVM](http://llvm.org/) | 16 |
| [Clang](http://clang.llvm.org/) | 16+ |
| [Intel XED](https://software.intel.com/en-us/articles/xed-x86-encoder-decoder-software-library) | Latest |
| Unzip | Latest |
| [ccache](https://ccache.dev/) | Latest |

### Build
If you prepare these libraries, you can easily build elfconv by executing [`scripts/build.sh`](https://github.com/yomaytk/elfconv/blob/main/scripts/build.sh) as follows.
```bash
$ git clone --recursive https://github.com/yomaytk/elfconv
$ cd elfconv
/elfconv$ ./scripts/build.sh
```
After finishing the build, you can find the directory `elfconv/build`, and you can build the *'lifter'* (these source codes are mainly located in the [`backend/remill/`](https://github.com/yomaytk/elfconv/tree/main/backend/remill) and [`lifter/`](https://github.com/yomaytk/elfconv/tree/main/lifter)) by *ninja* after modifying the *'lifter'* codes.

After that, you can compile the ELF binary to the WASM binary using [`scripts/dev.sh`](https://github.com/yomaytk/elfconv/blob/main/scripts/dev.sh) as follows. `dev.sh` execute the translation and compiles the [`runtime/`](https://github.com/yomaytk/elfconv/tree/main/runtime) (statically linked with generated LLVM bitcode) and generate the WASM binary. when you execute the script, you should explicitly specify the path of the elfconv directory (`/root/elfconv` on the container) with `NEW_ROOT` or rewrite the `ROOT_DIR` in `dev.sh`. 
```bash
### Browser
~/elfconv/build# NEW_ROOT=/path/to/elfconv TARGET=wasm-browser ../scripts/dev.sh path/to/ELF # generate the WASM binary under the elfconv/build/lifter
~/elfconv/build# emrun --no_browser --port 8080 ./lifter/exe.wasm.html # execute the generated WASM binary with emscripten
------------------------
### Host (WASI Runtimes)
~/elfconv/build# NEW_ROOT=/path/to/elfconv WASISDK=1 TARGET=wasm-host ../scripts/dev.sh path/to/ELF
~/elfconv/build# wasmedge ./lifter/exe.wasm
```
### Common build issues

## Acknowledgement
elfconv uses or references some projects as following. Great thanks to its all developers!
- remill ([Apache Lisence 2.0](https://github.com/lifting-bits/remill/blob/master/LICENSE))
    - Original Source: https://github.com/lifting-bits/remill
    - elfconv uses remill in order to convert machine codes to LLVM IR instructions. the source code is contained in [`./backend/remill`](https://github.com/yomaytk/elfconv/tree/main/backend/remill) and is modified for using from front-end and supporting additional instructions.
- Sleigh Library ([Apache Lisence 2.0](https://github.com/lifting-bits/sleigh/blob/master/LICENSE))
    - Original Source: https://github.com/lifting-bits/sleigh
    - sleigh is a language to describe the semantics of instructions, and this library is part of the [Ghidra reverse engineering platform](https://github.com/NationalSecurityAgency/ghidra) and underpins its disassemler and decompilation engines.
- MyAOT ([Apache Lisence 2.0](https://github.com/AkihiroSuda/myaot/blob/master/LICENSE))
    - Original Source: https://github.com/AkihiroSuda/myaot
    - An experimental AOT-ish compiler (Linux/riscv32 ELF → Linux/x86_64 ELF, Mach-O, WASM, ...)
    - We refrenced the design of MyAOT for developing elfconv.
