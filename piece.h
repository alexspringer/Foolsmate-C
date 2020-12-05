#ifndef  PIECE_H
#define PIECE_H
#include <vector>
#include <iostream>
#include <string>
#include <utility>
#include <list>

using namespace std;

using square = pair<int, int>;

//Interface for all pieces in chess.
class piece
{
	public:
		piece(int c) : color(c) {}
		virtual vector<square> move(square, const vector<vector<piece*>>&) = 0;
		bool possibleMoveHelper(square, const vector<vector<piece*>>&, vector<square>&);
		void moveRank(square, const vector<vector<piece*>>&, vector<square>&);
		void moveFile(square, const vector<vector<piece*>>&, vector<square>&);
		void moveDiagonol(square, const vector<vector<piece*>>&, vector<square>&);
		virtual void display() {std::cout<< " ";}

		/*helper function to determine if a piece is a pawn.
		  it is super handy for pawn promotion or when a pawn
		  is performing an en passant */
		virtual bool isPawn() { return false; }
		virtual bool getEnPassantFlag() { return false; }
		virtual void setEnPassantFlag(bool x) {}

		/*Helper functions to determine if the piece we are moving is
		  a king. Likewise, in order to castle we need to know if the piece on the 
		  edge of the board is a rook. We need to know this to handle castling rights.
		  since in this move it actually moves both the rook and the king,
		  it cannot be done solely in the move method for a king. */
		virtual bool isKing() { return false; }
		virtual bool getCastlingRights() { return false; }
		virtual void setCastlingRights(bool x) {}
		virtual bool isRook() { return false; }

		//setters and getters for the pieces color.
		int getColor() { return color; }
		void setColor(int c) { color = c; }
		void displayColor() { if(color == 0) cout << "W "; else cout << "B ";}
	private:
		int color;
};


class king : public piece
{
	public:
		king(int color) : piece(color) { castlingRights = true; }
		void display() { std::cout<< "K ";}
		vector<square> move(square start, const vector<vector<piece*>>& board);
		bool isKing() { return true; }
		void setCastlingRights(bool x) { castlingRights = x; }
		bool getCastlingRights() { return castlingRights; }
	private:
		bool castlingRights;
};

class queen : public piece
{
	public:
		queen(int color) : piece(color) {}
		void display() { std::cout<< "Q ";}
		vector<square> move(square start, const vector<vector<piece*>>& board);
	private:
};

class rook : public piece
{
	public:
		rook(int color) : piece(color) {}
		bool isRook() { return true; }
		void display() { std::cout<< "R ";}
		vector<square> move(square start, const vector<vector<piece*>>& board);
	private:
};

class bishop : public piece
{
	public:
		bishop(int color) : piece(color) {}
		void display() { std::cout<< "B ";}
		vector<square> move(square start, const vector<vector<piece*>>& board);
	private:
};

class knight : public piece
{
	public:
		knight(int color) : piece(color) {}
		void display() { std::cout<< "H ";}
		vector<square> move(square start, const vector<vector<piece*>>& board);
	private:
};

class pawn : public piece
{
	public:
		pawn(int color) : piece(color) { enPassantFlag = false; }
		void display() { std::cout<< "P ";}
		//Return a vector of all possible moves
		vector<square> move(square start, const vector<vector<piece*>>& board);
		bool isPawn() { return true; }
		bool getEnPassantFlag() { return enPassantFlag; }
		void setEnPassantFlag(bool x) { enPassantFlag = x; }
	private:
		bool enPassantFlag;
};

class game
{
	public:
		game();
		~game();
		void deleteBoard();
		void play();
		int movePiece(square, int);
		void displayBoard();
		void displayPossibleMoves(square, const vector<square>&);
		vector<vector<piece*>> getBoard() { return board; }
		square pickMove(const vector<square>&);
		int playerTurn(int);

		//special cases for pawns
		void enPassant(square, square);
		void pawnPromotion(square, square);

		//special move for king
		void castle(square, square);

		//Functions that help detecting checks and checkmates
		square findKing(int);
		bool canSaveKing(square, int);
		bool canMoveKing(square);
		bool scanForCheck(square);
		bool scanForMate(square, int);

	private:
		vector<vector<piece*>> board;
};

#endif // PIECE_H
