#include "piece.h"



//Helper functions for converting between the indicies the user sees
//and the indicies the backend vector uses.
square convertInput(char * input);
int convertFile(char input);
int convertRank(char input);
char convertFileOpposite(int index);
char convertRankOpposite(int index);
vector<char> convertIndex(square pos);



//setup the board and all the pieces.
game::game()
{
	board.resize(8, vector<piece*>(8)); 
	//white pieces start on row 0 and row 1.
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			if(i == 0)
			{
				if(j==0 || j==7)
				{
					board[i][j] = new rook(0);
				}

				if(j==1 || j==6)
				{
					board[i][j] = new knight(0);
				}

				if(j==2 || j==5)
				{
					board[i][j] = new bishop(0);
				}

				if(j==3)
				{
					board[i][j] = new queen(0);
				}

				if(j==4)
				{
					board[i][j] = new king(0);
				}
			}
			//all pawns on this row
			if(i == 1)
			{
				board[i][j] = new pawn(0);
			}
		}
	}
	for(int i = 2; i < 6; ++i) {
		for(int j = 0; j < 8; ++j)
		{
			board[i][j] = NULL;
		}
	}

	//black pieces start on row 6 and row 7.
	for(int i = 6; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			if(i == 7)
			{
				if(j==0 || j==7)
				{
					board[i][j] = new rook(1);
				}

				if(j==1 || j==6)
				{
					board[i][j] = new knight(1);
				}

				if(j==2 || j==5)
				{
					board[i][j] = new bishop(1);
				}

				if(j==3)
				{
					board[i][j] = new queen(1);
				}

				if(j==4)
				{
					board[i][j] = new king(1);
				}
			}
			//all pawns on this row
			if(i == 6)
			{
				board[i][j] = new pawn(1);
			}
		}
	}
}



game::~game()
{
	deleteBoard();
}



//deletes all dynamically allocated memory for the board.
void game::deleteBoard()
{
	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			if(board[i][j])
			{
				delete board[i][j];
				board[i][j] = NULL;
			}
		}
	}
}



int game::playerTurn(int color)
{
	int flag;

	//user input
	char temp[100];

	//Square of the piece the user wants to move
	square start = make_pair(-1,-1);

	//(-1,-1) is an error code meaning the starting square 
	//meaning that either there is no piece at the square the user
	//selects, or, the user selects a square that is out of bounds of the array.
	while(start.first == -1 && start.second == -1)
	{
		if(color == 0)
		{
			cout << '\n' << "(White's Turn) ";
		}

		else
		{
			cout<< '\n' << "(Blacks's Turn) ";
		}

		cout << "Enter the square of the piece you would like to move: ";
		cin.get(temp, 100);
		cin.ignore(100, '\n');
		//since the index of the array and the index that is displayed to the user
		//are different, we need to convert the user input into a compatible form.
		start = convertInput(temp);

		//Don't want to try to access the board with board[-1][-1]
		//because that is out of bounds!
		if(start.first != -1 && start.second != -1)
		{	
			//No piece error
			if(!board[start.first][start.second])
			{
				cout << "There isn't a piece at this space. " << endl;
				start = make_pair(-1,-1);
			}

			else
			{
				//Make sure white is not trying to move blacks pieces.
				if(board[start.first][start.second]->getColor() == color)
				{
					flag = movePiece(start, color);
					//If flag is -1, that means that the piece
					//selected is unable to move.
					if(flag == -1)
					{
						//allow the user to try another square.
						start = make_pair(-1,-1);
						flag = 0;
					}

					//If flag is -2 that means we accidently self checked ourself
					//In this case it is helpful to display the board again.
					if(flag == -2)
					{
						displayBoard();
						start = make_pair(-1,-1);
						flag = 0;
					}
				}
				
				//Let the user know that the piece they are trying to move
				//is the other players piece, not theirs.
				else
				{
					cout << "Thats not your piece! " << endl;
					start = make_pair(-1,-1);
				}
			}
		}
	}
	displayBoard();
	return flag;
}



