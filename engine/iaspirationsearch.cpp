#include "iaspirationsearch.h"

IAspirationSearch::IAspirationSearch(QObject *parent)
    : IFailSoftAlphaBeta(parent)
{
}

IAspirationSearch::~IAspirationSearch()
{
}

void IAspirationSearch::search()
{
	qint32 window = 50;
	qint32 searchDepth = m_searchDepth;
	m_searchDepth--;
	qint32 x = failSoftAlphaBeta(m_searchDepth, MINIMUM_VALUE, MAXIMUM_VALUE);
	m_searchDepth = searchDepth;
	qint32 y = failSoftAlphaBeta(m_searchDepth, x - window, x + window);
	if(y < x - window)
		failSoftAlphaBeta(m_searchDepth, MINIMUM_VALUE, y);
	if(y > x + window)
		failSoftAlphaBeta(m_searchDepth, y, MAXIMUM_VALUE);
}
