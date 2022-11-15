#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <fstream>
#include <windows.h>

using std::cout; using std::cin; using std::string; using std::vector; using std::ifstream; using std::ofstream; using std::ws; using std::getline;

/*
* written by chapel1337
* started on 11/13/2022, finished on 11/14/2022
* wrote the text editing stuff on 11/13/2022, finished everything else on 11/14/2022
* first time using multiple keys for navigation
* probably wrote one of the longest function i've ever wrote (see saveLoadTextFile)
* i think the saving and loading is as best as it can get, i could add unpresent file protections
* not much else to say about this, it's just a normal text editor
*/

ifstream fileInput{};
ofstream fileOutput{};

vector<string> currentParagraph{};
string currentLine{};

int wordCount{};
int characterCount{};
int pointerPosition{};

string textFileName{};
string textFileDirectory{};

bool correctionsEnabled{};
bool fileSaved{};
bool saved{};

// ------- \\

void clear()
{
	system("cls");
}

void title(string title)
{
	SetConsoleTitleA((title + " - text editor").c_str());
}

void title()
{
	if (saved)
	{
		SetConsoleTitleA((textFileName + " - text editor").c_str());
	}
	else
	{
		if (fileSaved)
		{
			SetConsoleTitleA(("*" + textFileName + " - text editor").c_str());
		}
		else
		{
			SetConsoleTitleA(("*blank.txt" + textFileName + " - text editor").c_str());
		}
	}
}

void setTextColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void resetTextColor()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void refresh();

// ------- \\

void help()
{
	title("help");
	clear();

	cout << "help\n\n";

	cout << "the pointer is where you type your text, its position is indicated by the ^ below\n";
	cout << "to move the pointer backwards, press -\n";
	cout << "to move the pointer forwards, press +\n\n";

	cout << "to save your text to a file, press esc + s\n";
	cout << "to load a text file, press esc + l\n";
	cout << "to go to help, press esc + h\n\n";

	cout << "for everything else, this text editor works like your average one\n\n";

	cin.get();
	refresh();
}

void quickSave()
{
	fileOutput.open(textFileDirectory + textFileName);

	if (currentParagraph.size() == 0)
	{
		fileOutput << currentLine;
	}
	else
	{
		for (int i{ 0 }; i < currentParagraph.size(); ++i)
		{
			fileOutput << currentParagraph[i] << '\n';
		}
		fileOutput << currentLine;
	}

	fileOutput.close();

	Beep(225, 200);
	refresh();
}

string textFileNameInput{};
string textFileDirectoryInput{};

void saveLoadTextFile(int position, bool alreadyInput, bool saveLoad) // save = true, false = load
{
	clear();
	if (saveLoad) title("save text file"); else title("load text file");

	if (currentParagraph.size() == 0 && currentLine.length() == 0)
	{
		if (saveLoad) cout << "  there is no text to save!\n"; else cout << "  there is no text to load!\n";

		cin.ignore();
		refresh();
	}

	if (alreadyInput)
	{
		if (saveLoad) cout << "  save text file\n\n"; else cout << "  load text file\n\n";

		if (saveLoad)
		{
			for (int i{ 0 }; i < currentParagraph.size(); ++i)
			{
				cout << "  " << currentParagraph[i] << '\n';
			}
			cout << "  " << currentLine << '\n';
		}

		cout << "\n  (please to include the file extension)\n";
		if (saveLoad) cout << "  what would you like to name your text file?\n\n"; else cout << "  what is the name of your text file?\n\n";

		cout << "  > ";
		getline(cin >> ws, textFileNameInput);

		clear();

		if (saveLoad) cout << "  save text file\n\n"; else cout << "  load text file\n\n";

		if (saveLoad)
		{
			for (int i{ 0 }; i < currentParagraph.size(); ++i)
			{
				cout << "  " << currentParagraph[i] << '\n';
			}
			cout << "  " << currentLine << '\n';
		}

		if (saveLoad) cout << "\n  what directory do you want your text file to be in?\n\n"; else cout << "\n  what directory is your file located in?\n\n";

		cout << "  > ";
		getline(cin >> ws, textFileDirectoryInput);

		if (textFileDirectoryInput[2] != '\\')
		{
			textFileDirectoryInput = "C:\\" + textFileDirectoryInput; // assuming that the user wants it in the C:\ drive
		}
		else if (textFileDirectoryInput[textFileDirectoryInput.length() - 1] != '\\')
		{
			textFileDirectoryInput += '\\';
		}
	}

	clear();

	if (saveLoad) cout << "  save text file\n\n"; else cout << "  load text file\n\n";

	if (saveLoad)
	{
		for (int i{ 0 }; i < currentParagraph.size(); ++i)
		{
			cout << "  " << currentParagraph[i] << '\n';
		}
		cout << "  " << currentLine << '\n';
	}

	cout << "\n  file name: " << textFileNameInput << '\n';
	cout << "  file directory: " << textFileDirectoryInput << "\n\n";

	cout << "  are you satisfied with this?\n\n";
	
	if (position == 1)
	{
		cout << " > yes <\n";
	}
	else
	{
		cout << " yes\n";
	}
	if (position == 2)
	{
		cout << "  > no <\n";
	}
	else
	{
		cout << "  no\n";
	}

	int input{ _getch() };

	if (input == 'w' && position != 1)
	{
		saveLoadTextFile(position - 1, false, saveLoad);
	}
	else if (input == 's' && position != 2)
	{
		saveLoadTextFile(position + 1, false, saveLoad);
	}
	else if (input == 13)
	{
		if (position == 1)
		{
			if (saveLoad)
			{
				textFileDirectory = textFileDirectoryInput;
				textFileName = textFileNameInput;

				fileOutput.open(textFileDirectory + textFileName);

				for (int i{ 0 }; i < currentParagraph.size(); ++i)
				{
					fileOutput << currentParagraph[i] << '\n';
				}
				fileOutput << currentLine;

				fileOutput.close();
			}
			else
			{
				string line{};

				currentParagraph.clear();
				textFileDirectory = textFileDirectoryInput;
				textFileName = textFileNameInput;

				fileInput.open(textFileDirectory + textFileName);

				while(getline(fileInput, line))
				{
					currentParagraph.push_back(line);
				}

				currentLine = currentParagraph[currentParagraph.size() - 1];
				currentParagraph.pop_back();
				pointerPosition = currentLine.length();

				fileInput.close();
			}
		}

		fileSaved = true;
		saved = true;

		textFileDirectoryInput.clear();
		textFileNameInput.clear();

		refresh();
	}
	else
	{
		saveLoadTextFile(position, false, saveLoad);
	}
}