//Handles players taking turns, as well as allows the players to select the piece
//they want to move. Also does some basic error checking to make sure the players
//aren't trying to cheat!
void game::play()
{
	//Flag is used to notify if the user accidently selects a square where
	//there isn't a piece, in that case, we let them choose a different square.
	int flag = 0;

	//used to calculate whose turn it is
	int count = 0;
	
	displayBoard();
	
	//flag == 3 is the return code for checkmate from movePiece.
	while(flag != 3)
	{
		//Whites turn
		if(count % 2 == 0)
		{
			flag = playerTurn(count % 2);
			if(flag == 3)
			{
				cout << "\nCheckmate. ~Black Wins~ \n" << endl;
			}
		}
	
		//Blacks turn, repeat all the steps done for white with black.
		if(count % 2 == 1)
		{
			flag = playerTurn(count % 2);
			if(flag == 3)
			{
				cout << "\nCheckmate. ~White Wins~\n" << endl;
			}
		}
		++count;
	}
}



//Displays the vector of possible moves in a helpful way to the players to help
//them make decisions about what their next move should be.
void game::displayPossibleMoves(square start, const vector<square>& possibleMoves)
{
	//Build a tempory representation of the board so we can display the 
	//possible moves a player can make.
	char array[8][8];
	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			if(i == start.first && j == start.second)
			{
				array[i][j] = 'X';
			}

			else
			{
				array[i][j] = '*';
			}
		}
	}

	cout << '\n' << "Available Moves: ";
	for(square x: possibleMoves)
	{
		vector<char> temp = convertIndex(x);
		for(char y: temp)
		{
			cout << y;
		}
		cout << " ";
		array[x.first][x.second] = ' ';
	}

	//Board representation of possible moves the player can make.
	cout << '\n' << "   " << "-----------------" << '\n';
	for(int i = 0; i < 8; ++i)
	{
		cout << i+1 << " | ";
		for(int j = 0; j < 8; ++j)
		{
			cout << array[i][j] << " ";
		}
		cout << "|" << '\n';
	}

	cout << "   " << "-----------------" << '\n'
	     << "    " << "A " << "B " << "C " << "D " << "E " << "F " 
	     << "G " << "H " << endl;
	return;
}



//Given a vector of possible moves, allow the user to pick the move
//they want, and return that to movePiece, so it can implement that change
//to the actual board.
square game::pickMove(const vector<square>& possibleMoves)
{
	square end = make_pair(-1,-1);
	char temp[100];
	while(end.first == -1 && end.second == -1)
	{
		cout << '\n' << "Move to: ";
		cin.get(temp, 100);
		cin.ignore(100, '\n');
		end = convertInput(temp);
		for(square x: possibleMoves)
		{
			if(x == end)
			{
				return end;
			}
		}
		cout << "That is not a possible move " << endl;
		end = make_pair(-1,-1);
	}
}



//Special move for a pawn. In passing
void game::enPassant(square start, square end)
{
		// en passant delete
		if(board[start.first][start.second]->isPawn())
		{
			if(start.first+1 == end.first)
			{
				if(start.second+1 == end.second || start.second-1 == end.second)
				{
					if(!board[end.first][end.second])
					{
						delete board[end.first-1][end.second];
						board[end.first-1][end.second] = NULL;
					}
				}
			}

			if(start.first-1 == end.first)
			{
				if(start.second+1 == end.second || start.second-1 == end.second)
				{
					if(!board[end.first][end.second])
					{
						delete board[end.first+1][end.second];
						board[end.first+1][end.second] = NULL;

					}
				}
			}
		}
		return;
}



