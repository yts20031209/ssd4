#include "iminiwindowsearch.h"

IMiniWindowSearch::IMiniWindowSearch(QObject *parent)
    : ISearchEngine(parent)
{
}

IMiniWindowSearch::~IMiniWindowSearch()
{
}

qint32 IMiniWindowSearch::miniWindowSearch(qint32 searchDepth, qint32 alpha, qint32 beta)
{
	bool isFirstChessStep = true;
    qint32 bestScore = 0;
    for (quint8 index(0);index < 16;index++)
	{
        if (m_operatePieceList[index]->isDead)
			continue;

        QList<Step*> chessStepList = m_operatePieceList[index]->allPossibleSteps();
        while (!chessStepList.isEmpty())
        {
            m_stepCount++;
            Step* pChessStep = chessStepList.back();
            chessStepList.removeLast();

            bool record = false;
            if (isFirstChessStep)
            {
                isFirstChessStep = false;
                fakeMove(pChessStep);
                bestScore = -miniWindowBeta(searchDepth - 1, -beta, -alpha);
                unFakeMove(pChessStep);

                if (m_pBestStep) delete m_pBestStep;
                m_pBestStep = pChessStep;
                record = true;
            }
            else if(bestScore < beta)
            {
                if (bestScore > alpha) alpha = bestScore;

                fakeMove(pChessStep);
                qint32 score = -miniWindowBeta(searchDepth - 1, -alpha - 1, -alpha);

                if (score > alpha && score < beta)
                {
                    bestScore = -miniWindowBeta(searchDepth - 1, -beta, -score);
                    if (m_pBestStep) delete m_pBestStep;
                    m_pBestStep = pChessStep;
                    record = true;
                }
                else if (score > bestScore)
                {
                    bestScore = score;
                    if (m_pBestStep) delete m_pBestStep;
                    m_pBestStep = pChessStep;
                    record = true;
                }

                unFakeMove(pChessStep);
            }
            
            if (!record)
            {
                delete pChessStep;
            }
        }
	}

	return bestScore;
}

qint32 IMiniWindowSearch::miniWindowAlpha(qint32 searchDepth, qint32 alpha, qint32 beta)
{
    if (searchDepth == 0)
        return calScore(m_camp);

    bool isFirstChessStep = true;
    qint32 bestScore = 0;
    for (quint8 index(0); index < 16; index++)
    {
        if (m_operatePieceList[index]->isDead)
            continue;

        QList<Step*> chessStepList = m_operatePieceList[index]->allPossibleSteps();
        while (!chessStepList.isEmpty())
        {
            m_stepCount++;
            Step* pChessStep = chessStepList.back();
            chessStepList.removeLast();

            if (isFirstChessStep)
            {
                isFirstChessStep = false;
                fakeMove(pChessStep);
                bestScore = -miniWindowBeta(searchDepth - 1, -beta, -alpha);
                unFakeMove(pChessStep);
            }
            else if (bestScore < beta)
            {
                if (bestScore > alpha) alpha = bestScore;

                fakeMove(pChessStep);
                qint32 score = -miniWindowBeta(searchDepth - 1, -alpha - 1, -alpha);

                if (score > alpha&& score < beta)
                {
                    bestScore = -miniWindowBeta(searchDepth - 1, -beta, -score);
                }
                else if (score > bestScore)
                {
                    bestScore = score;
                }

                unFakeMove(pChessStep);
            }

            delete pChessStep;
            pChessStep = Q_NULLPTR;
        }
    }

    return bestScore;
}

qint32 IMiniWindowSearch::miniWindowBeta(qint32 searchDepth, qint32 alpha, qint32 beta)
{
    if (searchDepth == 0)
        return calScore(FACTION::red);

    bool isFirstChessStep = true;
    qint32 bestScore = 0;
    for (quint8 index(16); index < 32; index++)
    {
        if (m_operatePieceList[index]->isDead)
            continue;

        QList<Step*> chessStepList = m_operatePieceList[index]->allPossibleSteps();
        while (!chessStepList.isEmpty())
        {
            m_stepCount++;
            Step* pChessStep = chessStepList.back();
            chessStepList.removeLast();

            if (isFirstChessStep)
            {
                isFirstChessStep = false;
                fakeMove(pChessStep);
                bestScore = -miniWindowAlpha(searchDepth - 1, -beta, -alpha);
                unFakeMove(pChessStep);
            }
            else if (bestScore < beta)
            {
                if (bestScore > alpha) alpha = bestScore;

                fakeMove(pChessStep);
                qint32 score = -miniWindowAlpha(searchDepth - 1, -alpha - 1, -alpha);

                if (score > alpha&& score < beta)
                {
                    bestScore = -miniWindowAlpha(searchDepth - 1, -beta, -score);
                }
                else if (score > bestScore)
                {
                    bestScore = score;
                }

                unFakeMove(pChessStep);
            }

            delete pChessStep;
            pChessStep = Q_NULLPTR;
        }
    }

    return bestScore;
}

void IMiniWindowSearch::search()
{
    miniWindowSearch(m_searchDepth, MINIMUM_VALUE, MAXIMUM_VALUE);
}
