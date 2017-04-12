#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <set>
#include <vector>

#include "colour.hh"

namespace colour
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
	int n, colours, visited = 0, solutions = 0;
	std::vector<int> b;
	std::set<std::pair<int, int>> pairs;

	board(int n) :
		n(n),
		colours(2*n + n%2),
		b(n * n, (-1))
	{
	}

	inline int pos(int x, int y)
	{
		return x + n * y;
	}

	bool is_complete()
	{
		for (int x = 0; x < n; ++x) {
			for (int y = 0; y < n; ++y) {
				if (b[pos(x, y)] == -1) return false;
			}
		}
		return true;
	}

	bool is_valid(int x, int y)
	{
		int c = b[pos(x, y)];
		if (x > 0) {
			int o = b[pos(x - 1, y)];
			if (c == o) return false;
		}
		if (y > 0) {
			int o = b[pos(x, y - 1)];
			if (c == o) return false;
		}
		if (x < n - 1) {
			int o = b[pos(x + 1, y)];
			if (c == o) return false;
		}
		if (y < n - 1) {
			int o = b[pos(x, y + 1)];
			if (c == o) return false;
		}
		std::set<std::pair<int, int>> prs;
		for (int x = 0; x < n; ++x) {
			for (int y = 0; y < n; ++y) {
				c = b[pos(x, y)];
				if (c == -1) continue;
				if (x < n - 1 && b[pos(x + 1, y)] != -1) {
					auto p = colorpair(c, b[pos(x + 1, y)]);
					int size = prs.size();
					prs.emplace(p);
					if (prs.size() == size) return false;
				}
				if (y < n - 1 && b[pos(x, y + 1)] != -1) {
					auto p = colorpair(c, b[pos(x, y + 1)]);
					int size = prs.size();
					prs.emplace(p);
					if (prs.size() == size) return false;
				}
			}
		}
		return true;
	}

	bool isused(int c1, int c2)
	{
		return pairs.find(colorpair(c1, c2)) != pairs.cend();
	}

	std::pair<int, int> colorpair(int c1, int c2)
	{
		if (c1 < c2) {
			return std::make_pair(c1, c2);
		} else {
			return std::make_pair(c2, c1);
		}
	}

	bool can_put(int x, int y, int c)
	{
		if (x > 0) {
			int o = b[pos(x - 1, y)];
			if (c == o || isused(c, o)) return false;
		}
		if (y > 0) {
			int o = b[pos(x, y - 1)];
			if (c == o || isused(c, o)) return false;
		}
		if (x < n - 1) {
			int o = b[pos(x + 1, y)];
			if (c == o || isused(c, o)) return false;
		}
		if (y < n - 1) {
			int o = b[pos(x, y + 1)];
			if (c == o || isused(c, o)) return false;
		}
		return true;
	}

	void colour(int x, int y, int c)
	{
		b[pos(x, y)] = c;
		if (x > 0 && b[pos(x - 1, y)] != -1) {
			pairs.emplace(colorpair(c, b[pos(x - 1, y)]));
		}
		if (y > 0 && b[pos(x, y - 1)] != -1) {
			pairs.emplace(colorpair(c, b[pos(x, y - 1)]));
		}
		if (x < n - 1 && b[pos(x + 1, y)] != -1) {
			pairs.emplace(colorpair(c, b[pos(x + 1, y)]));
		}
		if (y < n - 1 && b[pos(x, y + 1)] != -1) {
			pairs.emplace(colorpair(c, b[pos(x, y + 1)]));
		}
	}

	void uncolour(int x, int y)
	{
		int c = b[pos(x, y)];
		if (x > 0) pairs.erase(colorpair(c, b[pos(x - 1, y)]));
		if (y > 0) pairs.erase(colorpair(c, b[pos(x, y - 1)]));
		if (x < n - 1) pairs.erase(colorpair(c, b[pos(x + 1, y)]));
		if (y < n - 1) pairs.erase(colorpair(c, b[pos(x, y + 1)]));
		b[pos(x, y)] = -1;
	}
};

board b(1);

void put_backtrack(int sx, int sy, int sc)
{
	for (int x = sx; x < b.n; ++x) {
		for (int y = sy; y < b.n; ++y) {
			for (int c = 0; c < b.colours; ++c) {
				++b.visited;
				b.colour(x, y, c);
				if (b.is_valid(x, y)) {
					if (x == b.n - 1 && y == b.n - 1 && b.is_complete()) {
						++b.solutions;
					}
					put_backtrack(x, y + 1, 0);
				}
				b.uncolour(x, y);
			}
		}
		sy = 0;
	}
}

void put_forward(int sx, int sy, int sc)
{
	for (int x = sx; x < b.n; ++x) {
		for (int y = sy; y < b.n; ++y) {
			for (int c = sc; c < b.colours; ++c) {
				if (b.can_put(x, y, c)) {
					++b.visited;
					b.colour(x, y, c);
					put_forward(x, y + 1, 0);
					if (x == b.n - 1 && y == b.n - 1 && b.is_complete()) {
						++b.solutions;
						
					}
					b.uncolour(x, y);
				}
			}
			sc = 0;
		}
		sy = 0;
	}
}

void solve(int n, int method, int& visited, int& solutions)
{
	std::function<void(int, int, int)> solver;
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

	b = board(n);
	solver(0, 0, 0);

	visited = b.visited;
	solutions = b.solutions;
}

}