//When a Pawn gets to the last rank of the opponents side of the board it gets
//promoted from a pawn to a queen.
void game::pawnPromotion(square start, square end)
{
		//Pawn promotion, if a pawn reaches the final rank,
		//it gets promoted to a queen.
		if(board[end.first][end.second] && board[end.first][end.second]->isPawn())
		{
			if(board[end.first][end.second]->getEnPassantFlag())
			{
				board[end.first][end.second]->setEnPassantFlag(false);
			}

			//set enPassantFlag if the pawn used its special.
			if(start.first-2 == end.first || start.first+2 == end.first)
			{
				board[end.first][end.second]->setEnPassantFlag(true);
			}

			if(end.first == 0 || end.first == 7)
			{
				int color = board[end.first][end.second]->getColor();
				delete board[end.first][end.second];
				board[end.first][end.second] = new queen(color);
			}
		}
	return;
}



//This method defines the movement for the kinds special move, castling
void game::castle(square start, square end)
{
	/*If the piece is a king, it may also have the ability to use its special move
	  called castling. The reason why this is done here instead of the king's 
	  move method, is that it involves moving both the king and the rook.
	  We also need to check if the king has forfited castling rights.*/
	if(board[start.first][start.second]->isKing())
	{
		//The only time a king can move two spaces is during a castle.
		if(end.second == start.second - 2)
		{
			piece * rook = board[start.first][start.second-4];
			board[start.first][start.second-4] = NULL;
			board[start.first][start.second-1] = rook;
		}

		if(end.second == start.second + 2)
		{
			piece * rook = board[start.first][start.second+3];
			board[start.first][start.second+3] = NULL;
			board[start.first][start.second+1] = rook;
		}

		piece * king = board[start.first][start.second];

		//If the king every moves he forfeits castling rights.
		king -> setCastlingRights(false);
	}
	return;
}



//This method handles manages the board pointers and is responsible for moving
//pieces on the board. If it returns -1, the piece cannot be moved, if it returns 0,
//Pieces moved but no king was put in check. and if it returns 1, then the next player
//will have to deal with check.
int game::movePiece(square start, int color)
{
	//starting the turn in check
	square kingPos = findKing(color);
	if(scanForCheck(kingPos))
	{
		if(scanForMate(kingPos, color))
		{
			return 3;
		}
	}

	int enemyColor;
	//Build a vector of all the possible moves of the selected piece.
	vector<square> possibleMoves = board[start.first][start.second]->move(start, getBoard());
	square end = make_pair(-1,-1);

	if(possibleMoves.empty())
	{
		cout << "This piece cannot be moved, try another. " << endl;
		return -1; //error code for piece cannot be moved.
	}

	displayPossibleMoves(start, possibleMoves);
	end = pickMove(possibleMoves);

  	castle(start, end);
	//If enPassant was the move chosen by the users, this will handle
	//the special movement case, if not, it will just return.
	enPassant(start, end);

	//default movement.
	piece * tempStart = board[start.first][start.second];
	piece * tempEnd = board[end.first][end.second];
	board[start.first][start.second] = NULL;
	board[end.first][end.second] = tempStart;

	//check to see if moving this piece puts myself in check
	//if it does then we need to revert the changes to the board,
	//and let the player try another move.
	kingPos = findKing(color);

	if(scanForCheck(kingPos))
	{	
		cout << "Whoops!~ That move puts yourself in check! Try another. " << endl;
		board[start.first][start.second] = tempStart;
		board[end.first][end.second] = tempEnd;
		return -2;
	}

	else
	{
		if(tempEnd)
		{
			delete tempEnd;
			tempEnd = NULL;
		}
	}
	
	//if I pawn reaches the last rank on the enemy side it will
	//get promoted to a queen.
	pawnPromotion(start, end);



	//check to see if the piece I moved put the enemy in check
	if(color == 0)
	{
		enemyColor = 1;
	}

	else
	{
		enemyColor = 0;
	}

	kingPos = findKing(enemyColor);
	if(scanForCheck(kingPos))
	{
		cout << "Opponent in check. " << endl;
		return 1;
	}

	return 0; //exit code for default movement. (no checks)
}



