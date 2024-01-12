#include "iminimax.h"

IMiniMax::IMiniMax(QObject *parent)
    : ISearchEngine(parent)
{
#if 0
	if (searchDepth == 0)
		return calScore(m_chessCamp);

	qint32 currentScore = (currentChessCamp == KChessCamp::Black) ? MINIMUM_VALUE : MAXIMUM_VALUE;
    for (KChess* pChess : m_operatePieceList)
	{
		if (pChess->dead() || (pChess->camp() != currentChessCamp))
			continue;

        QList<PieceStep*> chessStepList = pChess->allPossibleSteps(m_operatePieceList);
		while (!chessStepList.isEmpty())
		{
			m_stepCount++;
            PieceStep* pChessStep = chessStepList.back();
			chessStepList.removeLast();

			fakeMove(pChessStep);
			qint32 score = miniMax(searchDepth - 1, nextChessCamp(currentChessCamp), currentScore);
			unFakeMove(pChessStep);

			if (searchDepth != m_searchDepth)
			{
				if ((currentChessCamp == KChessCamp::Black) ? (score >= currentBestScore) : (score <= currentBestScore))
				{
					while (!chessStepList.isEmpty())
					{
                        PieceStep* pChessStep = chessStepList.back();
						chessStepList.removeLast();
						delete pChessStep;
					}
					return score;
				}
			}

			bool condition = (currentChessCamp == KChessCamp::Black) ? (score > currentScore) : (score < currentScore);
			if (condition) currentScore = score;

			if (condition && (m_searchDepth == searchDepth))
			{
                if (m_pBestPieceStep) delete m_pBestPieceStep;
                m_pBestPieceStep = pChessStep;
			}
			else
			{
				delete pChessStep;
			}
		}
	}

	return currentScore;
#endif
}

IMiniMax::~IMiniMax()
{
}

qint32 IMiniMax::miniMax(qint32 searchDepth)
{
	qint32 score(0);
	quint8 len(0);
	qint32 currentScore(MINIMUM_VALUE);
    for (quint8 index(0); index < 16; index++)
	{
        if (m_operatePieceList[index]->isDead) continue;

        QList<Step*> chessStepList = m_operatePieceList[index]->allPossibleSteps();
		len = chessStepList.count();
		while (len--)
		{
			m_stepCount++;
            Step* pChessStep = chessStepList[len];

			fakeMove(pChessStep);
			score = getMin(searchDepth - 1, currentScore);
			unFakeMove(pChessStep);

			if (score > currentScore)
			{
				currentScore = score;
                if (m_pBestStep)
				{
                    delete m_pBestStep;
                    m_pBestStep = Q_NULLPTR;
				}
                m_pBestStep = pChessStep;
			}
			else
			{
				delete pChessStep;
				pChessStep = Q_NULLPTR;
			}
		}
		chessStepList.clear();
	}

	return currentScore;
}

qint32 IMiniMax::getMax(qint32 searchDepth, qint32 currentMax)
{
	if (searchDepth == 0 || fightOver())
        return calScore(m_camp);

	qint32 score(0);
	quint8 len(0);
	qint32 currentScore(MINIMUM_VALUE);
    for (quint8 index(0);index < 16;index++)
	{
        if (m_operatePieceList[index]->isDead) continue;

        QList<Step*> chessStepList = m_operatePieceList[index]->allPossibleSteps();
		len = chessStepList.count();
		while (len--)
		{
			m_stepCount++;
            Step* pChessStep = chessStepList[len];

			fakeMove(pChessStep);
			score = getMin(searchDepth - 1, currentScore);
			unFakeMove(pChessStep);

			delete pChessStep;
			pChessStep = Q_NULLPTR;

			if (score >= currentMax)
			{
				while (len--)
				{
					delete chessStepList[len];
					chessStepList[len] = Q_NULLPTR;
				}
				chessStepList.clear();
				return score;
			}
			
			if (score > currentScore) currentScore = score;
		}
		chessStepList.clear();
	}

	return currentScore;
}

qint32 IMiniMax::getMin(qint32 searchDepth, qint32 currentMin)
{
	if (searchDepth == 0 || fightOver())
        return calScore(m_camp);

	quint8 len(0);
	qint32 score(0);
	qint32 currentScore(MAXIMUM_VALUE);
    for (quint8 index(16);index < 32;index++)
	{
        if (m_operatePieceList[index]->isDead) continue;

        QList<Step*> chessStepList = m_operatePieceList[index]->allPossibleSteps();
        len = chessStepList.count();
		while (len--)
		{
			m_stepCount++;
            Step* pChessStep = chessStepList[len];

			fakeMove(pChessStep);
			score = getMax(searchDepth - 1, currentScore);
			unFakeMove(pChessStep);

			delete pChessStep;
			pChessStep = Q_NULLPTR;

			if (score <= currentMin)
			{
				while (len--)
				{
					delete chessStepList[len];
					chessStepList[len] = Q_NULLPTR;
				}
				chessStepList.clear();
				return score;
			}

			if (score < currentScore) currentScore = score;
		}
		chessStepList.clear();
	}

	return currentScore;
}

void IMiniMax::search()
{
	miniMax(m_searchDepth);
}
