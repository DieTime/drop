<p align="center">
    <img src="assets/icon.svg" width="130px" />
</p>

<p align="center">
    Drop anything in the trash!
</p>

<p align="center">
   <img
      src="https://github.com/DieTime/drop/actions/workflows/build.yaml/badge.svg?color=9676F2&label=Linux%20build"
      alt="Linux build"
   />
   <img
      src="https://img.shields.io/github/license/DieTime/drop?color=9676F2&label=License"
      alt="License type"
   />
</p>

## Introduction

If sometimes you want to remove something to the trash from the terminal, you're at the right place. The `drop` utility will give you such an opportunity!

## Supported platforms

This code supports only Linux-based operating systems.

## Installation

```bash
$ git clone https://github.com/DieTime/drop.git
$ cd drop
$ meson setup build
$ meson compile -C build
$ meson install -C build
```

## Usage

To drop some file or directory in the trash just run the following command:

```bash
$ drop <PATH>
```

You can also drop multiple files or directories in the trash:

```bash
$ drop <PATH> [PATH...]
```

## License

This code is distributed under the [MIT](./LICENSE) license.
