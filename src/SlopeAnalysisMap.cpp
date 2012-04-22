/*
* Optimal Lunar Landing Analysis
* https://github.com/dankrusi/optimal-lunar-landing-analysis
*
* Contributor(s):
* Dan Krusi <dan.krusi@nerves.ch> (original author)
* Stephan Krusi <stephan.krusi@gmail> (co-author)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
* of the Software, and to permit persons to whom the Software is furnished to do
* so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include "SlopeAnalysisMap.h"

#include <QDebug>
#include <QtCore/qmath.h>

SlopeAnalysisMap::SlopeAnalysisMap(ElevationAnalysisMap *elevationAnalysisMap, QSettings *settings, QObject *parent) :
    AnalysisMap(elevationAnalysisMap,settings,parent)
{
    // Init
    _name = "Slope Analysis";
    _elevationAnalysisMap = elevationAnalysisMap;
}

double SlopeAnalysisMap::calculateScoreForPoint(int x, int y) {
    // Get slope in degrees
    return mapSlopeToScore(calculateSlopeForPoint(x,y));
};

double SlopeAnalysisMap::calculateSlopeForPoint(int x, int y) {
    // Calculate the dx & dy for this point
    // TODO: handle coordinate change??
    double y2 = _elevationAnalysisMap->getScoreForPoint(x,y+1);
    double y1 = _elevationAnalysisMap->getScoreForPoint(x,y-1);
    double x2 = _elevationAnalysisMap->getScoreForPoint(x+1,y);
    double x1 = _elevationAnalysisMap->getScoreForPoint(x,y);
    
    double dy = y2 - y1;
    double dx = x2 - x1;

    // Find the angle of the scalar of the gradient (radians 0 to 1/2PI)
    double gradient = qAtan( qSqrt(dx*dx + dy*dy) );
    // Convert angle to degrees
    double slope = gradient*180.0/M_PI;
    return slope;
}

double SlopeAnalysisMap::mapSlopeToScore(double slope) {
    // Linear analysis
    return qPow((-1.0/90.0*slope+1.0),6.0);
    //return (-100/90*slope+100)/100;
    //return qPow((-1/90*slope+1),16);
}
