#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

#include "nqueens.hh"

namespace nqueens
{

void pvec(std::vector<int>& v)
{
	for (auto i : v) {
		std::cerr << i << " ";
	}
	std::cerr << "\n";
}

struct board
{
	int n, queens = 0, visited = 0, solutions = 0;
	std::vector<bool> b;
	std::vector<int> columns, rows, diagl, diagr;

	board(int n) : 
		n(n), 
		b(n * n, 0), 
		columns(n, 0), 
		rows(n, 0), 
		diagl(2*n-1, 0), 
		diagr(2*n-1, 0) 
	{}

	inline int dl(int x, int y)
	{
		return n - 1 + x - y;
	}

	inline int dr(int x, int y)
	{
		return x + y;
	}

	inline int pos(int x, int y)
	{
		return x + n * y;
	}

	inline bool empty(int x, int y)
	{
		return !(b[x + n * y]);
	}

	void put(int x, int y)
	{
		b[pos(x, y)] = 1;
		++columns[x];
		++rows[y];
		++diagl[dl(x, y)];
		++diagr[dr(x, y)];
	}

	void remove(int x, int y)
	{
		b[pos(x, y)] = 0;
		--columns[x];
		--rows[y];
		--diagl[dl(x, y)];
		--diagr[dr(x, y)];
	}

	bool is_valid_after(int x, int y)
	{
		return columns[x] == 1 
			&& rows[y] == 1 
			&& diagl[dl(x, y)] == 1 
			&& diagr[dr(x, y)] == 1;
	}

	bool can_put(int x, int y)
	{
		return columns[x] == 0 
			&& rows[y] == 0 
			&& diagl[dl(x, y)] == 0 
			&& diagr[dr(x, y)] == 0;
	}
};

void put_backtrack(board& b, int sx, int sy)
{
	if (b.queens == b.n) {
		++b.solutions;
		return;
	}

	for (int x = sx; x < b.n; ++x) {
		for (int y = sy; y < b.n; ++y) {
			if (b.empty(x, y)) {
				++b.visited;
				b.put(x, y);
				if (b.is_valid_after(x, y)) {
					//std::cerr << "valid " << x << "," << y << "\n";
					++b.queens;
					put_backtrack(b, x, y);
					--b.queens;
				}
				b.remove(x, y);
			}
		}
		sy = 0;
	}
}

void put_forward(board& b, int sx, int sy)
{
	if (b.queens == b.n) {
		++b.solutions;
		return;
	}

	for (int x = sx; x < b.n; ++x) {
		for (int y = sy; y < b.n; ++y) {
			if (b.can_put(x, y)) {
				++b.visited;
				b.put(x, y);
				++b.queens;
				put_forward(b, x, y);
				--b.queens;
				b.remove(x, y);
			}
		}
		sy = 0;
	}
}

void solve(int n, int method, int& visited, int& solutions)
{
	std::function<void(board&, int, int)> solver;
	switch (method) {
	case BACKTRACK:
		solver = put_backtrack;
		break;
	case FORWARD:
		solver = put_forward;
		break;
	default:
		throw std::runtime_error("Unknown solving method");
	}

	board b(n);
	solver(b, 0, 0);

	visited = b.visited;
	solutions = b.solutions;
}

}
