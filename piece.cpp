#include "piece.h"

//Defines pawn movement in chess. Covers pawns special inital move forward two start,
//normal movement, capturing - including en passant
vector<square> pawn::move(square start, const vector<vector<piece*>>& board)
{
	vector<square> possibleMoves;

	//if the piece is white
	if(getColor() == 0)
	{
		square temp = make_pair(start.first+1, start.second);	
		
		//if a piece is not in front of the pawn
		if(!board[temp.first][temp.second])
		{	
			possibleMoves.push_back(temp);
		}

		/*if the pawn is still in its intial rank and file, it has
		  the option to move two spaces forward instead of one, so we add
		  this to our possible moves vector.*/
		if(start.first == 1)
		{
			temp = make_pair(start.first+2, start.second);
			if(!board[temp.first][temp.second] && !board[temp.first-1][temp.second])
			{	
				possibleMoves.push_back(temp);
			}
		}	

		//en passant
		if(start.first == 4)
		{
			//make sure this is only checking if it is within the array bounds
			if(start.second+1 < 8)
			{
				if(board[start.first][start.second+1])
				{
					if(board[start.first][start.second+1]->getEnPassantFlag())
					{
						temp = make_pair(start.first+1, start.second+1);
						possibleMoves.push_back(temp);
					}
				}
			}
			if(start.second-1 > 0)
			{
				if(board[start.first][start.second-1])
				{
					if(board[start.first][start.second-1]->getEnPassantFlag())
					{
						temp = make_pair(start.first+1, start.second-1);
						possibleMoves.push_back(temp);
					}
				}
			}
		}

		//Pawn capturing.
		/*If there is an enemy piece on one of the two diagonals the pawn
		  may move there and capture the enemy piece */
		if((start.second+1) <= 7)
		{
			if(board[start.first+1][start.second+1] &&
				board[start.first+1][start.second+1]->getColor() != getColor())
			{
				temp = make_pair(start.first+1, start.second+1);
				possibleMoves.push_back(temp);
			}
		}

		if((start.second-1) >= 0)
		{
			if(board[start.first+1][start.second-1] &&
				board[start.first+1][start.second-1]->getColor() != getColor())
			{
				temp = make_pair(start.first+1, start.second-1);
				possibleMoves.push_back(temp);
			}
		}
	}

	else
	{
		square temp = make_pair(start.first-1, start.second);	
		if(temp.first == 0)
		{
			return possibleMoves;
		}

		//if a piece is not in front of the pawn
		if(!board[temp.first][temp.second])
		{	
			possibleMoves.push_back(temp);
		}

		/*if the pawn is still in its intial rank and file, it has
		  the option to move two spaces forward instead of one, so we add
		  this to our possible moves vector.*/
		if(start.first == 6)
		{
			temp = make_pair(start.first-2, start.second);
			if(!board[temp.first][temp.second] && !board[temp.first+1][temp.second])
			{	
				possibleMoves.push_back(temp);
			}
		}	

		//en passant
		if(start.first == 3)
		{
			//make sure this is only checking if it is within the array bounds
			if(start.second+1 < 8)
			{
				if(board[start.first][start.second+1])
				{
					if(board[start.first][start.second+1]->getEnPassantFlag())
					{
						temp = make_pair(start.first-1, start.second+1);
						possibleMoves.push_back(temp);
					}
				}
			}
			if(start.second-1 > 0)
			{
				if(board[start.first][start.second-1])
				{
					if(board[start.first][start.second-1]->getEnPassantFlag())
					{
						temp = make_pair(start.first-1, start.second-1);
						possibleMoves.push_back(temp);
					}
				}
			}
		}

		//pawn capturing
		if((start.second+1) <= 7 && (start.second-1) >= 0)
		{
			/*If there is an enemy piece on one of the two diagonals the pawn
			  may move there and capture the enemy piece */
			if(board[start.first-1][start.second+1] &&
				board[start.first-1][start.second+1]->getColor() != getColor())
			{
				temp = make_pair(start.first-1, start.second+1);
				possibleMoves.push_back(temp);
			}


			if(board[start.first-1][start.second-1] &&
				board[start.first-1][start.second-1]->getColor() != getColor())
			{
				temp = make_pair(start.first-1, start.second-1);
				possibleMoves.push_back(temp);
			}
		}
	}

	return possibleMoves;
}



vector<square> queen::move(square start, const vector<vector<piece*>>& board)
{
	vector<square> possibleMoves;
	moveRank(start, board, possibleMoves);
	moveFile(start, board, possibleMoves);
	moveDiagonol(start, board, possibleMoves);
	return possibleMoves;
}



