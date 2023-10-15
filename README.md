
# LEXER Documentation

## Introduction

This documentation pertains to the LEXER part of the project, which is responsible for tokenizing input strings. The LEXER breaks down input strings into meaningful tokens, which are then processed by the PARSER. This documentation does not cover the PARSER, and another team will provide details on that component.

## Files

1. **lex.h**: This header file provides definitions and declarations for the LEXER. 
    - `TokenType`: An enumeration representing different types of tokens.
    - `token`: A structure to represent individual tokens.
    - `SyntaxError`: An exception class to capture syntax errors during tokenization.
    - `tokenize`: A function to tokenize input strings.

2. **lex.cpp**: This is the implementation file for the LEXER.
    - Contains functions to determine token types, create tokens, and tokenize input strings.

## Usage

To use the LEXER:

1. Include "lib/lex.h".
2. Call the `tokenize` function with your input string.
3. The function will return a vector of tokens.

```cpp
#include "lib/lex.h"
...
std::vector<token> my_tokens = tokenize(my_input_string);
```

### Handling Errors

When using the `tokenize` function, it is essential to handle potential `SyntaxError` exceptions. If an invalid character is encountered during tokenization, a `SyntaxError` is thrown. The PARSER team should wrap calls to `tokenize` in a try-catch block and halt parsing if a lexer error is caught.

```cpp
try {
    std::vector<token> my_tokens = tokenize(my_input_string);
    // Continue with parsing
} catch (const SyntaxError& e) {
    std::cerr << e.what() << std::endl;
    // Halt parsing
}
```

## Conclusion

The LEXER serves as the first step in processing and understanding input strings. Proper error handling is crucial to ensure that only valid tokens proceed to the parsing stage.

---

# PARSER Documentation

**NOTE TO PARSER TEAM**: Please include your README documentation below this note.

