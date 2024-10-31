# SymbolicCalculator

This program is the second version of a program I made a few years ago during covid. I am going back and redoing the script with better programming practices.  The program is split up into the following sections:

## Parser
The inp7ut string is taken within the main function and passed to `tokenize.cpp`. This parses the input string into tokens defined by `token.hpp`. It also uses a multi-way trie to lookup functions in a defined lookup table

## Modified Shunting Yard Algorithm
The parsed output is passed in a token container and passed to `postfix.cpp`, where I have implemented a shunting yard algorithm that I have adapted to take in functions and variables.

## Expression Tree
The postfixed expression is then formed into a binary expression tree (`expression_tree.cpp`) of expression nodes (`expression_nodes.cpp`). This makes the expression easier to traverse and modify.

## Derivative Calculator
The derivative is taken from the expression tree, using an [automatic differentiation](https://en.wikipedia.org/wiki/Automatic_differentiation) algorithm. If steps are requested, we use derivative rules taught in calc 1 as well as a generalized power rule to create a new tree. 