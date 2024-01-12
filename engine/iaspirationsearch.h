#ifndef IASPIRATIONSEARCH_H
#define IASPIRATIONSEARCH_H

#include "ifailsoftalphabeta.h"

class IAspirationSearch : public IFailSoftAlphaBeta
{
	Q_OBJECT

public:
    IAspirationSearch(QObject *parent = Q_NULLPTR);
    virtual ~IAspirationSearch();

protected:
	virtual void search() override;
};

#endif //IASPIRATIONSEARCH_H
