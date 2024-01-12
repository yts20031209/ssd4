#ifndef IALPHABETA_H
#define IALPHABETA_H

#include "isearchengine.h"

class IAlphaBeta : public ISearchEngine
{
    Q_OBJECT

public:
    IAlphaBeta(QObject *parent = Q_NULLPTR);
    virtual ~IAlphaBeta();

    qint32 alphaBeta(qint32 searchDepth, qint32 alpha, qint32 beta);//0层
    qint32 alpha(qint32 searchDepth, qint32 alpha, qint32 beta);//非0偶数层
    qint32 beta(qint32 searchDepth, qint32 alpha, qint32 beta);//奇数层

protected:
    virtual void search() override;
};

#endif //IALPHABETA_H
