# Ace Engine Code Style Guide

- Module Folder Names: `PascalCase`
- Header Filenames: `PascalCase.hpp`
- Inline Header Filenames: `PascalCase.inl`
- Implementation Filenames: `PascalCase.cpp`
- Root Namespace: `ace`
- Inner Namespaces, Classes, Structs, Enumerators, Functions, Methods: `PascalCase`
- Interface (Pure Virtual) Classes/Structures: `IPascalCase`
- Constants, Static Constants, Enumerations: `SCREAMING_SNAKE_CASE`
- Function Parameters: `pPascalCase`
- Member Variables: `mPascalCase`
- Local Variables: `lPascalCase`
- Static Members/Variables: `sPascalCase`

## Functions

```c++
void functionWithNoArgs ();

void functionWithArg (
    const std::string& pArg
);

void functionWithMultipleArgs (
    const std::string& pName,
    const std::uint8_t& pAge
);
```