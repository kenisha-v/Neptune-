
## LEXER Documentation

### Introduction

This documentation is for the LEXER part of the project, which is in charge of reading text input and transforming it into a sequence of tokens that can be processed by the PARSER, or printed onto the standard output.

### Files

1. *lex.h* includes:
    - `TokenType`: An enum representing different types of tokens, that helps the parser categorize tokens.
    - `token`: A structure to represent individual tokens including {line, column, token itself, & type of the token}
    - `SyntaxError` {contains location (line# & column#) of the error.}
    - declaration of `tokenize`

2. *lex.cpp* includes:
    - definition of `tokenize`
    - definition of the `print` function, used in the main function of this code
    - main: takes input and prints a table of tokens with their location, or gives a SyntaxError with location (if any).

### Usage of tokenize fn
- arguments: Input string
- possible returns:  
    1. vector of tokens
    2. Syntax Error


### Checkpoint 2 Changes
1. Added new type of token -
    - 'Variables' - handles identifiers that begin with alphabets or underscore, and can have numbers in them

2. '=' is added in the list of operators

3. It can now handle multiple lines of inputs and create token vectors for them.

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



1. parse.h includes:

    - `Node`: A structure to represent individual nodes in the AST which contains its value and a vector of the nodes of its children.

    - `AST`: A class containing member variables and functions to create an abstract syntax tree, evaluating and printing it.

    - `ParseError` {contains location (line# & column#) of the unexpected token it encountered.}

    -`EvaluationError` {contains information of errors encountered during the evaluation of the AST, like runtime error, etc.}



2. parse.cpp includes:

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


### Checkpoint 2 Changes
Parser now handles assignment between identifies and numbers.
1. '=' is now handled by the AST and can have multiple childrens.

2. Handling errors caused by assignment, i.e., there can be multiple identifiers but there should be 1 AND ONLY 1 numeric value. 

3. During evaluation the identifiers are replaced with their numeric values.

4. It can now handle multiple lines of inputs and create multiple AST.

### Handling Errors



Error’s handled are of 3 types which are taken care of by using try-catch blocks:
1. Syntax error: which is received from the LEXER.
2. Parse error: when creating the AST, parse error is encountered when it comes across an unexpected token, like END, where it is not supposed to be.
3. Evaluation error: this error arises when the tree is being evaluated and it faces a runtime error, like division by 0.



### Conclusion

The PARSER serves as the final step in creating and evaluating syntax tree’s for our final aim Calculator.

It ensures that there are no error while creating the tree or while evaluating it when converting it form a prefix to an infix expression .

---

## CALC Documentation

### Introduction



This documentation is for the CALCULATOR part of the project, which takes a sequence of tokens in infix_expression from the LEXER, converts the sequence into an AST, and then evaluates it from the tree.



### Files

*calc.cpp*:

- This program reads standard input one line at a time where each line contains exactly one infix expression.
- After reading a line, this program lexes it into tokens.
- It then parses those tokens into an AST.
- It prints the AST in infix form  (prints parentheses around every operation).
- It then evaluates the AST and prints the result.
- When it reaches the end of its input, it exits with exit code zero.



### Handling Errors

this program recognizes all of the errors described in the s-expression parser, 
but because it runs an interactive loop: it does not exit when hitting an error. Instead, it prints the error message and then waits for another line of user input.

- For lexer and parser errors, only the error message is printed.
- For runtime errors, the AST is printed in infix form before the error is printed.
- In the case of a runtime error, no variables are updated.


### Conclusion

This parser takes in input in the form of infix expressions instead of s-expressions.
It serves as an easier version of the calculator for the normal public as not everyone is familiar with S-expressions.

---
### Error file

*errors.h*: It acts as a unified file for all errors, for readability.
Includes: SyntaxError, ParseError, EvaluationError.

---

## Statments Documentation

### Introduction

This documentation is for the format part of the project in which it can read in its entire input and build ans AST tree and then print it.

### Files


1. Stree.hpp includes:

    -`SNode`: The base class for a Symbol Tree node. It has an AST expression and a pointer to the next node. It allows the node to be evaluated and printed.

    - `ExpressionNode`: This class is derived from `SNode` and represents an expression node, as well as implementing particular evaluation and printing logic.

    - `WhileNode`: A specialized `SNode` representing a `while` loop construct. It contains logic for evaluating the loop and a branch to execute while the condition is true.

    - `PrintNode`: Represents a `print` statement within the tree. 

    - `IfNode`:  Represents a `if` statement node with true and false branches.

    - `STree`: This is the main class that represents the Tree. It keeps track of a list of nodes (`SNode` objects), parses token blocks into the tree structure, and offers methods for evaluating and printing the tree.



2. Stree.cpp includes:
    - **Base Node (`SNode`)**: Constructs a node with an AST pointer and a following node. The destructor guarantees that all associated memory is freed. 

    - **Expression Node (`ExpressionNode`)**: Extends `SNode` to support expression evaluation. It invokes the 'evaluate' method on its AST and iterates to the next accessible node.

    - **While Node (`WhileNode`)**: A while loop construct is represented by this node. It examines its condition and body, which are captured as an AST and a Symbol Tree, until the condition is false.

    - **Print Node (`PrintNode`)**:  Handles print statements by evaluating expressions and reporting the results. It can evaluate boolean and double values with particular output formatting.

    - **If Node (`IfNode`)**: Manages conditional structures, deciding between two branches of execution based on the evaluation of its conditional expression.

    - **Symbol Tree (`STree`)**: Orchestrates the overall structure, providing the functionality to parse a block of tokens into a tree of nodes and to evaluate the entire tree. It includes a destructor that cleans up the tree to prevent memory leaks.
 


3.  format.cpp includes:

    - A main function: The program reads an entire file character by character into a string.
    -  It tokenizes the input string into a sequence of tokens.
    -  STree is then built using the tokens.
    -  The Stree is then printed keeping the indentation in mind.

4. scrypt.cpp includes:

    -The program reads input from the standard input stream (stdin) one character at a time until the end-of-file (EOF) is reached.
    - A sequence of tokens is then created.
    - These tokens are subsequently parsed to construct a Symbol Tree (STree), which represents the structured form of the input expressions.
    - The STree is then evaluated, and the result of this evaluation is printed to the standard output.

### Conclusion
 The program read in its entire input, build an AST of the program, and then print it. Then it evalutes the tree. Now the tree also supports statements like `if`, `while` and `print`

*errors.h*: It acts as a unified file for all errors, for readability.
Includes: SyntaxError, ParseError, EvaluationError.

