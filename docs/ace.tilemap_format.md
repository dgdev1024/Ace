# `.atm` - Ace Engine Tilemap Format

Below is a specification for the binary tilemap format used by the Ace Engine
and its client applications.

## File Format

Below is a table detailing how the Ace Asset File Format is structured. All
integer values are stored in little-endian byte order.

| Address       | Size      | Description                                           |
|---------------|-----------|-------------------------------------------------------|
| `$00000000`   | 4 Bytes   | Magic Number, always `$ACE713A9`                      |
| `$00000004`   | 1 Byte    | Engine Major Version                                  |
| `$00000005`   | 1 Byte    | Engine Minor Version                                  |
| `$00000006`   | 2 Bytes   | Engine Revision                                       |
| `$00000008`   | 4 Bytes   | Single Tile Width, In Pixels                          |
| `$0000000C`   | 4 Bytes   | Single Tile Height, In Pixels                         |
| `$00000010`   | 4 Bytes   | Tile Map Width, In Tile Widths                        |
| `$00000014`   | 4 Bytes   | Tile Map Height, In Tile Heights                      |
| `$00000018`   | Variable  | Tile Structure Definition, Null-Terminated            |

## Tile Structure

Each tile in an `.atm` tilemap file is a structure, whose members' types are
defined by a null-terminated sequence of bytes starting from offset `$00000018`.
The members' types are enumerated and defined as follows:

| Enumeration   | Type              | Size      | Description                                           |
|---------------|-------------------|-----------|-------------------------------------------------------|
| `0x00`        | Null              | N/A       | The tile structure definition's null terminator.      |
| `0x10`        | Float             | 4 Bytes   | A single-precision floating point number.             |
| `0x11`        | Double            | 8 Bytes   | A double-precision floating point number.             |
| `0x12`        | Int               | 4 Bytes   | A 32-bit signed integer.                              |
| `0x13`        | Unsigned Int      | 4 Bytes   | A 32-bit unsigned integer.                            |
| `0x14`        | Boolean           | 1 Byte    | A boolean value, `true` or `false`.                   |
| `0x20`        | 2 Floats          | 8 Bytes   | A 2D vector of Floats.                                |
| `0x21`        | 2 Doubles         | 16 Bytes  | A 2D vector of Doubles.                               |
| `0x22`        | 2 Ints            | 8 Bytes   | A 2D vector of Ints.                                  |
| `0x23`        | 2 Unsigned Ints   | 8 Bytes   | A 2D vector of Unsigned Ints.                         |
| `0x24`        | 2 Booleans        | 2 Bytes   | A 2D vector of Booleans.                              |
| `0x30`        | 3 Floats          | 12 Bytes  | A 3D vector of Floats.                                |
| `0x31`        | 3 Doubles         | 24 Bytes  | A 3D vector of Doubles.                               |
| `0x32`        | 3 Ints            | 12 Bytes  | A 3D vector of Ints.                                  |
| `0x33`        | 3 Unsigned Ints   | 12 Bytes  | A 3D vector of Unsigned Ints.                         |
| `0x34`        | 3 Booleans        | 3 Bytes   | A 3D vector of Booleans.                              |
| `0x40`        | 4 Floats          | 16 Bytes  | A 4D vector of Floats.                                |
| `0x41`        | 4 Doubles         | 32 Bytes  | A 4D vector of Doubles.                               |
| `0x42`        | 4 Ints            | 16 Bytes  | A 4D vector of Ints.                                  |
| `0x43`        | 4 Unsigned Ints   | 16 Bytes  | A 4D vector of Unsigned Ints.                         |
| `0x44`        | 4 Booleans        | 4 Bytes   | A 4D vector of Booleans.                              |
| `0xA0`        | 3x3 Float Matrix  | 36 Bytes  | A 3x3 Float Matrix (9 Floats)                         |
| `0xA1`        | 3x3 Double Matrix | 72 Bytes  | A 3x3 Double Matrix (9 Doubles)                       |
| `0xB0`        | 4x4 Float Matrix  | 64 Bytes  | A 4x4 Float Matrix (16 Floats)                        |
| `0xB1`        | 4x4 Double Matrix | 128 Bytes | A 4x4 Double Matrix (16 Doubles)                      |
