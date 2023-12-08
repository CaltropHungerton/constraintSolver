# constraintSolver
i made a constraint solver in c++! right now it solves sudokus, but it is easy to expand.

to compile:

g++ constraintsatbase.cc sudoku.cc -o sudoku

to extend the solver for other problems:

-include the base header file

-create classes for each of your constraints inheriting from "Constraint". They should implement "isSatisfied"

-create a solver class that inherits from CSP. Implement "prettyprint", "load" (if necessary), and "impinges".

in main: create the solver object, set up the problem with load(), and call backtrackBegin()!

Happy NP HARD solving!
