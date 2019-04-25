#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// gets random number between min-max inclusive
int genRandom(unsigned int min, unsigned int max) {
   int r;
   const unsigned int range   = 1 + max - min;
   const unsigned int buckets = RAND_MAX / range;
   const unsigned int limit   = buckets * range;

   do {
      r = rand();
   } while (r >= limit);

   return min + (r / buckets);
}

// finds largest value in an int array
int findLargest(int arr[], unsigned int size) {
   int largest = arr[0];
   for (int i = 1; i < size; i++) {
      if (arr[i] > largest) {
         largest = arr[i];
      }
   }
   return largest;
}

// prints board + stats
void printStats(char board[], bool turn) {
   printf("TURN: %s\n", (turn ? "X (You)" : "O (CPU)"));
   for (int i = 0; i < 9; i++) {
      if (i % 3 == 0) printf("\n|");
      printf("%c|", board[i]);
   }
   printf("\n");
}

// check if board is in win state for specified symbol
bool checkWinner(char symbol, char board[]) {

   const int length = 3;

   int fToken = 0;
   int bToken = 0;

   for (int x = 0; x < length; x++) {

      int xToken = 0; 
      int yToken = 0;

      if (board[(x * 3) + x] == symbol) fToken += 1;

      const int r = (length - 1) - x;
      if (board[(x * 3) + r] == symbol) bToken += 1;

      for (int y = 0; y < length; y++) {

         if (board[(y * length) + x] == symbol) yToken += 1;

         if (board[(x * length) + y] == symbol) xToken += 1;

         if (xToken == length || yToken == length || 
             fToken == length || bToken == length) return true;

      }

   }

   return false;
}

// computer ai
void compMakeMove(int* mx, int* my, char board[]) {
   int px = 0, py = 0, maxToken = 0;

   for (int x = 0; x < 3; x++) { 
      for (int y = 0; y < 3; y++) {

         // only calculate if spot empty
         if (board[y * 3 + x] == ' ') {

            board[y * 3 + x] = 'O';
            // automatically make move if computer would win.
            if (checkWinner('O', board)) {
               px = x;
               py = y;
               maxToken = 999;
            }

            // find highest scoring move
            int fToken = 0, bToken = 0;
            for (int cx = 0; cx < 3; cx++) {

               int xToken = 0, yToken = 0;

               if (board[(cx * 3) + cx] == 'O') fToken += 1;
               const int r = 2 - cx;
               if (board[(cx * 3) + r] == 'O') bToken += 1;

               for (int cy = 0; cy < 3; cy++) {
                  if (board[(cy * 3) + cx] == 'O') yToken += 1;

                  if (board[(cx * 3) + cy] == 'O') xToken += 1;

                  int tokens[4] = { xToken, yToken, fToken, bToken };
                  const int largest = findLargest(tokens, 4);
               
                  if (largest > maxToken) {
                     px = x;
                     py = y;
                     maxToken = largest;
                  }

                  if (maxToken < 100 && largest == maxToken) {
                     const int rn = rand() % 100;
                     if (rn < 5) {
                        px = x;
                        py = y;
                     }
                  }
               }
            }

            board[y * 3 + x] = 'X';

            if (checkWinner('X', board) && maxToken < 500) {
               px = x;
               py = y;
               maxToken = 499;
            }

            board[y * 3 + x] = ' ';
         }
      }
   }

   *my = py;
   *mx = px;
}

int main(void) {

   char board[9];
   bool turn  = true;
   int  moves = 0;

   //make empty board
   for (int i = 0; i < 9; i++) 
      board[i] = ' ';

   while(true) {

      printStats(board,turn);

      // board maxed out with no winner. draw.
      if (moves >= 9) break;

      const int movesPre = moves;

      int x = 0, y = 0;
      const char symb = turn ? 'X' : 'O';

      if (turn) {
         // player turn
         do {
            printf("Pick X position: ");
            scanf("%d", &x);
            printf("Pick Y position: ");
            scanf("%d", &y);
         } while (board[y * 3 + x] != ' ' || ((x | y) > 3 || (x | y) < 0));
         moves += 1;
      } else {
         // computer turn
         compMakeMove(&x, &y, board);
         moves += 1;
      }

      // make move
      if (moves > movesPre) {
         board[y * 3 + x] = symb;
         turn = !turn;
      }

      // end game if win state
      if (checkWinner(symb, board)) {
         printf("----------\nWINNER: %c!\n----------\n", symb);
         break;
      }

   }

   printf("----------\nGAME OVER!\n----------\n");

   return 0;
}