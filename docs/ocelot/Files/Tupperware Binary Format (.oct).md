The tupperware file format stores bulk data on how other files and parameters are structured. You will find things like material parameters, object information, world space transforms and parent references, texture assignment details, etc.

# Oct header
---
The header section is the first section in the oct file, spanning 60 bytes long

| Name                | Type      | Size         | Value                          |
| :------------------ | :-------- | :----------- | ------------------------------ |
| `magic`             | `char[4]` | 4 bytes      | LE: `0x45017629`               |
|                     |           |              | BE: `0x29760145`               |
| `version`           | `float`   | 4 bytes      | Typically `1.1`                |
| `padding`           | `u32`     | 4 bytes      | Unused                         |
| `stringTableLength` | `u32`     | 4 bytes      | Size of string table in bytes  |
| `atomLength`        | `u32`     | 4 bytes      | Size of the atom tree in bytes |
| `reserved`          | `u32[10]` | `4*10` bytes | Reserved space, unused         |

## Magic

The magic section validates the oct file is valid, and can also serve as a way to identify the endianness of the oct file. There are two possible values for the magic section:

- Little endian: `0x45017629`
- Big endian: `0x29760145`

If the oct files header section loads as the big endian value, the oct file is in big endian and needs to be converted as you load the values of the file.

## Version

