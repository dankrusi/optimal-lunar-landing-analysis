#include "ElevationAnalysisMap.h"

ElevationAnalysisMap::ElevationAnalysisMap(ElevationDataMap *elevationMap, QSettings *settings, QObject *parent) :
    AnalysisMap(elevationMap,settings,parent)
{
    // Init
    _name = "Elevation Analysis";
    _elevationMap = elevationMap;
}

double ElevationAnalysisMap::calculateScoreForPoint(int x, int y) {

    int elevation = _elevationMap->getElevationAtPoint(x,y);

    int minElevation = -9150;
    int maxElevation = 10760;

    return (double)(elevation - minElevation) / (double)(maxElevation - minElevation);

};