/*
void saveMenu(int position)
{
	clear();

	cout << "  save menu\n\n";

	if (position == 1)
	{
		cout << "  > save to text file <\n";
	}
	else
	{
		cout << "  save to text file\n";
	}

	if (position == 2)
	{
		cout << "  > load text file <\n";
	}
	else
	{
		cout << "  load text file\n";
	}

	if (position == 3)
	{
		cout << "  > exit <\n";
	}
	else
	{
		cout << "  exit\n";
	}

	int input{ _getch() };

	if (input == 'w' && position != 1)
	{
		saveMenu(position - 1);
	}
	else if (input == 's' && position != 3)
	{
		saveMenu(position + 1);
	}
	else if (input == 13)
	{
		switch (position)
		{
		case 1:
			saveTextFile();
			break;

		case 2:
			loadTextFile();
			break;

		case 3:
			refresh();
			break;
		}
	}
	else
	{
		saveMenu(position);
	}
}
*/

// ------- \\

void escHandler()
{
	int input{ _getch() };

	if (input == 'l')
	{
		saveLoadTextFile(1, true, false);
	}
	else if (input == 's')
	{
		if (fileSaved)
		{
			quickSave();
		}
		else
		{
			saveLoadTextFile(1, true, true);
		}
	}
	else if (input == 'h')
	{
		help();
	}
}

void getInput(bool loop)
{
	int input{ _getch() };

	if (input == 13 || currentLine.length() >= 95)
	{
		currentParagraph.push_back(currentLine);

		if (pointerPosition != currentLine.length())
		{
			currentLine.erase(pointerPosition, currentLine.length());
		}
		else
		{
			currentLine.clear();
		}

		pointerPosition = 0;
	}
	else if (input == '-' && pointerPosition != 0)
	{
		--pointerPosition;
	}
	else if (input == '+' && pointerPosition < currentLine.length())
	{
		++pointerPosition;
	}
	else if (input == 8)
	{
		if (pointerPosition == 0 && currentParagraph.size() != 0)
		{
			currentLine = currentParagraph[currentParagraph.size() - 1];
			currentParagraph.pop_back();

			pointerPosition = currentLine.length();
		}
		else if (pointerPosition != 0)
		{
			currentLine.erase(currentLine.begin() + pointerPosition - 1);
			--pointerPosition;
		}
	}
	else if (input == 9)
	{
		for (int i{ 0 }; i <= 6; ++i)
		{
			currentLine.insert(currentLine.begin() + pointerPosition, ' ');
			++pointerPosition;
		}
	}
	else if (input == 27)
	{
		loop = false;

		escHandler();
	}
	else if (input != '-' && input != '+' && input != 8 && input != 9 && input != 127)
	{
		currentLine.insert(currentLine.begin() + pointerPosition, input);
		++pointerPosition;
	}

	if (fileSaved)
	{
		saved = false;
	}

	if (loop)
	{
		refresh();
		getInput(true);
	}
}

void displayStats()
{
	cout << "total word count: " << wordCount << '\n';
	if (characterCount != 1)
	{
		cout << "total character count: " << characterCount << "\n\n";
	}
	else
	{
		cout << "total character count: 0\n\n";
	}

	cout << "pointer position: " << pointerPosition << "\n\n";
}

void refresh()
{
	clear();
	displayStats();
	title();

	characterCount = 0;
	wordCount = 0;

	for (int i{ 0 }; i < currentLine.length(); ++i)
	{
		if (currentLine[i] == ' ' && currentLine[i - 1] != ' ' && i != 0)
		{
			++wordCount;
		}
	}

	for (int i{ 0 }; i < currentParagraph.size(); ++i)
	{
		for (int o{ 0 }; o < currentParagraph[i].size(); ++o)
		{
			cout << currentParagraph[i][o];

			++characterCount;
		}

		cout << '\n';
	}
	characterCount += currentLine.length() + 1;

	cout << currentLine << '\n';

	for (int i{ 0 }; i < pointerPosition; ++i)
	{
		cout << ' ';
	}
	cout << '^';

	getInput(true);
}

// ------- \\

int main()
{
	setTextColor(6);
	cout << "written by chapel1337\n";
	cout << "started on 11/13/2022, finished on 11/14/2022\n\n";
	resetTextColor();
	
	setTextColor(4);
	cout << "press esc + h for help\n";
	resetTextColor();

	Sleep(2000);
	Beep(200, 325);

	refresh();
	getInput(true);
}