/*This method defines the movement for a king in a game of chess. A king may move
  one space in any direction, as long as it won't put the king in check. A king 
  also has access to a special move called castling. If the king meets the conditions
  to castle, the king may move two spaces right or left, and it also moves the rook.
*/
vector<square> king::move(square start, const vector<vector<piece*>>& board)
{
	vector<square> possibleMoves;
	square temp;

	//Move up one space.
	temp = make_pair(start.first-1, start.second);
	if(0 <= temp.first && temp.first < 8 && 0 <= temp.second && temp.second < 8)
		possibleMoveHelper(temp, board, possibleMoves);

	//Move down one space.
	temp = make_pair(start.first+1, start.second);
	if(0 <= temp.first && temp.first < 8 && 0 <= temp.second && temp.second < 8)
		possibleMoveHelper(temp, board, possibleMoves);

	//Move left one space.
	temp = make_pair(start.first, start.second-1);
	if(0 <= temp.first && temp.first < 8 && 0 <= temp.second && temp.second < 8)
		possibleMoveHelper(temp, board, possibleMoves);

	//Move right one space.
	temp = make_pair(start.first, start.second+1);
	if(0 <= temp.first && temp.first < 8 && 0 <= temp.second && temp.second < 8)
		possibleMoveHelper(temp, board, possibleMoves);
	

	//Move diagonolly up and right, one space.
	temp = make_pair(start.first-1, start.second+1);
	if(0 <= temp.first && temp.first < 8 && 0 <= temp.second && temp.second < 8)
		possibleMoveHelper(temp, board, possibleMoves);

	//Move diagonolly up and left, one space.
	temp = make_pair(start.first-1, start.second-1);
	if(0 <= temp.first && temp.first < 8 && 0 <= temp.second && temp.second < 8)
		possibleMoveHelper(temp, board, possibleMoves);

	//Move diagonolly down and right, one space.
	temp = make_pair(start.first+1, start.second+1);
	if(0 <= temp.first && temp.first < 8 && 0 <= temp.second && temp.second < 8)
		possibleMoveHelper(temp, board, possibleMoves);

	//Move diagonolly down and left, one space.
	temp = make_pair(start.first+1, start.second-1);
	if(0 <= temp.first && temp.first < 8 && 0 <= temp.second && temp.second < 8)
		possibleMoveHelper(temp, board, possibleMoves);
	
											/*
	Castling is the special move for a king, but it also involves moving
	the rook in addition to the king. So, instead of pushing the square of where
	the king will move to from castling, we will pass a square that acts as a 
	flag. Here we will pass (-1,-1) if we are castling left, or (8,8) if we
	are castling to the right. This will let the move piece method know that
	the move we want to perform is castling instead of a normal move, since
	these squares are actually out of bounds.
									      */
	if(castlingRights)
	{
	      if(!board[start.first][start.second+1] && !board[start.first][start.second+2] 
	      		&& board[start.first][start.second+3] && board[start.first][start.second+3]->isRook())
	      {
		      temp = make_pair(start.first, start.second+2);
		      possibleMoves.push_back(temp);
	      }

	      if(!board[start.first][start.second-1] && !board[start.first][start.second-2]
	      		&& !board[start.first][start.second-3] && board[start.first][start.second-4]
			&& board[start.first][start.second-4]->isRook())
	      {
		      temp = make_pair(start.first, start.second-2);
		      possibleMoves.push_back(temp);
	      }
	}

	return possibleMoves;
}



vector<square> knight::move(square start, const vector<vector<piece*>>& board)
{
	vector<square> possibleMoves;
	square temp;

	//up left
	temp = make_pair(start.first-2, start.second-1);
	if(0 <= temp.first && temp.first < 8 && 0 <= temp.second && temp.second < 8)
		possibleMoveHelper(temp, board, possibleMoves);

	//up right
	temp = make_pair(start.first-2, start.second+1);
	if(0 <= temp.first && temp.first < 8 && 0 <= temp.second && temp.second < 8)
		possibleMoveHelper(temp, board, possibleMoves);

	//left up
	temp = make_pair(start.first-1, start.second-2);
	if(0 <= temp.first && temp.first < 8 && 0 <= temp.second && temp.second < 8)
		possibleMoveHelper(temp, board, possibleMoves);

	//left down
	temp = make_pair(start.first+1, start.second-2);
	if(0 <= temp.first && temp.first < 8 && 0 <= temp.second && temp.second < 8)
		possibleMoveHelper(temp, board, possibleMoves);

	//right up
	temp = make_pair(start.first-1, start.second+2);
	if(0 <= temp.first && temp.first < 8 && 0 <= temp.second && temp.second < 8)
		possibleMoveHelper(temp, board, possibleMoves);

	//right down
	temp = make_pair(start.first+1, start.second+2);
	if(0 <= temp.first && temp.first < 8 && 0 <= temp.second && temp.second < 8)
		possibleMoveHelper(temp, board, possibleMoves);

	//down left
	temp = make_pair(start.first+2, start.second-1);
	if(0 <= temp.first && temp.first < 8 && 0 <= temp.second && temp.second < 8)
		possibleMoveHelper(temp, board, possibleMoves);

	//down right
	temp = make_pair(start.first+2, start.second+1);
	if(0 <= temp.first && temp.first < 8 && 0 <= temp.second && temp.second < 8)
		possibleMoveHelper(temp, board, possibleMoves);

	return possibleMoves;
}



