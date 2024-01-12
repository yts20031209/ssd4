#include "ialphabeta.h"

#include <QtCore/qdebug.h>

IAlphaBeta::IAlphaBeta(QObject *parent)
    : ISearchEngine(parent)
{
}

IAlphaBeta::~IAlphaBeta()
{
}

qint32 IAlphaBeta::alphaBeta(qint32 searchDepth, qint32 alpha, qint32 beta)
{
    Step* pStep = Q_NULLPTR;
    qint32 score(0);
    quint8 pieceStepCount(0);
    QList<Step*> stepList;
    Stone* pPiece = Q_NULLPTR;
    for (quint8 index(0);index < 16;index++)
    {
        pPiece = m_operatePieceList[index];
        if (pPiece->isDead) continue;

        stepList = pPiece->allPossibleSteps();

        pieceStepCount = stepList.count();
        while (pieceStepCount--)
        {
            m_stepCount++;
            pStep = stepList[pieceStepCount];
            pStep->executeMove();//fakeMove(pStep);
//            if (checkSelfGeneral())
//            {
//                pStep->revoke();//unFakeMove(pStep);
//                delete pStep;
//                continue;
//            }
            score = -this->beta(searchDepth - 1, -beta, -alpha);
            pStep->revokeMove();//unFakeMove(pStep);

            if (score > alpha)
            {
                alpha = score;
                if (m_pBestStep != Q_NULLPTR) delete m_pBestStep;
                m_pBestStep = pStep;
            }
            else
            {
                delete pStep;
                pStep = Q_NULLPTR;
            }

            if (score >= beta)
            {
                while (pieceStepCount--)
                {
                    delete stepList[pieceStepCount];
                }
                stepList.clear();
                return score;
            }

        }
        stepList.clear();
    }

    return alpha;
}

qint32 IAlphaBeta::alpha(qint32 searchDepth, qint32 alpha, qint32 beta)
{
    if (searchDepth == 0 || fightOver())
        return calScore(m_camp);

    Step* pStep = Q_NULLPTR;
    Stone* pPiece = Q_NULLPTR;
    qint32 score(0);
    for(quint8 index(0);index < 16;index++)
    {
        pPiece = m_operatePieceList[index];
        if (pPiece->isDead) continue;

        QList<Step*> chessStepList = pPiece->allPossibleSteps();
        while (!chessStepList.isEmpty())
        {
            pStep = chessStepList.back();
            chessStepList.removeLast();

            fakeMove(pStep);
//            if (checkSelfGeneral())
//            {
//                unFakeMove(pStep);
//                delete pStep;
//                pStep = Q_NULLPTR;
//                continue;
//            }
            m_stepCount++;
            score = - (this->beta(searchDepth - 1, -beta, -alpha));
            pStep->revokeMove();//unFakeMove(pStep);

            delete pStep;
            pStep = Q_NULLPTR;

            if (score > alpha) alpha = score;

            if (score >= beta)
            {
                while (!chessStepList.isEmpty())
                {
                    pStep = chessStepList.back();
                    chessStepList.removeLast();
                    delete pStep;
                }
                return score;
            }
        }
    }

    return alpha;
}

qint32 IAlphaBeta::beta(qint32 searchDepth, qint32 alpha, qint32 beta)
{
    if (searchDepth == 0 || fightOver())
        return calScore(FACTION::red);

    Step* pStep = Q_NULLPTR;
    Stone* pPiece = Q_NULLPTR;
    qint32 score(0);
    for(quint8 index(16);index < 32;index++)
    {
        pPiece = m_operatePieceList[index];
        if (pPiece->isDead) continue;

        QList<Step*> chessStepList = pPiece->allPossibleSteps();
        while (!chessStepList.isEmpty())
        {
            pStep = chessStepList.back();
            chessStepList.removeLast();

            pStep->executeMove();//fakeMove(pStep);
//            if (checkCampGeneral(Camp::Red))
//            {
//                unFakeMove(pStep);
//                delete pStep;
//                pStep = Q_NULLPTR;
//                continue;
//            }
            m_stepCount++;
            score = -(this->alpha(searchDepth - 1, -beta, -alpha));
            pStep->revokeMove();//unFakeMove(pStep);

            delete pStep;
            pStep = Q_NULLPTR;

            if (score > alpha) alpha = score;

            if (score >= beta)
            {
                while (!chessStepList.isEmpty())
                {
                    pStep = chessStepList.back();
                    chessStepList.removeLast();
                    delete pStep;
                }
                return score;
            }
        }
    }

    return alpha;
}

void IAlphaBeta::search()
{
    alphaBeta(m_searchDepth, MINIMUM_VALUE, MAXIMUM_VALUE);
}
