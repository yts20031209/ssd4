#ifndef IMINWINDOWSEARCH_H
#define IMINWINDOWSEARCH_H

#include "isearchengine.h"

class IMiniWindowSearch : public ISearchEngine
{
	Q_OBJECT

public:
    IMiniWindowSearch(QObject *parent = Q_NULLPTR);
    virtual ~IMiniWindowSearch();

	qint32 miniWindowSearch(qint32 searchDepth, qint32 alpha, qint32 beta);
	qint32 miniWindowAlpha(qint32 searchDepth, qint32 alpha, qint32 beta);
	qint32 miniWindowBeta(qint32 searchDepth, qint32 alpha, qint32 beta);

protected:
	virtual void search() override;
};

#endif //IMINWINDOWSEARCH_H
