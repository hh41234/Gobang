#pragma GCC optimize(2)
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <cstring>
#include <algorithm>
#include "AIController.h"
#include <utility>
using namespace std;
extern int ai_side;
string ai_name = "hh41234";
int turn;
typedef long long ll;
const int point[2][6] = {{0, 10, 100, 1000, 10000, 10000000}, {0, 0, 10, 100, 1000, 10000000}};
const int depth = 10;
const long long ninf = -1000000000000000000LL;
const long long inf = 1000000000000000000LL;
int width[12] = {0, 2, 3, 3, 3, 3, 3, 5, 6, 7, 9};
class Coordinate
{
public:
	int x, y, w;
	Coordinate(int X, int Y) : x(X), y(Y), w(80)
	{
		if (x == 0)
			w -= 60;
		if (x == 14)
			w -= 60;
		if (y == 0)
			w -= 60;
		if (y == 14)
			w -= 60;
		w = max(w, 0);
	}
	Coordinate(int X, int Y, int W) : x(X), y(Y), w(W) {}
	Coordinate operator+(Coordinate &rhs)
	{
		return Coordinate(x + rhs.x, y + rhs.y);
	}
	Coordinate operator-(Coordinate &rhs)
	{
		return Coordinate(x - rhs.x, y - rhs.y);
	}
	Coordinate operator*(int dir)
	{
		return Coordinate(x * dir, y * dir);
	}
	bool operator<(const Coordinate &rhs) const
	{
		if (w != rhs.w)
		{
			return w > rhs.w;
		}
		if (x != rhs.x)
		{
			return x > rhs.x;
		}
		return y > rhs.y;
	}
	bool operator==(const Coordinate &rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}
	bool valid()
	{
		return x < 15 && x >= 0 && y < 15 && y >= 0;
	}
};

Coordinate dir[4] = {Coordinate(1, 0), Coordinate(0, 1), Coordinate(1, 1), Coordinate(1, -1)};

class Board
{
public:
	int board[15][15];
	Board()
	{
		memset(board, -1, sizeof(board));
	}
	int operator[](const Coordinate &coor)
	{
		return board[coor.x][coor.y];
	}
	void modify(const Coordinate &coor, int color)
	{
		board[coor.x][coor.y] = color;
	}
};

struct gen
{
	int l, r;
	bool lblank, rblank;
	gen() : l(0), r(0), lblank(true), rblank(true) {}
	int len()
	{
		if (l + r >= 5)
			return 5;
		return l + r + 1;
	}
	int defence_len()
	{
		return l + r;
	}
	int attack_gen()
	{
		if (len() == 5)
			return 10000000;
		if (!lblank && !rblank)
			return 0;
		return point[lblank ^ rblank][len()];
	}
	int defence_gen()
	{
		if (!lblank && !rblank)
			return 0;
		return point[lblank ^ rblank][defence_len()];
	}
};

