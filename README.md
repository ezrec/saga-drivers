SAGA Chipset Drivers (for AmigaOS 3.x)
=====================================

| Directory | Feature                                            |
| --------- | -------------------------------------------------- |
| saga.card | Picasso96 plugin for AmigaOS and AROS              |

## saga.card

* Supports 320x240 and 640x480 at 60Hz
* Can display in 8 bit palette mode, 15 bit hicolor, 16 bit hicolor,
  24-bit RGB, and 32-bit RGB.
* Works with both Picasso96 for AmigaOS, or with AROS

### Compilation

This driver is designed to be compiled via the AROS ABIv1 m68k crosscompiler.

The `saga-card` __metamake__ target will build the ELF file into your AROS
target directory, as `AROS:Libs/Picasso96/saga.card`

Use the AROS `elf2hunk` tool to convert the ELF to AmigaOS HUNK format, to
use it on an AmigaOS system.

### Installation (AmigaOS)

* Install Picasso96. Select _only_ the `uaegfx` driver during installation.
* Rename `SYS:Devs/Monitors/uaefgx` to `SYS:Devs/Monitors/SAGA` in __Workbench__
* Modify the `SAGA` icon's tooltypes, and set `BoardType=saga`
* Copy `saga.card` to `SYS:Libs/Picasso96/saga.card`
* Reboot, and use `SYS:Prefs/Picasso96Mode` to attach the SAGA board to the modes
* Reboot, and use `SYS:Prefs/ScreenMode` to select the SAGA screen mode

## sagasd.card

* Supports SD and SDHC devices

### Compilation

This driver is designed to be compiled via the AROS ABIv1 m68k crosscompiler.

The `saga-sd` __metamake__ target will build the ELF file into your AROS
target directory, as `AROS:Devs/sagasd.device

Use the AROS `elf2hunk` tool to convert the ELF to AmigaOS HUNK format, to
use it on an AmigaOS system.

### Installation (AmigaOS)

* Copy the HUNK version of `sagasd.device` to DEVS:
* Install 'GiggleDisk' from AmiNet for automatic partition support
* Create a `C:IconX` script with the following content:

```
; Semi-automatic mounting for SAGA SD devices
; Requires 'GiggleDisk' from AmiNet to be in your
; path - see http://aminet.net/package/disk/misc/giggledisk.lha
;
If EXISTS T:SDMount
    Echo "SD has already been mounted:"
    Echo ""
    Echo "Partitions"
    Echo "----------"
    Cd T:SDMount
    List LFORMAT="    %s:" ~(#?.info)
    Echo ""
    Ask "[EXIT]"
Else
    MakeDir T:SDMount
    GiggleDisk DEVICE=sagasd.device UNIT=0 PREFIX=SD TO=T:SDMount
    Mount >NIL: T:SDMount/~(#?.info)
EndIf
```
