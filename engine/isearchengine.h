#ifndef ISEARCHENGINE_H
#define ISEARCHENGINE_H

#include "board.h"
#include "stone.h"
//#include "piece/ipiece.h"

constexpr qint32 MAXIMUM_VALUE = 2520;
constexpr qint32 MINIMUM_VALUE = -2520;
constexpr qint32 DEFAULT_SEARCH_DEPTH = 4;

class ISearchEngine : public QObject
{
    Q_OBJECT
public:
    ISearchEngine(QObject *parent = Q_NULLPTR);
    virtual ~ISearchEngine();

    void setSearchDepth(const qint32& searchDepth);
    qint32 searchDepth() const;

    void setCamp(FACTION camp);
    FACTION camp() const;

    void setBestStep(Step* pBestStep);
    Step* bestStep();

    void setOperatePieceList(const QList<Stone*>& operatePieceieceList);
    QList<Stone*>& operatePieceList();

    void setStepCount(const quint64& count);
    quint64 stepCount() const;

    void setFirstStep(bool firstStep);
    bool firstStep() const;

    bool fightOver();
    Step* bestStep(const QList<Stone*>& pieceList);
    qint32 calScore(FACTION camp);

    void fakeMove(Step* pStep);
    void unFakeMove(Step* pStep);

    FACTION nextCamp(const FACTION& currentCamp);

    bool checkEnemyGeneral();
    bool checkSelfGeneral();
    bool checkCampGeneral(const FACTION& camp);
    bool checkGeneral(const QPoint& enemyGeneralPos, quint8 beginIndex, quint8 endIndex, bool need = false);
    Step* escapeCheck(const QList<Stone*>& pieceList);
    void getFirstStep();

protected:
    virtual void search() = 0;

signals:
    void searchEnd();

public slots:
    void startSearch();

protected:
    qint32 m_searchDepth;
    FACTION m_camp;
    Step* m_pBestStep;
    Step* m_pCheckGeneralStep;
    QList<Stone*> m_operatePieceList;
    quint64 m_stepCount;
    bool m_firstStep;
    board *b;
};

#endif //ISEARCHENGINE_H