class AI
{
public:
	Board board;
	set<Coordinate> blank;
	gen status[15][15][2][4];
	AI()
	{
		for (int i = 0; i < 15; ++i)
			for (int j = 0; j < 15; ++j)
			{
				blank.insert(Coordinate(i, j));
			}
		for (int i = 0; i < 15; ++i)
		{
			for (int color = 0; color < 2; ++color)
			{
				status[0][i][color][0].lblank = status[0][i][color][2].lblank = status[0][i][color][3].lblank = status[14][i][color][0].rblank = status[14][i][color][2].rblank = status[14][i][color][3].rblank = status[i][0][color][1].lblank = status[i][0][color][2].lblank = status[i][14][color][3].lblank = status[i][14][color][1].rblank = status[i][14][color][2].rblank = status[i][0][color][3].rblank = false;
			}
		}
	}
	int get_gen(int x, int y)
	{
		int ret = 0;
		if (x == -1)
			return 0;
		for (int color = 0; color < 2; ++color)
			for (int dir = 0; dir < 4; ++dir)
			{
				ret += status[x][y][color][dir].attack_gen();
			}
		return ret;
	}
	void change(const int &x, const int &y, int d)
	{
		for (int color = 0; color < 2; ++color)
		{
			status[x][y][color][d].l = status[x][y][color][d].r = 0;
			Coordinate rtmp = Coordinate(x, y) + dir[d];
			while (rtmp.valid() && board[rtmp] == color)
			{
				++status[x][y][color][d].r;
				rtmp = rtmp + dir[d];
			}
			Coordinate ltmp = Coordinate(x, y) - dir[d];
			while (ltmp.valid() && board[ltmp] == color)
			{
				++status[x][y][color][d].l;
				ltmp = ltmp - dir[d];
			}
			status[x][y][color][d].lblank = ltmp.valid() && board[ltmp] == -1;
			status[x][y][color][d].rblank = rtmp.valid() && board[rtmp] == -1;
		}
	}
	int get_gen(int x, int y, int color)
	{
		int ret = 0;
		if (x == -1)
			return 0;
		for (int dir = 0; dir < 4; ++dir)
		{
			ret += status[x][y][1 - color][dir].defence_gen();
		}

		for (int dir = 0; dir < 4; ++dir)
		{
			ret += status[x][y][color][dir].attack_gen();
		}
		return ret;
	}
	void change(const int &x, const int &y)
	{
		for (int i = max(0, x - 5); i <= min(14, x + 5); ++i)
		{
			for (int j = max(0, y - 5); j <= min(14, y + 5); ++j)
			{
				if (board.board[i][j] == -1)
				{
					blank.erase(Coordinate(i, j, get_gen(i, j)));
					for (int k = 0; k < 4; ++k)
						change(i, j, k);
					blank.insert(Coordinate(i, j, get_gen(i, j)));
				}
			}
		}
	}
	ll minimax(int depth, ll alpha, ll beta, int color, Coordinate &ans, int *width)
	{
		if (depth == 0)
			return 0;
		ll value;
		if (color == ai_side)
		{
			value = ninf;
			auto it = blank.begin();
			for (int i = 0; i < width[depth] && i <= int(blank.size()); ++i, ++it)
			{
				Coordinate cur = *it;
				ll tmp = get_gen(cur.x, cur.y, color);
				if (tmp >= int(1e7))
				{
					value = inf;
					ans = cur;
					break;
				}
				blank.erase(it);
				board.modify(cur, color);
				change(cur.x, cur.y);
				Coordinate move(-1, -1);
				tmp += minimax(depth - 1, alpha - tmp, beta - tmp, 1 - color, move, width);
				if (tmp > value)
				{
					value = tmp;
					ans = cur;
				}
				blank.insert(cur);
				board.modify(cur, -1);
				change(cur.x, cur.y);
				it = blank.find(cur);
				alpha = max(value, alpha);
				if (alpha >= beta)
					return inf;
			}
			return value;
		}
		else
		{
			value = inf;
			set<Coordinate>::iterator it = blank.begin();
			for (int i = 0; i < width[depth] && i <= int(blank.size()); ++i, ++it)
			{
				Coordinate cur = *it;
				Coordinate coor(-1, -1);
				ll tmp = -get_gen(cur.x, cur.y, color);
				if (tmp <= int(-1e7))
				{
					value = ninf;
					ans = cur;
					break;
				}
				blank.erase(it);
				board.modify(cur, color);
				change(cur.x, cur.y);
				tmp += minimax(depth - 1, alpha - tmp, beta - tmp, 1 - color, coor, width);
				if (tmp < value)
				{
					value = tmp;
					ans = cur;
				}
				blank.insert(cur);
				board.modify(cur, -1);
				change(cur.x, cur.y);
				it = blank.find(cur);
				beta = min(value, beta);
				if (alpha >= beta)
					return ninf;
			}
			return value;
		}
	}
	void flip()
	{
		for (int i = 0; i < 15; ++i)
			for (int j = 0; j < 15; ++j)
				if (board.board[i][j] >= 0)
				{
					board.board[i][j] = 1 - board.board[i][j];
					change(i, j);
				}
	}
	ll value_flip(int color)
	{
		ll value = 0;
		for (int i = 0; i < 15; ++i)
			for (int j = 0; j < 15; ++j)
			{
				if (board.board[i][j] != -1)
				{
					for (int d = 0; d < 4; ++d)
						change(i, j, d);
					if (board.board[i][j] == color)
					{
						value += get_gen(i, j);
					}
					else
						value -= get_gen(i, j);
				}
			}
		if (value > 100)
			value -= 100;
		value <<= 1;
		return -value;
	}
	pair<int, int> WhiteChange()
	{
		Coordinate coor(-1, -1);
		if (value_flip(ai_side) - minimax(depth - 1, inf, ninf, ai_side, coor, width) < minimax(depth, inf, ninf, ai_side, coor, width))
		{
			board.modify(coor, ai_side);
			blank.erase(Coordinate(coor.x, coor.y, get_gen(coor.x, coor.y)));
			change(coor.x, coor.y);
			return make_pair(coor.x, coor.y);
		}
		else
		{
			flip();
			return make_pair(-1, -1);
		}
	}
	pair<int, int> BlackChange()
	{
		board.modify(Coordinate(0, 1), ai_side);
		blank.erase(Coordinate(0, 1, get_gen(0, 1)));
		change(0, 1);
		return make_pair(0, 1);
	}
};

