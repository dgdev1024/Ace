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
- Non-range-based `for` loop initializers: single lower-case letter (eg. `i`, `j`)

- Augmented Standard Namespace: `astd`
- Everything therein: `snake_case`

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

## Non-Range-Based For Loops

```c++
for (std::size_t i = 0; i < 10; ++i)
{
    for (std::size_t j = 0; j < 10; ++i)
    {
        
    }
}
```