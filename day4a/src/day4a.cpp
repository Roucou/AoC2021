#include <algorithm>
#include <array>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct GridPosition
{
	int col;
	int row;
};

class Board
{
	array<array<int, 5>, 5> _numbers;
	array<array<bool, 5>, 5> _marked;

	void parseLine(string line, array<int, 5> &row)
	{
		for (size_t index = 0; index < 5; index++)
		{
			char digit1 = line.at(index * 3);
			char digit2 = line.at(index * 3 + 1);
			if (digit1 == ' ')
			{
				digit1 = '0';
			}
			row[index] = (digit1 - '0') * 10 + (digit2 - '0');
		}
	}

	public:
	Board(vector<string> &board)
	{
		unmarkAll();
		fromString(board);
	}

	void unmarkAll()
	{
		for (size_t row_count = 0; row_count < 5; row_count++)
		{
			for (size_t col_count = 0; col_count < 5; col_count++)
			{
				_marked[row_count][col_count] = false;
			}
		}
	}

	void fromString(vector<string> &board)
	{
		cout << "New board!" << endl;
		for(size_t line_count = 0; line_count < 5; line_count++)
		{
			string line = board[line_count];
			array<int, 5> row;
			parseLine(line, row);
			_numbers[line_count] = row;
		}
	}

	void print()
	{
		for (size_t row_count = 0; row_count < 5; row_count++)
		{
			array<int, 5> row_numbers = _numbers[row_count];
			array<bool, 5> row_marked = _marked[row_count];
			for (size_t col_count = 0; col_count < 5; col_count++)
			{
				int number = row_numbers[col_count];
				bool marked = row_marked[col_count];
				if (marked)
				{
					cout << '[' << to_string(number) << ']' << ' ';
				}
				else
				{
					cout << to_string(number) << ' ';
				}
			}
			cout << endl;
		}
	}

	int getNumber(GridPosition pos)
	{
		return _numbers[pos.row][pos.col];
	}

	int getNumber(int col, int row)
	{
		return _numbers[row][col];
	}

	bool findNumber(int number, GridPosition &pos)
	{
		for (size_t row = 0; row < 5; row++)
		{
			for (size_t col = 0; col < 5; col++)
			{
				if (_numbers[row][col] == number)
				{
					pos.col = col;
					pos.row = row;
					return true;
				}
			}
		}

		return false;
	}

	void mark(GridPosition pos)
	{
		_marked[pos.row][pos.col] = true;
	}

	bool isMarked(GridPosition pos)
	{
		return _marked[pos.row][pos.col];
	}

	bool isMarked(int col, int row)
	{
		return _marked[row][col];
	}

	void markBall(int number)
	{
		GridPosition pos;

		// Find number
		bool found = findNumber(number, pos);
		if (found)
		{
			mark(pos);
		}
	}

	bool hasWinningColumn()
	{
		for (size_t col = 0; col < 5; col++)
		{
			if (isMarked(col, 0) && isMarked(col, 1) && isMarked(col, 2) && isMarked(col, 3) && isMarked(col, 4))
			{
				return true;
			}	
		}

		return false;
	}

	bool hasWinningRow()
	{
		for (size_t row = 0; row < 5; row++)
		{
			if (isMarked(0, row) && isMarked(1, row) && isMarked(2, row) && isMarked(3, row) && isMarked(4, row))
			{
				return true;
			}	
		}

		return false;
	}

	int calculateSumOfUnmarkedNumbers()
	{
		int sum = 0;
		for (size_t row = 0; row < 5; row++)
		{
			for (size_t col = 0; col < 5; col++)
			{
				if (!isMarked(col, row))
				{
					sum += getNumber(col, row);
				}
			}
		}

		return sum;
	}
};

class Bingo
{
	vector<Board> _boards;
	vector<int> _lottery;

	void parseLottery(string s)
	{
		stringstream ss(s);
		string token;
		while (getline(ss, token, ','))
		{
			int ball = stoi(token);
			_lottery.push_back(ball);
		}
	}

	public:
	void parse(vector<string> &input)
	{
		string s = input.front();
		parseLottery(s);
		input.erase(input.begin());

		int line_count = 0;
		vector<string> lines;
		for (const auto &line: input)
		{
			if (line_count == 0)
			{
				lines.clear();
				line_count++;
				continue; // skip blank line
			}
			lines.push_back(line);
			if (line_count == 5)
			{
				Board board{lines};
				_boards.push_back(board);
				line_count = 0;
				board.print();
				continue;
			}
			line_count++;
		}
	}

	void drawBalls()
	{
		bool we_have_a_winner = false;
		for (const auto &ball: _lottery)
		{
			cout << "Ball: " << to_string(ball) << endl;
			for (auto &board: _boards)
			{
				board.markBall(ball);
				cout << endl;
				board.print();

				// Winner?
				if (board.hasWinningColumn() || board.hasWinningRow())
				{
					we_have_a_winner = true;
					cout << "We have a winner!" << endl;

					// Calculate answer
					int sum = board.calculateSumOfUnmarkedNumbers();
					int answer = sum * ball;
					cout << "Answer: " << to_string(answer) << endl;
					break;
				}
			}

			if (we_have_a_winner)
			{
				break;
			}	
		}
	}
};

int main(int argc, char *argv[])
{
	// First command line argument is the file to open
	if (argc != 2)
	{
		cout << "Usage: day4a <input.txt>" << endl;
		return -1;
	}

	// Open file
	string path = argv[1];
	ifstream file(path);

	// Read file, line per line
	std::string str;
	vector<string> input;
	while (getline(file, str))
	{
		input.push_back(str);
	}

	// And (bin)go!
	Bingo bingo;
	bingo.parse(input);
	bingo.drawBalls();

	return 0;
}

