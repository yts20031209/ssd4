#ifndef INEGAMAX_H
#define INEGAMAX_H

#include "isearchengine.h"

class INegaMax : public ISearchEngine
{
	Q_OBJECT

public:
    INegaMax(QObject *parent = Q_NULLPTR);
    virtual ~INegaMax();

    qint32 negaMax(qint32 searchDepth, FACTION currentCamp);

protected:
	virtual void search() override;
};

#endif //INEGAMAX_H
