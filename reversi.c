//
// Author: Randy Ding
//

#include "reversi.h"
#include "stdio.h"


// finds if the coordinate is within the board
bool positionInBounds(int n, int row, int col)
{

  return row < n && row >= 0 && col < n && col >= 0;
}


// finds the opposite colour of input and if the input is invalid, returns null
char findOpposite(char colour)
{
  if (colour == 'B') return 'W'; 
  
  else return 'B';
}


// changes char coordinates into int
int pieceToNum(char i) 
{
  return i - 'a';
}


// prints board to terminal
void printBoard(char board[][26], int n)
{

  char letter;

  printf("  ");
  for (int i = 0; i < n; i++)
  {

    letter = 'a' + i;
    printf("%c", letter);
  }
  printf("\n");

  // iterates through the rows(up to down)
  for (int i = 0; i < n; i++)
  {

    letter = 'a' + i;
    printf("%c ", letter);

    // iterates through the columns(left to right)
    for (int j = 0; j < n; j++)
    {

      printf("%c", board[i][j]);
    }

    printf("\n");
  }
}


// set the board to a clean slate board
void initialSetup(char board[][26], int n)
{

  for (int i = 0; i < 26; i++)
  {
    for (int j = 0; j < 26; j++)
    {
      board[i][j] = 'U';
    }
  }


  // places initial pieces
  board[n / 2][n / 2] = 'W';
  board[n / 2 - 1][n / 2 - 1] = 'W';
  board[n / 2 - 1][n / 2] = 'B';
  board[n / 2][n / 2 - 1] = 'B';
}


void winCount(char board[][26], int n)
{
  int whiteCount = 0;
  int blackCount = 0;
  for(int i = 0; i < n; i++)
  {
    for(int j = 0; j < n; j++)
    {

      if(board[i][j] == 'W') whiteCount++;
      else if(board[i][j] == 'B') blackCount++;
    }

  }

  if( whiteCount == blackCount) printf("Draw!");
  else if( whiteCount > blackCount) printf("W player wins.");
  else if( blackCount > whiteCount) printf("B player wins."); 
}


// checks if the given direction with a coordinate is a possible move for the player
bool checkLegalInDirection(char board[][26], int n, int row, int col,
                           char colour, int deltaRow, int deltaCol)
{

  // checks for the validity of the first tile in that direction
  if (!positionInBounds(n, row + deltaRow, col + deltaCol) ||
      findOpposite(colour) != board[row + deltaRow][col + deltaCol])
    return false;

  // code reiteration starts 2 tiles in that direction
  int i = 2;

  while (positionInBounds(n, row + (i * deltaRow), col + (i * deltaCol)))
  {
    
    // checks the tile in that direction and of i distance and checks validity of the tile
    if (findOpposite(colour) == board[row + (i * deltaRow)][col + (i * deltaCol)])
      i++;
    else if (colour == board[row + (i * deltaRow)][col + (i * deltaCol)])
      return true;
    else if ('U' == board[row + (i * deltaRow)][col + (i * deltaCol)])
      return false;
  }

  return false;
}


// checks validity of move from 3 char input then places new piece if valid
bool isAvailable(char board[][26], int n, char colour)
{

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      for (int deltaRow = -1; deltaRow <= 1; deltaRow++)
      {
        for (int deltaCol = -1; deltaCol <= 1; deltaCol++)
        {

          if ((deltaRow != 0 || deltaCol != 0) && checkLegalInDirection(board, n, i, j, colour, deltaRow, deltaCol) &&
              board[i][j] == 'U')
          {

            return true;
          }
        }
      }
    }
  }
  return false;
}



