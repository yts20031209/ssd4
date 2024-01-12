#include "inegamax.h"

INegaMax::INegaMax(QObject *parent)
    : ISearchEngine(parent)
{
}

INegaMax::~INegaMax()
{
}

qint32 INegaMax::negaMax(qint32 searchDepth, FACTION currentCamp)
{
    if (searchDepth == 0)
        return calScore(currentCamp);

    qint32 currentScore = MINIMUM_VALUE;
    for (Stone* pPiece : m_operatePieceList)
    {
        if (pPiece->getFac() != currentCamp || pPiece->isDead)
            continue;

        QList<Step*> chessStepList = pPiece->allPossibleSteps();
        while(!chessStepList.isEmpty())
        {
            m_stepCount++;
            Step* pChessStep = chessStepList.back();
            chessStepList.removeLast();

            fakeMove(pChessStep);
            qint32 score = -negaMax(searchDepth - 1, nextCamp(currentCamp));
            unFakeMove(pChessStep);

            bool condition = score > currentScore;
            if (condition) currentScore = score;

            if (condition && searchDepth == m_searchDepth)
            {
                if (m_pBestStep) delete m_pBestStep;
                m_pBestStep = pChessStep;
            }
            else
            {
                delete pChessStep;
            }
        }
    }

    return currentScore;
}

void INegaMax::search()
{
    negaMax(m_searchDepth, m_camp);
}
