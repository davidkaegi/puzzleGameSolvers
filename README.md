# puzzleGameSolvers

These are some short C++ programs that can solve certain puzzle games

## Sudoku

The sudoku solver program takes in a file containing a sudoku puzzle and prints the solution to the puzzle. Attached in the sudoku folder are some example inputs that were used to test the program.

## Minesweeper

The minesweeper solver takes user input to determine its behavior.
<ul>
  <li>
    If the user enters "enter" or "update" followed by coordinates in the form "L/R, U/D", where (0,0) is the top left corner of the game board, followed by and number (0-8) or "f" (representing a flag), the board will change in that square to state specified by the user.</li>
  <li>
    If the user enters "solve", the program will generate a list of every square that is guaranteed to either have a mine or no mine. Note that flags placed by the user are assumed to cover mines, the program does not check for potential errors made by the user.
  </li>
  <li>
    If the user enters "print" the current game state will be printed.
  </li>
  <li>
    To terminate the program, the user can enter "quit".
  </li>
</ul>