//This method scans the board and returns the position of the white or black king
//depending on the color passed in.
//remember, color == white is color == 0 and color == black is color == 1
square game::findKing(int color)
{
	square kingPosition;

	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			if(board[i][j] && board[i][j]->isKing() && board[i][j]->getColor() == color)
			{
				kingPosition = make_pair(i,j);
			}
		}
	}
	return kingPosition;
}


bool game::canMoveKing(square kingPosition)
{
	//check to see if the king can move anywhere to stop checka
	vector<square> possibleMoves =
		board[kingPosition.first][kingPosition.second] -> move(kingPosition, getBoard());

	for(square x: possibleMoves)
	{
		piece * tempStart = board[kingPosition.first][kingPosition.second];
		piece * tempEnd = board[x.first][x.second];
		board[kingPosition.first][kingPosition.second] = NULL;
		board[x.first][x.second] = tempStart;
		//If the king can move to a position where it is not in check,
		//there is no checkmate.
		if(!scanForCheck(x))
		{
			board[kingPosition.first][kingPosition.second] = tempStart;
			board[x.first][x.second] = tempEnd;	
			return true;
		}
		board[kingPosition.first][kingPosition.second] = tempStart;
		board[x.first][x.second] = tempEnd;	
	}
	return false;
}



bool game::canSaveKing(square kingPosition, int color)
{
	vector<square> possibleMoves;
	//check to see if a piece can block the piece that is putting the king in check.
	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			//cout << i << endl;
			if(board[i][j] && board[i][j] -> getColor() == color && !board[i][j]->isKing())
			{
				square temp = make_pair(i,j);
				possibleMoves = board[i][j] -> move(temp, getBoard());
				for(square x: possibleMoves)
				{
					piece * tempStart = board[temp.first][temp.second];
					piece * tempEnd = board[x.first][x.second];
					board[temp.first][temp.second] = NULL;
					board[x.first][x.second] = tempStart;
					//If the king can move to a position where it is not in check,
					//there is no checkmate.
					if(!scanForCheck(kingPosition))
					{
						board[temp.first][temp.second] = tempStart;
						board[x.first][x.second] = tempEnd;	
						return true;
					}
					board[temp.first][temp.second] = tempStart;
					board[x.first][x.second] = tempEnd;	
				}
			}

		}
	}
	return false;
}



//Scan the board for checkmate.
bool game::scanForMate(square kingPosition, int color)
{
	if(canMoveKing(kingPosition) || canSaveKing(kingPosition, color))
	{
		return false;
	}
	return true;
}



