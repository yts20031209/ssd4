#include "isearchengine.h"

//#include "iglobal.h"

#include <QtCore/qdebug.h>
#include <QtCore/qdatetime.h>

#include <QtCore/qthread.h>

ISearchEngine::ISearchEngine(QObject *parent)
    : QObject(parent)
    , m_searchDepth(DEFAULT_SEARCH_DEPTH)
    , m_camp(FACTION::black)
    , m_pBestStep(Q_NULLPTR)
    , m_pCheckGeneralStep(Q_NULLPTR)
    , m_stepCount(0)
    , m_firstStep(true)
{
}

ISearchEngine::~ISearchEngine()
{
}

void ISearchEngine::setSearchDepth(const qint32& searchDepth)
{
    m_searchDepth = searchDepth;
}

qint32 ISearchEngine::searchDepth() const
{
    return m_searchDepth;
}

void ISearchEngine::setCamp(FACTION camp)
{
    m_camp = camp;
}

FACTION ISearchEngine::camp() const
{
    return m_camp;
}

void ISearchEngine::setBestStep(Step* pBestStep)
{
    m_pBestStep = pBestStep;
}

Step* ISearchEngine::bestStep()
{
    return m_pBestStep;
}
//*
void ISearchEngine::setOperatePieceList(const QList<Stone*>& pieceList)
{
    m_operatePieceList = pieceList;
}
//*
QList<Stone*>& ISearchEngine::operatePieceList()
{
    return m_operatePieceList;
}

void ISearchEngine::setStepCount(const quint64& count)
{
    m_stepCount = count;
}

quint64 ISearchEngine::stepCount() const
{
    return m_stepCount;
}

void ISearchEngine::setFirstStep(bool firstStep)
{
    m_firstStep = firstStep;
}

bool ISearchEngine::firstStep() const
{
    return m_firstStep;
}
//游戏结束
bool ISearchEngine::fightOver()
{
    Stone* pRedGeneral = board::Board().findStone(TYPE::SHUAI, FACTION::red);
    if (pRedGeneral == nullptr)
        return true;

    Stone* pBlackGeneral = board::Board().findStone(TYPE::SHUAI, FACTION::black);
    if (pBlackGeneral == nullptr)
        return true;

    return false;
    //return board::gameOver;
}

//*
Step* ISearchEngine::bestStep(const QList<Stone*>& pieceList)
{
    m_stepCount = 0;
    m_operatePieceList = pieceList;
    m_pBestStep = Q_NULLPTR;
    m_pCheckGeneralStep = Q_NULLPTR;
    QTime startTime = QTime::currentTime();
    if (!checkEnemyGeneral())
    {
        search();//如果敌方已被将军，便不再判断，直接击杀
    }
    QTime endTime = QTime::currentTime();
    qDebug() << startTime.msecsTo(endTime) << "ms, " << m_stepCount << "步";
    return m_pBestStep;
}

