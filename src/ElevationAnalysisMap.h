#ifndef ELEVATIONANALYSISMAP_H
#define ELEVATIONANALYSISMAP_H

#include "AnalysisMap.h"
#include "ElevationDataMap.h"

class ElevationAnalysisMap : public AnalysisMap
{

Q_OBJECT

protected:
    ElevationDataMap *_elevationMap;

public:
    explicit ElevationAnalysisMap(ElevationDataMap *elevationMap, QSettings *settings, QObject *parent = 0);
    virtual double calculateScoreForPixel(int x, int y);

signals:

public slots:

};

#endif // ELEVATIONANALYSISMAP_H
