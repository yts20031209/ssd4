#ifndef IMINIMAX_H
#define IMINIMAX_H

#include "isearchengine.h"

class IMiniMax : public ISearchEngine
{
	Q_OBJECT

public:
    IMiniMax(QObject *parent = Q_NULLPTR);
    virtual ~IMiniMax();

	qint32 miniMax(qint32 searchDepth);
	qint32 getMax(qint32 searchDepth, qint32 currentMax);
	qint32 getMin(qint32 searchDepth, qint32 currentMin);

protected:
	virtual void search() override;
};

#endif //IMINIMAX_H