qint32 ISearchEngine::calScore(FACTION camp)
{
    //车 马 相 士 将 炮 兵
    //车9分、炮4分半、马4分、过河兵2分、未过河兵1分、相2分、仕2分。
    /*enum class IPieceType {
    Empty = 0,  //未知
    General,       //将/帅
    Guard,      //士
    Elephant,   //象
    Horse,      //马
    Castle,        //车
    Cannon,     //炮
    Soldier     //兵/卒
};*/
    qint32 pieceScore[7] = { 2000, 10, 10, 40, 90, 50, 10 };
    //qint32 chessScore[7] = { 90, 40, 10, 10, 2000, 50, 10 };//2520
    qint32 blackScore = 0;
    qint32 redScore = 0;
    for (Stone* pPiece : m_operatePieceList)
    {
        if (pPiece->isDead)
        {
            if (pPiece->getStoneType() == TYPE::SHUAI)
            {
                if (pPiece->getFac() == FACTION::black)
                {
                    redScore = MAXIMUM_VALUE;
                    blackScore = MINIMUM_VALUE;
                }
                else
                {
                    redScore = MINIMUM_VALUE;
                    blackScore = MAXIMUM_VALUE;
                }
                break;
            }
            else
                continue;
        }

        /*QList<PieceStep*> chessStepList = pChess->allPossibleSteps(m_operatePieceList);
        for (PieceStep* pChessStep : chessStepList)
        {
            Piece* pKillChess = pChessStep->killChess();
            if (pKillChess != Q_NULLPTR)
            {
                if (pKillChess->camp() == PieceCamp::Black)
                    blackScore += -10;
                else
                    redScore += -10;
            }
            delete pChessStep;
        }*/

        if (pPiece->getFac() == FACTION::black)
        {
            blackScore += pieceScore[qint32(pPiece->getStoneType())];
        }// +chessStepList.count() * 2;

        else
        {
            redScore += pieceScore[qint32(pPiece->getStoneType())];
        }// +chessStepList.count() * 2;
    }

    qint32 situationScore = (camp == FACTION::black) ? blackScore - redScore : redScore - blackScore;
    return situationScore;
}
//*
void ISearchEngine::fakeMove(Step* pStep)
{
    pStep->executeMove();
}
//*
void ISearchEngine::unFakeMove(Step* pStep)
{
    pStep->revokeMove();
}

FACTION ISearchEngine::nextCamp(const FACTION& currentCamp)
{
    FACTION nextCamp = FACTION::black;
    if (currentCamp == FACTION::black)
    {
        nextCamp = FACTION::red;
    }

    return nextCamp;
}

bool ISearchEngine::checkEnemyGeneral()
{
    //获取敌方将军（元帅）的位置
    FACTION enemyCamp = this->nextCamp(m_camp);
    Stone* pEnemyGeneral = board::Board().findStone(TYPE::SHUAI, enemyCamp);
    QPoint enemyGeneralPos = pEnemyGeneral->getPos();

    //获取我方所有棋子的初始位置和结束位置（是相连着的）
    quint8 beginIndex = board::Board().campBeginIndex(m_camp);
    quint8 endIndex = board::Board().campEndIndex(m_camp);

    //检查
    return checkGeneral(enemyGeneralPos, beginIndex, endIndex, true);
}

bool ISearchEngine::checkSelfGeneral()
{
    //获取己方将军（元帅）的位置
    FACTION enemyCamp = this->nextCamp(m_camp);
    Stone* pSelfGeneral = board::Board().findStone(TYPE::SHUAI, m_camp);
    QPoint selfGeneralPos = pSelfGeneral->getPos();

    //获取敌方所有棋子的初始位置和结束位置（是相连着的）
    quint8 beginIndex = board::Board().campBeginIndex(enemyCamp);
    quint8 endIndex = board::Board().campEndIndex(enemyCamp);

    //检查
    return checkGeneral(selfGeneralPos, beginIndex, endIndex);
}

