#ifndef NQUEENS_HH
#define NQUEENS_HH

namespace nqueens
{

const int BACKTRACK = 0;
const int FORWARD = 1;

void solve(int n, int method, int& visited, int& solutions);

}

#endif
