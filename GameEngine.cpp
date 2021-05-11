
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

#include "GameEngine.h"
#include "LinkedList.h"
#include "TileCodes.h"

GameEngine::GameEngine() {}

GameEngine::~GameEngine() {}

void GameEngine::newGame() {
  if (!std::cin.eof()) {
    std::cout << std::endl << "Starting a New Game" << std::endl << std::endl;
    int numOfPlayers = 2;
    LinkedList *playerHands[MAX_PLAYERS];
    std::string players[MAX_PLAYERS];

    for (int i = 0; i < numOfPlayers; i++) {
      bool isValidInput = true;
      if (!std::cin.eof()){
          do {
            std::cout << "Enter a name for player " << i + 1
                      << " (uppercase characters only)" << std::endl
                      << "> ";

            isValidInput = true;
            std::cin >> players[i];
            std::cin.ignore(); // Prevents carriage return
            for (unsigned int j = 0; j < players[i].length(); j++) {
              // Check if lowercase
              if ((players[i][j] >= *"a" && players[i][j] <= *"z") ||
                  (players[i][j] >= *"0" && players[i][j] <= *"9")) {
                isValidInput = false;
              }
            }

            if (!isValidInput) {
              std::cout << std::endl << "Invalid. MUST USE UPPERCASE" << std::endl;
            } else if (isValidInput) {
              // Create hand for player
              playerHands[i] = new LinkedList();
            }
          } while (!isValidInput && !std::cin.eof());
        }
      }

    // Populate tilebag
    LinkedList *tileBag = new LinkedList();
    tileBag->populateLinkedList();
    tileBag->shuffleLinkedList();

    // Empty Player Score Array
    int playersScores[MAX_PLAYERS];

    // Ensure score is 0
    for ( int i = 0; i<numOfPlayers;i++){
      playersScores[i] = 0;
    }

    // Empty Board State
    std::vector<std::string> boardState;

    // Default Board Dim
    int boardDim[] = {26, 26};

    std::cout << std::endl << "Let's Play!" << std::endl;


    startGame(numOfPlayers, players, tileBag, playerHands, 0,
              playersScores, boardState, boardDim);
  }
}

void GameEngine::loadGame(){
  std::cout   << std::endl 
              << "Enter the filename from which load a game" 
              << std::endl << "> ";


  std::string filename;
  std::cin >> filename;

  std::ifstream in(filename);

  std::string str;
  std::vector<std::string> fileVector;
  // Read the next line from File untill it reaches the end.
  while (std::getline(in, str))
  {
    fileVector.push_back(str);
  }

  // int                                numOfPlayers                  This is the amount of players   DONE
  int numOfPlayers = 2;
  // std::string        Parralel Array	players[MAX_PLAYERS]          These is the players names    DONE
  std::string players[MAX_PLAYERS];
  for ( unsigned int i=0; i<fileVector.size(); i++){
    // fileVector[i] is the line in the file
    if ( i == 0 ){
      players[0] = fileVector[i];
    }
    if ( i == 3 ){
      players[1] = fileVector[i];
    }
  }
  // LinkedList                         tileBag                       This is the games tilebag
  LinkedList *tileBag = new LinkedList();
  // LinkedList array   Parralel Array	playerHands                   These are the players hands
  LinkedList *playerHands[MAX_PLAYERS];
  // int                                currentPlayer                 This is the currents playes turn
  int currentPlayer = 0;
  // int array          Parralel Array	playersScores[MAX_PLAYERS]    These are the players scores
  int playersScores[MAX_PLAYERS];
  // vector array       boardState                                    This is the boards state
  std::vector<std::string> boardState;
  // int array          boardDim                                      This is the boards dimenstions  DONE
  int boardDim[] = {26, 26};

  startGame(numOfPlayers, players, tileBag, playerHands, currentPlayer,
            playersScores, boardState, boardDim);
}

