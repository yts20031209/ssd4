#include "ifailsoftalphabeta.h"

IFailSoftAlphaBeta::IFailSoftAlphaBeta(QObject *parent)
    : ISearchEngine(parent)
{
}

IFailSoftAlphaBeta::~IFailSoftAlphaBeta()
{
}

qint32 IFailSoftAlphaBeta::failSoftAlphaBeta(qint32 searchDepth, qint32 alpha, qint32 beta)
{
    qint32 currentScore(MINIMUM_VALUE);
    qint32 score(0);
    for (quint8 index(0);index < 16;index++)
    {
        if (m_operatePieceList[index]->isDead)
            continue;

        QList<Step*> chessStepList = m_operatePieceList[index]->allPossibleSteps();
        while (!chessStepList.isEmpty())
        {
            m_stepCount++;
            Step* pStep = chessStepList.back();
            chessStepList.removeLast();

            fakeMove(pStep);
            score = -failSoftBeta(searchDepth - 1, -beta, -alpha);
            unFakeMove(pStep);

            if (score > alpha)
            {
                currentScore = score;

                if (score >= alpha) alpha = score;

                if (m_pBestStep) delete m_pBestStep;
                m_pBestStep = pStep;

                if (score >= beta)
                {
                    while (!chessStepList.isEmpty())
                    {
                        Step* pChessStep = chessStepList.back();
                        chessStepList.removeLast();
                        delete pChessStep;
                    }
                    return score;
                }
            }
            else
            {
                delete pStep;
                pStep = Q_NULLPTR;
            }
        }
    }

    return currentScore;
}

qint32 IFailSoftAlphaBeta::failSoftAlpha(qint32 searchDepth, qint32 alpha, qint32 beta)
{
    if (searchDepth == 0)
        return calScore(m_camp);

    qint32 currentScore(MINIMUM_VALUE);
    qint32 score(0);
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

            fakeMove(pChessStep);
            score = -failSoftBeta(searchDepth - 1, -beta, -alpha);
            unFakeMove(pChessStep);

            delete pChessStep;
            pChessStep = Q_NULLPTR;

            if (score > alpha)
            {
                currentScore = score;

                if (score >= alpha) alpha = score;

                if (score >= beta)
                {
                    while (!chessStepList.isEmpty())
                    {
                        Step* pChessStep = chessStepList.back();
                        chessStepList.removeLast();
                        delete pChessStep;
                    }
                    return score;
                }
            }
        }
    }

    return currentScore;
}

qint32 IFailSoftAlphaBeta::failSoftBeta(qint32 searchDepth, qint32 alpha, qint32 beta)
{
    if (searchDepth == 0)
        return calScore(FACTION::red);

    qint32 currentScore(MINIMUM_VALUE);
    qint32 score(0);
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

            fakeMove(pChessStep);
            score = -failSoftAlpha(searchDepth - 1, -beta, -alpha);
            unFakeMove(pChessStep);

            delete pChessStep;
            pChessStep = Q_NULLPTR;

            if (score > alpha)
            {
                currentScore = score;

                if (score >= alpha) alpha = score;

                if (score >= beta)
                {
                    while (!chessStepList.isEmpty())
                    {
                        Step* pChessStep = chessStepList.back();
                        chessStepList.removeLast();
                        delete pChessStep;
                    }
                    return score;
                }
            }
        }
    }

    return currentScore;
}

void IFailSoftAlphaBeta::search()
{
	failSoftAlphaBeta(m_searchDepth, MINIMUM_VALUE, MAXIMUM_VALUE);
}
