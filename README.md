# CHIP-8 Disassembler

A CHIP-8 disassembler I'm building from scratch in C++.

## What it does

It takes a CHIP-8 ROM and turns the raw bytes back into readable instructions.

So this:

```
00 E0 A2 2A 60 0C 61 08 D0 1F
```

Becomes this:

```
0x200: 00E0  CLS
0x202: A22A  LD I, 0x22A
0x204: 600C  LD V0, 0x0C
0x206: 6108  LD V1, 0x08
0x208: D01F  DRW V0, V1, 15
```

## Why I'm doing this

I'm building a [CHIP-8 emulator](https://github.com/GrantBaum/chip8) and wanted to see what the ROMs are actually doing. A disassembler lets me read any game line by line. It's also a great debugging tool. When a test fails in my emulator I can look at the exact instructions around it.

It reuses the decode logic from the emulator. The emulator decodes an instruction and runs it. This decodes it and prints it.

The plan is to build an assembler next. Then I can write my own programs, assemble them, disassemble the output and check I get the same code back.

## Status

Work in progress.

- [ ] Read a ROM file
- [ ] Print each instruction with its address
- [ ] Decode all 35 instructions
- [ ] Print unknown bytes as raw data
- [ ] Save output to a file

## Building

You need g++. On Ubuntu or WSL:

```
sudo apt install build-essential
```

Then:

```
make
```

## Running

```
./disasm path/to/rom.ch8
```

## A heads up

ROMs mix code and data. Sprites are just bytes too, so this will sometimes "decode" a sprite into nonsense instructions. That's normal for a simple disassembler. Anything that doesn't match a real instruction prints as `DB` (raw data).

## Instruction names

Uses the standard names from Cowgod's reference.

| Opcode | Prints as |
|--------|-----------|
| `00E0` | `CLS` |
| `00EE` | `RET` |
| `1NNN` | `JP NNN` |
| `2NNN` | `CALL NNN` |
| `3XNN` | `SE VX, NN` |
| `4XNN` | `SNE VX, NN` |
| `5XY0` | `SE VX, VY` |
| `6XNN` | `LD VX, NN` |
| `7XNN` | `ADD VX, NN` |
| `8XY0` | `LD VX, VY` |
| `8XY1` | `OR VX, VY` |
| `8XY2` | `AND VX, VY` |
| `8XY3` | `XOR VX, VY` |
| `8XY4` | `ADD VX, VY` |
| `8XY5` | `SUB VX, VY` |
| `8XY6` | `SHR VX` |
| `8XY7` | `SUBN VX, VY` |
| `8XYE` | `SHL VX` |
| `9XY0` | `SNE VX, VY` |
| `ANNN` | `LD I, NNN` |
| `BNNN` | `JP V0, NNN` |
| `CXNN` | `RND VX, NN` |
| `DXYN` | `DRW VX, VY, N` |
| `EX9E` | `SKP VX` |
| `EXA1` | `SKNP VX` |
| `FX07` | `LD VX, DT` |
| `FX0A` | `LD VX, K` |
| `FX15` | `LD DT, VX` |
| `FX18` | `LD ST, VX` |
| `FX1E` | `ADD I, VX` |
| `FX29` | `LD F, VX` |
| `FX33` | `LD B, VX` |
| `FX55` | `LD [I], VX` |
| `FX65` | `LD VX, [I]` |

## Resources

- [Cowgod's Chip-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [Guide to making a CHIP-8 emulator](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/) by Tobias V. Langhoff
- [My CHIP-8 emulator](https://github.com/GrantBaum/chip8)
