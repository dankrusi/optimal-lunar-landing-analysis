#ifndef EXPONENTIALSLOPEANALYSISMAP_H
#define EXPONENTIALSLOPEANALYSISMAP_H


#include "SlopeAnalysisMap.h"

class ExponentialSlopeAnalysisMap : public SlopeAnalysisMap
{
public:
    explicit ExponentialSlopeAnalysisMap(ElevationDataMap *elevationMap, QSettings *settings, QObject *parent = 0);
    virtual double mapSlopeToScore(double slope);
};

#endif // EXPONENTIALSLOPEANALYSISMAP_H
