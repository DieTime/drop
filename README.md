<p align="center">
    <img src="assets/icon.svg" width="130px" />
</p>

<p align="center">
    Drop anything in the trash!
</p>

<p align="center">
   <img
      src="https://img.shields.io/github/actions/workflow/status/DieTime/drop/.github%2Fworkflows%2Fbuild.yaml?label=Linux%20build&color=9676F2"
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

To drop some file or directory in the trash just run the following commandю

```bash
$ drop icon.png
```

You can use the `--verbose` option to enable more detailed messages.

```bash
$ drop --verbose icon.svg

👀 Verbose! Attempting to select a file name for the trash
   ↳ icon.svg (drop-uuid=2721ebaf-f835-4a08-b604-e0c624609abc)
👀 Verbose! Trash info successfully created
   ↳ /home/glazzkoff/.local/share/Trash/info/icon.svg (drop-uuid=2721ebaf-f835-4a08-b604-e0c624609abc).trashinfo
👀 Verbose! File successfully copied to the trash
   ↳ /home/glazzkoff/.local/share/Trash/files/icon.svg (drop-uuid=2721ebaf-f835-4a08-b604-e0c624609abc)
👀 Verbose! File successfully removed
   ↳ icon.svg
```

## License

This code is distributed under the [MIT](./LICENSE) license.