bool ISearchEngine::checkCampGeneral(const FACTION &camp)
{
    //获取将军（元帅）的位置
    Stone* pGeneral = board::Board().findStone(TYPE::SHUAI, camp);
    QPoint generalPos = pGeneral->getPos();

    //获取所有棋子的初始位置和结束位置（是相连着的）
    quint8 beginIndex = board::Board().campBeginIndex(camp);
    quint8 endIndex = board::Board().campEndIndex(camp);

    //检查
    return checkGeneral(generalPos, beginIndex, endIndex);
}
//*
bool ISearchEngine::checkGeneral(const QPoint& enemyGeneralPos, quint8 beginIndex, quint8 endIndex, bool need)
{
    Step* pStep = Q_NULLPTR;
    for (quint8 index(beginIndex); index <= endIndex; index++)
    {
        if(m_operatePieceList[index]->isDead) continue;

        //如果我方能操作到的棋子能吃掉对面的帅，则直接吃掉
        pStep = m_operatePieceList[index]->canMoveTo(enemyGeneralPos);
        if(pStep != Q_NULLPTR)
        {
            if (need)
                m_pBestStep = pStep;
            else
                delete pStep;

            return true;
        }
    }

    return false;
}
//*
Step* ISearchEngine::escapeCheck(const QList<Stone*>& pieceList)
{
    for (qint8 index(14); index >= 0; index--)
    {
        if (pieceList[index]->isDead) continue;

        QList<Step*> stepList = pieceList[index]->allPossibleSteps();
        while (!stepList.isEmpty())
        {
            Step* pStep = stepList.back();
            stepList.removeLast();
            fakeMove(pStep);
            if (!checkSelfGeneral())
            {
                unFakeMove(pStep);
                while (!stepList.isEmpty())
                {
                    Step* pPieceStep2 = stepList.back();
                    stepList.removeLast();
                    delete pPieceStep2;
                    pPieceStep2 = Q_NULLPTR;
                }
                return pStep;
            }
            unFakeMove(pStep);
            delete pStep;
            pStep = Q_NULLPTR;
        }
    }

    QList<Step*> stepList = pieceList[15]->allPossibleSteps();
    while (!stepList.isEmpty())
    {
        Step* pPieceStep = stepList.back();
        stepList.removeLast();
        fakeMove(pPieceStep);
        if (!checkSelfGeneral())
        {
            unFakeMove(pPieceStep);
            while (!stepList.isEmpty())
            {
                Step* pPieceStep2 = stepList.back();
                stepList.removeLast();
                delete pPieceStep2;
                pPieceStep2 = Q_NULLPTR;
            }
            return pPieceStep;
        }
        unFakeMove(pPieceStep);
        delete pPieceStep;
        pPieceStep = Q_NULLPTR;
    }

    return Q_NULLPTR;
}
void ISearchEngine::getFirstStep()
{
    m_firstStep = false;
    if(board::Board().findStone(TYPE::PAO, FACTION::red)->getPos()== QPoint(4, 7)
            || board::Board().findStone(TYPE::PAO, FACTION::red)->getPos() == QPoint(4, 7))
    {
        Stone* pPiece = board::Board().findStone(TYPE::MA, FACTION::black);
        m_pBestStep = pPiece->canMoveTo(QPoint(2, 2));
        return;
    }

    quint8 index = 16;
    do {
        qsrand(QTime(0, 0, 0).msecsTo(QTime::currentTime()));
        index = qrand() % 16;
        if(index < 2 || index == 6 || index == 8 || index == 10 || index == 11)
            index = 16;
    } while(index == 16);

    Stone* pPiece = m_operatePieceList[index];
    if (pPiece->getStoneType() == TYPE::MA)
    {
        //左边的马
        if (pPiece->getPos().x() == 1)
        {
            m_pBestStep = pPiece->canMoveTo(QPoint(2, 2));
            return;
        }

        //右边的马
        m_pBestStep = pPiece->canMoveTo(QPoint(6, 2));
        return;
    }

    //炮
    if (pPiece->getStoneType() == TYPE::PAO)
    {
        m_pBestStep = pPiece->canMoveTo(QPoint(4, 2));
        return;
    }

    //象
    if (pPiece->getStoneType() == TYPE::XIANG)
    {
        m_pBestStep = pPiece->canMoveTo(QPoint(4, 2));
        return;
    }

    //士
    if (pPiece->getStoneType() == TYPE::SHI)
    {
        m_pBestStep = pPiece->canMoveTo(QPoint(4, 1));
        return;
    }

    //兵
    if (pPiece->getStoneType() == TYPE::BING)
    {
        m_pBestStep = pPiece->canMoveTo(QPoint(pPiece->getPos().x(), pPiece->getPos().y() + 1));
        return;
    }
}

void ISearchEngine::startSearch()
{
    QThread::msleep(1000);//先停1秒
    bestStep(board::Board().getStoneList());
    emit searchEnd();
}

