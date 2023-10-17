
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

### Introduction



This documentation is for the PARSER part of the project, which is in charge of taking a sequence of tokens from the LEXER, converting the sequence into an AST and then evaluating it by creating an infix-expression from the tree.



### Files



1. *parse.h* includes:

    - `Node`: A structure to represent individual nodes in the AST which contains its value and a vector of the nodes of its children.

    - `AST`: A class containing member variables and functions to create an abstract syntax tree, evaluating and printing it.

    - `ParseError` {contains location (line# & column#) of the unexpected token it encountered.}

    -`EvaluationError` {contains information of errors encountered during the evaluation of the AST, like runtime error, etc.}



2. *parse.cpp* includes:

    - Constructors and destructors of the Node structure and the AST class.

    - Definition of the functions used to create an AST, print and evaluate it. namely:  	1. printAST
	2. evaluate
	
    - main: takes input, sends it to the LEXER to create a sequence from it, and prints the AST with the evaluated output, or exits with an error.



### Usage of printAST fn

- arguments: Node

- usage: prints an infix expression

- possible returns:  

    1. void



### Usage of evaluate fn

- arguments: Node

- usage: calculates the result of the equation

- possible returns:  

    1. double
    2. Evaluation error



### Handling Errors



Error’s handled are of 3 types which are taken care of by using try-catch blocks:
1. Syntax error: which is received from the LEXER.
2. Parse error: when creating the AST, parse error is encountered when it comes across an unexpected token, like END, where it is not supposed to be.
3. Evaluation error: this error arises when the tree is being evaluated and it faces a runtime error, like division by 0.



### Conclusion



The PARSER serves as the final step in creating and evaluating syntax tree’s for our final aim Calculator.

It ensures that there are no error while creating the tree or while evaluating it when converting it form a prefix to an infix expression .