void GameEngine::startGame(int numOfPlayers, std::string players[MAX_PLAYERS],
                           LinkedList *tileBag,
                           LinkedList *playerHands[MAX_PLAYERS],
                           int currentPlayer,
                           int playersScores[MAX_PLAYERS],
                           std::vector<std::string> boardState,
                           int boardDim[1]) {
  bool inGame = true;

  while (inGame && !std::cin.eof()) {
      if (inGame) {
        // Tile Display
        std::cout << players[currentPlayer] << ", it's your turn" << std::endl;
        for (int i = 0; i < numOfPlayers; i++) {
          std::cout << "Score for " << players[i] << ":" << playersScores[i]
                    << std::endl;
        }

        // Integers for columns
        for (int i = 0; i < boardDim[0]; i++) {
          if (i == 0) {
            std::cout << "  ";
            for (int j = 0; j < boardDim[1]; j++) {
              if ( j > 9 ){
                std::cout << j << " ";
              }
              else{
                std::cout << j << "  ";
              }
            }
            std::cout << std::endl << " ";
            for (int j = 0; j < boardDim[1]; j++) {
              std::cout << "---";
            }
          }

          // Uppercase Letters for rows
          std::cout << std::endl;
          char letter = *"A";
          char currentLetter = letter + i;
          std::cout << currentLetter;

          // Print this rows tiles and spaces
          for (int j = 0; j < boardDim[1]; j++) {
            std::cout << "|";
            bool tileIsHere = false;
            std::string currentNumber = std::to_string(j);
            for (unsigned int i = 0; i < boardState.size(); i++) {
              if (currentLetter == boardState[i][3]) {
                char tileNumber = boardState[i][4];
                char tileSecondNumber = boardState[i][5];
                if (currentNumber[0] == tileNumber && currentNumber[1] == tileSecondNumber) {
                  std::cout << boardState[i][0] << boardState[i][1];
                  tileIsHere = true;
                }
              }
            }
            if (!tileIsHere) {
              std::cout << "  ";
            }
          }
          std::cout << "|";
        }

        // User draw max amount of tiles
        playerHands[currentPlayer]->drawHand(tileBag);

        std::cout << std::endl << std::endl << "Your hand is" << std::endl;
        playerHands[currentPlayer]->printHand();

        // Do user input
        bool inputIsValid = false;
        
        while (!inputIsValid && !std::cin.eof()) {
          std::cout << std::endl << "> ";
          std::string userIn;
          std::getline(std::cin, userIn);
         

          std::vector<std::string> inArr; // Input
          std::stringstream data(userIn);
          
          std::string tmpString;
          
          while (std::getline(data, tmpString, ' ')) {
            inArr.push_back(tmpString);
          }

          // place G5 at C4
          if (inArr.size() > 0) {
            if (inArr[0] == "place") {
              if (inArr.size() > 1) {
                if (checkTileFormat(inArr[1])) {
                  if (inArr.size() > 2) {
                    if (inArr[2] == "at") {
                      if (inArr.size() > 3) {
                        if (placeLoactionCheck(boardState, boardDim,
                                               inArr[3])) {
                          if (checkTileInPlayerHand(inArr[1],
                                                    playerHands[currentPlayer])) {
                            if ( inArr[3][4] >= *"0" && inArr[3][4] <= *"9" ){
                            boardState.push_back(inArr[1] + "@" +
                                                 inArr[3]+inArr[4]);
                            }
                            else{
                            boardState.push_back(inArr[1] + "@" +
                                                 inArr[3]);
                            }

                                                 for ( unsigned int i = 0; i<boardState.size(); i++){
                                                   std::cout << boardState[i];
                                                 }
                           /* Scoring function
                              TODO: 2.3.5 
                              Load board state
                              a = position to inArr[3]
                              Iterate over board state get the tiles relative 
                              to the placed tile and determine the score
                              
                           */
                            inputIsValid = true;
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
            // IF replace
            if (inArr[0] == "replace") { 
               /*Write code
                  TODO: 2.3.6 Replacing tile in hand
               */
            }
            // IF save game
            if (inArr[0] == "save") { 
              if (inArr.size() > 1) {
                /*Write code
                    TODO: 2.3.7 Save game
                */
                std::ofstream outfile(inArr[1]);
                for ( int i=0; i<numOfPlayers; i++){
                  outfile << players[i] << std::endl
                          << playersScores[i] << std::endl
                          << playerHands[i]->printHandToString()
                          << std::endl;
                }
                for ( unsigned int i = 0; i<boardState.size(); i++ ){
                  if (i == boardState.size()-1){
                    outfile << boardState[i];
                  }
                  else {
                    outfile << boardState[i]+",";
                  }
                }
                outfile << std::endl
                        << boardDim[0] << "," << boardDim[1] << std::endl
                        << tileBag->printHandToString()
                        << std::endl
                        << players[currentPlayer];
                outfile.close();
                inputIsValid = true;
              }
            }
          } 
          if (std::cin.eof()){
            inputIsValid = true;
          }
          if (!inputIsValid) {
            std::cout << std::endl << "Invalid Input";
          }
        }
      }
    
    // Change player turn
    if ( currentPlayer == numOfPlayers-1 ){
      currentPlayer = 0;
    }
    else {
      currentPlayer++; 
    }
  }
}

/*
   For the place command
   example...   place P4 at A1

   Ensures the tile (P4) is correctly formatted.
*/
bool GameEngine::checkTileFormat(std::string tile) {
  bool isValid = false;
  if (tile[0] == RED || tile[0] == ORANGE || tile[0] == YELLOW ||
      tile[0] == GREEN || tile[0] == BLUE || tile[0] == PURPLE) {
    isValid = true;
  }

  int shapeInt = (int)tile[1] - '0';
  if (shapeInt == CIRCLE || shapeInt == STAR_4 || shapeInt == DIAMOND ||
      shapeInt == SQUARE || shapeInt == STAR_6 || shapeInt == CLOVER) {
    isValid = true;
  }
  return isValid;
}

/*
   For the place command
   example...   place P4 at A1

   Ensures the coordinates (A1) are correctly formatted.

   And there are no tiles on the location
*/
bool GameEngine::placeLoactionCheck(std::vector<std::string> boardState,
                                    int boardDim[1], std::string pos) {
  int posNumber = (int)pos[1] - '0';
  int posSecondNumber = (int)pos[2] - '0';
  if ( posSecondNumber < '0' ){
    posSecondNumber = 0;
  }
  int posChar = pos[0];
  int posCharRef = *"A";
  bool posIsValid = false;

  // If between A-Z
  if (posChar >= posCharRef && posChar < posCharRef + boardDim[0]) {
    if (posNumber < 9 && posSecondNumber < 9 ) {
        posIsValid = true;
    }
  }

  for (unsigned int i = 0; i < boardState.size(); i++) {
    if (boardState[i][3] == pos[0]) {
      if (boardState[i][4] == pos[1]) {
        if (boardState[i][5] == pos[2]) {
          posIsValid = false;
        }
      }
    }
  }
  

   /* TILE PLACEMENT GAME RULES 
   TODO: 2.3.5 Check tile placement

   After the command is given, the program must:
   [./]   1. Check that the command is correctly formatted.
   [.O]   2. Check that the placement of tile is legal according to the rules of Qwirkle.
   */
   
  return posIsValid;
}

/*
   For the place command
   example...   place P4 at A1

   Ensures the tile (P4) exists in players hand
*/
bool GameEngine::checkTileInPlayerHand(std::string tile,
                                       LinkedList *playerhand) {
  bool isInhand = false;
  for (int i = 0; i < playerhand->size(); i++) {
    std::string currentTile =
        playerhand->getTile(i)->getTileColour() +
        std::to_string(playerhand->getTile(i)->getTileShape());
    if (currentTile == tile) {
      playerhand->remove(i);
      isInhand = true;
    }
  }
  return isInhand;
}