AI mine;

void init()
{
	turn = 0;
}

pair<int, int> action(pair<int, int> loc)
{
	++turn;
	if (loc.first == -1)
		mine.flip();
	else
	{
		mine.board.modify(Coordinate(loc.first, loc.second), 1 - ai_side);
		mine.blank.erase(Coordinate(loc.first, loc.second, mine.get_gen(loc.first, loc.second)));
		mine.change(loc.first, loc.second);
	}
	if (turn == 1 && ai_side == 0)
	{
		mine.board.modify(Coordinate(7, 7), ai_side);
		mine.blank.erase(Coordinate(7, 7));
		mine.change(7, 7);
		return make_pair(7, 7);
	}

	if (turn == 1 && ai_side == 1)
	{
		mine.board.modify(Coordinate(0, 0), ai_side);
		mine.blank.erase(Coordinate(0, 0));
		mine.change(0, 0);
		return make_pair(0, 0);
	}
	if (turn == 2 && ai_side == 1)
	{
		return mine.WhiteChange();
	}
	if (turn == 2 && ai_side == 0)
	{
		return mine.BlackChange();
	}
	if (turn == 3 && ai_side == 0)
	{
		mine.board.modify(Coordinate(8, 6), ai_side);
		mine.blank.erase(Coordinate(8, 6, mine.get_gen(8, 6)));
		mine.change(8, 6);
		return make_pair(8, 6);
	}
	Coordinate bestmove(0, 0);
	for (int i = 6; i <= depth; i += 2)
	{
		if (mine.minimax(i, ninf, inf, ai_side, bestmove, width) > 1000000)
		{
			{
				mine.board.modify(bestmove, ai_side);
				mine.blank.erase(Coordinate(bestmove.x, bestmove.y, mine.get_gen(bestmove.x, bestmove.y)));
				mine.change(bestmove.x, bestmove.y);
				return make_pair(bestmove.x, bestmove.y);
			}
		}
	}
	mine.board.modify(bestmove, ai_side);
	mine.blank.erase(Coordinate(bestmove.x, bestmove.y, mine.get_gen(bestmove.x, bestmove.y)));
	mine.change(bestmove.x, bestmove.y);
	return make_pair(bestmove.x, bestmove.y);
}
