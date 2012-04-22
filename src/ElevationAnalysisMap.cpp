#include "ElevationAnalysisMap.h"

ElevationAnalysisMap::ElevationAnalysisMap(ElevationDataMap *elevationMap, QSettings *settings, QObject *parent) :
    AnalysisMap(elevationMap,settings,parent)
{
    // Init
    _name = "Elevation Analysis";
    _elevationMap = elevationMap;
}

double ElevationAnalysisMap::calculateScoreForPixel(int x, int y) {
    // Really trivial implementation: the higher the better...
    int elevation = _elevationMap->getElevationAtPoint(x,y);
    int minElevation = -9150;
    int maxElevation = 10760;
    return (double)(elevation - minElevation) / (double)(maxElevation - minElevation);
};