For a long while we thought this field was part of the [[#Magic]] section, making the magic 8 bytes long, but from years of studying the IEEE-754 floating point specification, I couldn't help but see a float. For some wack ass reason what seems to be the file version number is stored as a floating point number. World oct files seem to have a version of `1.1` or `0x3F8CCCCD` in little endian memory. This version number is the only one I know of so far.

## String Table Size

Represents the length of the string table (by itself) in bytes. This value is used to know when to stop reading strings from the oct file since the string table has no element counter. This values usage will be brought up later.

## Atom Length

Represents the length of the atom tree (by itself) in bytes. The atom tree is yet another region without an element counter and instead relies on this value to know when to stop reading. This values usage will be brought up later.

## Reserved

40 Bytes are reserved for what's most likely padding or future proofing extensibility of the header section. It was most likely a fail safe region for when the developers wanted to extend the header section, but for our cases its contents are 40 bytes of zeros.

# String Table
---
The string table is an array of C style null terminated strings, stored directly after the [[#Oct header]] segment. The buffer range will be from byte 60 to (60 + [[#String Table Size]]).
Since the string table represents characters (`char` values), no endian conversion is needed to process the string table.

> [!NOTE]
> The first string in the table will **always** be an empty string
> the last string will typically store the oct files export time in 24 hour format.

These string entries are used to identify types and properties stored within the atom tree. The string table is indexed using `u16` values meaning there's an implicit max string table size of 65535 elements. The string elements don't *seem* to have any specific order to them, and adding strings when they show up is most likely the original specification.

# Atom Data
---
The atom tree is stored directly after the [[#String Table]], ranging from
(60 + [[#String Table Size]]) to (60 + [[#String Table Size]] + [[#Atom Length]]) 

Atoms are generic data containers with variable byte sizes. Atoms can store
- individual values or entries ("Scalars")
- Arrays of values or entries ("Lists")
- Child atoms in a hierarchy ("Aggregate")

The size of each atom is defined in its 4 byte header segment from below.

## Atom Header
| Name     | Type  | Value              | Description                                 |
| :------- | ----- | ------------------ | ------------------------------------------- |
| flags    | `u16` | See below          | Defines the atom properties                 |
| nodeType | `u16` | String table index | String table index of the nodes type string |

## flags

|  15 \| 6 bits   |   2 bits   |    2 bits     |  3 bits   |  1 bit   | 2 bits \| 0 |
| :-------------: | :--------: | :-----------: | :-------: | :------: | :---------: |
| Hierarchy Depth | Int Stride | Length Stride | Data Type | Has Name |  Atom Type  |
### Atom Type

Atom type is 2 bits from \[0:1\] indicating the type of the atom. Aggregate being a container, List being a list of values/elements, Scalar being a single element *not restricted to scalars as the typical numeric definition.*

| Type  | Unknown  | Aggregate |   List   |  Scalar  |
| :---: | :------: | :-------: | :------: | :------: |
| Value | `00 (0)` | `01 (1)`  | `10 (2)` | `11 (3)` |
### Has Name

Has name is a boolean stored as a single bit \[2:2\]. If the bit is a 1, the atom has a name secondary to the atom name. Atoms representing elements of an aggregate atom typically come with a name that is equal to their relative index in the array, i.e. the third child of an aggregate atom often has a name pointing to "2" in the string table. (0 base example)

When the boolean is true, two additional bytes will come after the header segment representing an index into the string table `u16`.

### Data Type

The data type is 3 bits from \[3:5\] Representing the element type for Scalar and List atoms. Binary is only used with scalar atoms.

| Type  |  Unknown  |  String   |   Float   |  Integer  |  Binary   |
| :---: | :-------: | :-------: | :-------: | :-------: | :-------: |
| Value | `000 (0)` | `001 (1)` | `010 (2)` | `011 (3)` | `100 (4)` |

Combining the [[#Atom Type]] and [[#Data Type]] sections gives us a table of possible elements:

| Data Type       | Atom Type        | Definition |           Description            |
| :-------------- | :--------------- | :--------: | :------------------------------: |
| `000 (Unknown)` | `01 (Aggregate)` |  `atom[]`  |      A list of child atoms       |
| `001 (String)`  | `11 (Scalar)`    |  `string`  |   A single string table index    |
| `001 (String)`  | `10 (List)`      | `string[]` |  A list of string table indexes  |
| `010 (Float)`   | `11 (Scalar)`    |  `float`   |  A single floating point number  |
| `010 (Float)`   | `10 (List)`      | `float[]`  | A list of floating point numbers |
| `011 (Integer)` | `11 (Scalar)`    |   `int`    |   A single **signed** integer    |
| `011 (Integer)` | `10 (List)`      |  `int[]`   |  A list of **signed** integers   |
| `100 (Binary)`  | `11 (Scalar)`    |   `u8[]`   |         Raw binary data          |
> [!NOTE]
> String values are stored as `u16` values, and string lists are stored as `u16` arrays
### Length Stride

The Length stride part is 2 bits from \[6:7\] and only used for List/Scalar atom types. Its value represents the *stride* of a list atoms length term following after the header (or after the name key if [[#Has Name]] is present) Its value represents the stride in bytes from one byte to four.

| Value  |  `00`  |  `01`   |  `10`   |  `11`   |
| :----- | :----: | :-----: | :-----: | :-----: |
| Stride | 1 byte | 2 bytes | 3 bytes | 4 bytes |
- 1 byte: 0-255 elements
- 2 bytes: 0-65535 elements
- 3 bytes: 0-16777215 elements
- 4 bytes: 0-4294967295 elements
The value is evaluated as $\Large\lceil\log_{256}(\text{element count})\rceil$ or $\Large\frac{\lceil\log_2(\text{element count})\rceil}{8}$
TL;DR: The stride is just how many bytes are needed to store the array length.

### Int Stride

The integer stride term is 2 bits from \[8:9\] and is quite similar to the [[#Length Stride]] term. When packing integer values, this field determines the byte stride of the integer (for a Scalar atom) or for each integer element (for a List atom.) This value will show up after the [[#Length Stride]] term if its present.

| Value  |  `00`  |  `01`   |  `10`   |  `11`   |
| :----- | :----: | :-----: | :-----: | :-----: |
| Stride | 1 byte | 2 bytes | 3 bytes | 4 bytes |
- 1 byte: `-128` to `+127`
- 2 bytes: `-32768` to `+32767`
- 3 bytes: `-8388608` to `+8388607`
- 4 bytes: `-2147483648` to `+2147483647`

Each integer element in the list will have as many bytes as the integer stride term specifies.

### Hierarchy Depth

The last term is 6 bits wide from \[10:15\] representing the hierarchy level of the atom, with 0 being the root level. If the depth value of the next atom is greater than the current atoms depth value, it's to be considered a child of the current atom. The indentation level is 6 bits wide which implies there's a max hierarchy depth of 63 levels (which is honestly *far less* than they would ever use.)

## Node Type

The node type is a string index `u16` value which is the atoms identifier index into the string table. The key index is *not to be confused with the name key, which is dependent on [[#Has Name]]*  This string identifies the type of the atom itself.

## Possible Atom Layouts

Below is a table of possible combinations from the atom tree system.

| Field                 |    Size     | Used ...                                                 |
| :-------------------- | :---------: | -------------------------------------------------------- |
| m_flags               |   2 Bytes   | Always                                                   |
| m_keyIndex            |   2 Bytes   | Always                                                   |
| Name Key              |   2 Bytes   | Only if [[#Has Name]] is `1`                             |
| List Length           |  1-4 Bytes  | For `Lists`, `Strings`, `Binary Scalars`                 |
| Element Stride        |  1-4 Bytes  | For `Integers` and `List Integers`                       |
| (Data)                |  \* Bytes   | The stored data                                          |