vector<square> bishop::move(square start, const vector<vector<piece*>>& board)
{
	vector<square> possibleMoves;
	moveDiagonol(start, board, possibleMoves);
	return possibleMoves;
}



//Rooks can move any number of spaces on a rank or a file. Only verticle and Horizontal,
//not diagonol.
vector<square> rook::move(square start, const vector<vector<piece*>>& board)
{
	vector<square> possibleMoves;

	//find all the possible moves the rook can do on the file
	moveFile(start, board, possibleMoves);

	//find all the possible moves the rook can do on the rank.
	moveRank(start, board, possibleMoves);

	return possibleMoves;
}


bool piece::possibleMoveHelper(square temp, const vector<vector<piece*>>&board, vector<square>& possibleMoves)
{
	if(board[temp.first][temp.second])
	{
		//The piece blocking is the opposite color, therefore we can capture.
		if(board[temp.first][temp.second]->getColor() != getColor())
		{
			possibleMoves.push_back(temp);
		}

		//The piece blocking is just our piece so we cant move here :(
		return true;
	}

	else
	{
		possibleMoves.push_back(temp);
		return false;
	}

}


void piece::moveFile(square start, const vector<vector<piece*>>&board, vector<square>& possibleMoves)
{
	//basically blocker tells us if there is a piece somewhere on the file,
	//that blocks possible moves passed it.
	bool blocker = false;

	//move up the file
	for(int i = start.first+1; i < 8; ++i)
	{
		if(!blocker)
		{
			square temp = make_pair(i,start.second);	
			blocker = possibleMoveHelper(temp, board, possibleMoves);
		}
	}

	//reset the flag since we are going in the other direction now.
	blocker = false;


	//move down the file
	for(int i = start.first-1; i >= 0; --i)
	{
		if(!blocker)
		{
			square temp = make_pair(i,start.second);	
			blocker = possibleMoveHelper(temp, board, possibleMoves);
		}
	}
	return;	
}



void piece::moveRank(square start, const vector<vector<piece*>>&board, vector<square>& possibleMoves)
{
	bool blocker = false;

	for(int i = start.second+1; i < 8; ++i)
	{	
		if(!blocker)
		{
			square temp = make_pair(start.first, i);	
			blocker = possibleMoveHelper(temp, board, possibleMoves);
		}
	}

	blocker = false;

	for(int i = start.second-1; i >=0; --i)
	{
		if(!blocker)
		{
			square temp = make_pair(start.first, i);	
			blocker = possibleMoveHelper(temp, board, possibleMoves);
		}
	}
	return;
}



//Check all possible moves on the diagonols given a particular starting square.
//This method is used by bishops and queens.
void piece::moveDiagonol(square start, const vector<vector<piece*>>& board, vector<square>& possibleMoves)
{
	bool blocker = false;
	int i = start.first;
	int j = start.second;

	//down and right
	while(i < 7 && j < 7)
	{
		++i;
		++j;
		if(!blocker)
		{
			square temp = make_pair(i, j);	
			blocker = possibleMoveHelper(temp, board, possibleMoves);
		}
	}

	//reset variables because we are going checking the squares in a diff
	//direction
	i = start.first;
	j = start.second;
	blocker = false;

	//down and left
	while(i < 7 && j > 0)
	{
		++i;
		--j;
		if(!blocker)
		{
			square temp = make_pair(i, j);	
			blocker = possibleMoveHelper(temp, board, possibleMoves);
		}
	}

	i = start.first;
	j = start.second;
	blocker = false;

	//up and right
	while(i > 0 && j < 7)
	{
		--i;
		++j;
		if(!blocker)
		{
			square temp = make_pair(i, j);	
			blocker = possibleMoveHelper(temp, board, possibleMoves);
		}
	}

	i = start.first;
	j = start.second;
	blocker = false;

	//up and left
	while(i > 0 && j > 0)
	{
		--i;
		--j;
		if(!blocker)
		{
			square temp = make_pair(i, j);	
			blocker = possibleMoveHelper(temp, board, possibleMoves);
		}
	}
}
