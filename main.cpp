#include "piece.h"
#include <iostream>
#include <string.h>
using namespace std;



void menu();



int main()
{
	//menu();
	game obj = game();
	obj.play();
	return 1;
}



void menu()
{
	char temp[100];
	while(strcmp(temp, "4") != 0)
	{
		cout << "1. How to play. " << '\n'
		     << "2. Rules. " << '\n'
		     << "3. Start game. " << '\n'
		     << "4. Quit. " << '\n'
		     << "Enter choice: ";
		cin.get(temp, 100);
		cin.ignore(100, '\n');

		if(strcmp(temp, "1") == 0)
		{
			cout << "how to play!" << endl;
		}

		else if(strcmp(temp, "2") == 0)
		{
			cout << "how to play!" << endl;
		}

		else if(strcmp(temp, "3") == 0)
		{
			game obj = game();
			obj.play();
		}
		else if(strcmp(temp, "4") == 0)
		{
			cout << "Thanks for playing!" << endl;
		}

		else
		{
			cout << "Invalid choice, try again. " << endl;
		}
	}
	return;
}