//Scan all enemy pieces to detect if the king is in check.
bool game::scanForCheck(square kingPosition)
{
	vector<square> possibleMoves;
	for(int i=0; i<8; ++i)
	{
		for(int j=0; j<8; ++j)
		{
			//If the king and the piece we are looking at are different colors
			//then we want to see if the king is in check.
			if(board[i][j] && board[kingPosition.first][kingPosition.second]->getColor() 
				!= board[i][j] -> getColor())
			{

				square temp = make_pair(i,j);
				possibleMoves = board[i][j] -> move(temp, getBoard());

				for(square x: possibleMoves)
				{
					if(x.first == kingPosition.first && 
						x.second == kingPosition.second)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}



//display the board to test moving pieces before the gui for the app is developed.
void game::displayBoard()
{
	cout << '\n';
	cout << "   " << "-----------------" << '\n';
	for(int i = 0; i < 8; ++i)
	{
		cout << i+1 << " | ";
		for(int j = 0; j < 8; ++j)
		{
			if(board[i][j])
				board[i][j]->display();
			else
				cout << "* ";
		}
		cout << "|" << '\n';
	}

	cout << "   " << "-----------------" << '\n';
	cout << "    ";
	cout << "A " << "B " << "C " << "D " << "E " << "F " << "G " << "H " << endl;
	return;
}



//convert front end row indicies to back end row indicieis.
int convertFile(char input)
{
	int file;

	if(input == 'a' || input == 'A')
	{
		file = 0;	
	}

	else if(input == 'b' || input == 'B')
	{
		file = 1;
	}

	else if(input == 'c' || input == 'C')
	{
		file = 2;
	}

	else if(input == 'd' || input == 'D')
	{
		file = 3;
	}

	else if(input == 'e' || input == 'E')
	{
		file = 4;
	}

	else if(input == 'f' || input == 'F')
	{
		file = 5;
	}

	else if(input == 'g' || input == 'G')
	{
		file = 6;
	}

	else if(input == 'h' || input == 'H')
	{
		file = 7;
	}

	else
	{
		file = -1;
	}

	return file;
}



//convert front end column indicies to back end column indicies.
int convertRank(char input)
{
	int rank;

	if(input == '1')
	{
		rank = 0;	
	}

	else if(input == '2')
	{
		rank = 1;	
	}

	else if(input == '3')
	{
		rank = 2;	
	}

	else if(input == '4')
	{
		rank = 3;	
	}

	else if(input == '5')
	{
		rank = 4;	
	}

	else if(input == '6')
	{
		rank = 5;	
	}

	else if(input == '7')
	{
		rank = 6;	
	}

	else if(input == '8')
	{
		rank = 7;	
	}

	else
	{
		rank = -1;
	}

	return rank;
}



//convert back end column indicies to front end column indices
char convertRankOpposite(int index)
{
	char rank;

	if(index == 0)
	{
		rank = '1';	
	}

	else if(index == 1)
	{
		rank = '2';	
	}
	else if(index == 2)
	{
		rank = '3';	
	}
	else if(index == 3)
	{
		rank = '4';	
	}
	else if(index == 4)
	{
		rank = '5';	
	}
	else if(index == 5)
	{
		rank = '6';	
	}
	else if(index == 6)
	{
		rank = '7';	
	}
	else if(index == 7)
	{
		rank = '8';	
	}
	else
	{
		rank = 'z';
	}

	return rank;
}



//Convert the backend row indicies to frontend row indicies.
char convertFileOpposite(int index)
{
	char file;

	if(index == 0)
	{
		file = 'A';	
	}

	else if(index == 1)
	{
		file = 'B';	
	}
	else if(index == 2)
	{
		file = 'C';	
	}
	else if(index == 3)
	{
		file = 'D';	
	}
	else if(index == 4)
	{
		file = 'E';	
	}
	else if(index == 5)
	{
		file = 'F';	
	}
	else if(index == 6)
	{
		file = 'G';	
	}
	else if(index == 7)
	{
		file = 'H';	
	}

	return file;
}



//When we create the vector of all the possible moves a piece can make,
//it uses the indicies of the board vector in the back end of the application.
//This method converts the indicies to the ones that the user sees when they look
//at the board. Essentially this is a helper method that makes the game more readible.
vector<char> convertIndex(square pos)
{
	vector<char> convertedIndex;
	char temp;
	convertedIndex.push_back(convertFileOpposite(pos.second));
	convertedIndex.push_back(convertRankOpposite(pos.first));
	return convertedIndex;

}



//Since there is a discrepancy between the index of the board that is shown
//to the user (rows are numbered 1-8, and columns are labeled a-h), we need to
//convert these indicies to the indicies used by the board vector in the backend
//(so 0-7 for both rows and columns).
square convertInput(char * input)
{
	square start = make_pair(-1,-1);

	int first = convertRank(input[1]);
	int second = convertFile(input[0]);

	//Valid input was given
	if(first != -1 && second != -1)
	{
		start = make_pair(first, second);	
	}

	else
	{
		cout << "Invalid position, try again :) " << endl;
	}

	return start;
}
