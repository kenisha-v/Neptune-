
## LEXER Documentation

### Introduction

This documentation is for the LEXER part of the project, which is in charge of reading text input and transforming it into a sequence of tokens that can be processed by the PARSER, or printed onto the standard output.

### Files

1. **lex.h** includes:
    - `TokenType`: An enum representing different types of tokens, that helps the parser categorize tokens.
    - `token`: A structure to represent individual tokens including {line, column, token itself, & type of the token}
    - `SyntaxError` {contains location (line# & column#) of the error.}
    - declaration of `tokenize`

2. **lex.cpp** includes:
    - definition of `tokenize`
    - definition of the `print` function, used in the main function of this code
    - main: takes input and prints a table of tokens with their location, or gives a SyntaxError with location (if any).

### Usage of tokenize fn
- arguments: Input string
- possible returns:  
    1. vector of tokens
    2. Syntax Error

### Handling Errors

When using the `tokenize` function, it is important to call it in a try-catch block, because it throws a 'SyntaxError' when encountering an invalid character.
Otherwise, the program might terminate as the error was not caught.

### Conclusion

The LEXER serves as the first step in processing and understanding string inputs for our final aim Calculator.
It ensures that only valid characters are given to the parser in an easily readable format.

---

## PARSER Documentation

_NOTE TO PARSER TEAM: Please include your README documentation here._
