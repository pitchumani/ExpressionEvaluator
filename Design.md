# Problem: Expression Evaluator Framework

**Goal: Design a C++ framework to parse and evaluate arithmetic expressions like:**
```
(3 + 4) * (2 - 1) / 5 + sqrt(16)
```

**Requirements**
* Support binary operators: +, -, *, /
* Support unary functions like sqrt, abs, sin, cos
* Use a factory to instantiate different expression node types
* Design abstract classes/interfaces for expression nodes
* Make the evaluator extensible to add new operations easily

## Design
- cpp - console application
- read a line of string indefinitely until the input string is `quit`

parser
- input: string
- output: expression tree

IExpression
- evaluate() = 0

NumberNode
- IntegerNode, FloatNode
BinaryOpNode
- AddNode, SubNode, MulNode, DivNode
UnaryOpNode
- SqrtNode, AbsNode, SinNode, CosNode

ExpressionFactory
- IExpression *generateExpressionTree(string input)


expression
- node - abstract
  - evaluate() = 0;
- BinaryOp
- UnaryOp
- Number

Operator
- add
- sub
- mul
- bracket
- sqrt
- abs
- sin
- cos