int score(char board[][26], int n, char colour, int row, int col)
{

  int score = 0;
  int tempScore = 0;
  for (int deltaRow = -1; deltaRow <= 1; deltaRow++)
  {
    for (int deltaCol = -1; deltaCol <= 1; deltaCol++)
    {
      //checks if tile is empty, position in bounds and the move is legal
      if (board[row][col] == 'U' && positionInBounds(n, row, col) && 
          checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol))
      {
       
        int i = 1;
        //keeps flipping pieces until the same piece gets met
        while (!(deltaRow == 0 && deltaCol == 0) &&
               board[row + (i * deltaRow)][col + (i * deltaCol)] == findOpposite(colour))
        {
          tempScore = i;
          i++;
        }
      }
      score += tempScore;
      tempScore = 0;
    }
  }
  return score;
}


// the player of chosen colour moves
bool move(char board[][26], int n, char colour, int row, int col)
{

  // bool to see if the move is valid after checking all directions
  bool condition = false;

  for (int deltaRow = -1; deltaRow <= 1; deltaRow++)
  {
    for (int deltaCol = -1; deltaCol <= 1; deltaCol++)
    {
      
      //checks if tile is empty, position in bounds and the move is legal
      if (board[row][col] == 'U' && positionInBounds(n, row, col) && 
          checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol))
      {

        int i = 1;

        //keeps flipping pieces until the same piece gets met
        while (!(deltaRow == 0 && deltaCol == 0) &&
               board[row + (i * deltaRow)][col + (i * deltaCol)] == findOpposite(colour))
        {

          condition = true;
          board[row + (i * deltaRow)][col + (i * deltaCol)] = colour;
          i++;
        }
      }
    }
  }

  if (condition)
      {board[row][col] = colour; return true;}
    else
      {printf("Invalid move.\n"); return false;}
}



//iteration of available moves but finds the move w highest score, uses move function at end
bool cpuMove(char board[][26], int n, char colour)
{

  int row = -1, col = -1, highestScore = 0;
  for(int i = 0; i < n; i++)
  {
    for(int j = 0; j < n; j++)
    {
      if(score(board, n, colour, i, j) > highestScore)
      {

        row = i;
        col = j;
        highestScore = score(board, n, colour, i, j);
      }
    }
  }

  if(highestScore > 0 && move(board, n, colour, row, col))
  {

    printf("Computer places %c at %c%c.\n", colour, row + 'a', col + 'a');
    return true;
  }
  else return false;
} 



int main(void)
{

  int n;
  char cpuColour; 
  char currentTurn = 'B';
  char board[26][26];

  printf("Enter the board dimension: ");
  scanf("%d", &n);

  initialSetup(board, n);
  printf("Computer plays (B/W): ");
  scanf(" %c", &cpuColour);
  printBoard(board, n);
  bool condition = true;

  while(isAvailable(board, n, 'B') || isAvailable(board, n, 'W')){
    
    //CPU turn
    if(cpuColour == currentTurn){

      
      if(!isAvailable(board, n, cpuColour))
      {

        printf("%c player has no valid move.\n", cpuColour);
        currentTurn = findOpposite(currentTurn);
      }
      else{
      
      cpuMove(board, n, cpuColour);
      currentTurn = findOpposite(currentTurn);
      printBoard(board, n);
      }
    }


    //human turn
    else if(findOpposite(cpuColour) == currentTurn){

      // input as a string
      char pieceInfo[3];

      printf("Enter move for colour %c (RowCol): ", findOpposite(cpuColour));
      scanf("%s", pieceInfo);

      int row = pieceToNum(pieceInfo[0]);
      int col = pieceToNum(pieceInfo[1]);

      //places move and checks if the move is valid or not
      if(!isAvailable(board, n, findOpposite(cpuColour)))
      {

        printf("%c player has no valid move.\n", findOpposite(cpuColour));
        currentTurn = findOpposite(currentTurn);
      }
      else if(!move(board, n, findOpposite(cpuColour), row, col))
      {

        printf("%c player wins.\n", cpuColour);
        condition = false;
        break;
      }
      else {
      
        currentTurn = findOpposite(currentTurn);
        printBoard(board, n);
      }
    }
  }


  if(condition == true) winCount(board, n);

  return 0;
}
