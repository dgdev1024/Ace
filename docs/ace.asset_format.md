# `.ace` - ACE Engine Asset File Format

Below is the specification defining the binary file format used by the Ace
Engine for storing assets used by its client applications.

## File Format

Below is a table detailing how the Ace Asset File Format is structured. All
integer values are stored in little-endian byte order.

| Address       | Size      | Description                                           |
|---------------|-----------|-------------------------------------------------------|
| `$00000000`   | 4 Bytes   | Magic Number, always `$ACEA55E7`                      |
| `$00000004`   | 1 Byte    | Engine Major Version                                  |
| `$00000005`   | 1 Byte    | Engine Minor Version                                  |
| `$00000006`   | 2 Bytes   | Engine Revision                                       |
| `$00000008`   | 8 Bytes   | Asset Count                                           |
| `$00000010`   | 8 Bytes   | Total Compressed Asset Size, in Bytes                 |
| `$00000018`   | 8 Bytes   | Expected Decompressed Asset Size, in Bytes            |
| `$00000020`   | 8 Bytes   | Starting Offset of Asset Data                         |
| `$00000028`   | Variable  | Asset Information Table                               |
| Variable      | Variable  | Asset Data                                            |

## Asset Information Table

The Ace Asset File's Asset Information Table begins at offset `$00000014`, and
contains information about each asset contained in the file. An asset's
information structure is laid out as follows:

| Offset    | Size      | Description                                               |
|-----------|-----------|-----------------------------------------------------------|
| `$0000`   | 8 Bytes   | The starting offset of the asset's compressed data.       |
| `$0008`   | 8 Bytes   | The expected compressed size of the asset, in bytes.      |
| `$0010`   | 8 Bytes   | The expected decompressed size of the asset, in bytes.    |
| `$0018`   | 8 Bytes   | A checksum of the asset's data.                           |
| `$0020`   | 224 Bytes | The asset's name string; must be null-terminated.         |

The asset data's starting offset is relative to the start of the asset file's
asset data section.
