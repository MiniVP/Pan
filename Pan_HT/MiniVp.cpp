#include "MiniVp.h"

MiniVp::MiniVp(int a) : a(a), b(123), c(true)
{
}


MiniVp::~MiniVp()
{
}

int MiniVp::coucouCestNous()
{
	return b;
}

void MiniVp::setB(int cestnous)
{
	b = cestnous;
}