#ifndef IFAILSOFTALPHABETA_H
#define IFAILSOFTALPHABETA_H

#include "isearchengine.h"

class IFailSoftAlphaBeta : public ISearchEngine
{
	Q_OBJECT

public:
    IFailSoftAlphaBeta(QObject *parent = Q_NULLPTR);
    virtual ~IFailSoftAlphaBeta();

	qint32 failSoftAlphaBeta(qint32 searchDepth, qint32 alpha, qint32 beta);
	qint32 failSoftAlpha(qint32 searchDepth, qint32 alpha, qint32 beta);
	qint32 failSoftBeta(qint32 searchDepth, qint32 alpha, qint32 beta);

protected:
    virtual void search() override;
};

#endif //IFAILSOFTALPHABETA_H
