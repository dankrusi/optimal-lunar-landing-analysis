/*
* Optimal Lunar Landing Analysis
* https://github.com/dankrusi/optimal-lunar-landing-analysis
*
* Contributor(s)
* Christine Corbett <christine.corbett@gmail.com> (original author)
*
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

#include "SlopeAnalysisMapO2.h"

#include <QDebug>
#include <QtCore/qmath.h>

SlopeAnalysisMapO2::SlopeAnalysisMapO2(ElevationDataMap *elevationDataMap, QSettings *settings, QObject *parent) :
    AnalysisMap(elevationDataMap,settings,parent)
{
    // Init
    _name = "Slope Analysis 2nd order";
    _elevationDataMap = elevationDataMap;
}

double SlopeAnalysisMapO2::calculateScoreForPixel(int x, int y) {
    // Get slope in degrees
    return mapSlopeToScore(calculateSlopeForPoint(x,y));
};

double SlopeAnalysisMapO2::calculateSlopeForPoint(int x, int y) {
    // Calculate the height differences dHx & dHy for this point
    // TODO: handle coordinate change??
    double height_xy0 = _elevationDataMap->getElevationAtPoint(x,y);

    double height_y2 = _elevationDataMap->getElevationAtPoint(x,y+1);
    double height_y1 = _elevationDataMap->getElevationAtPoint(x,y-1);

    double height_x2 = _elevationDataMap->getElevationAtPoint(x+1,y);
    double height_x1 = _elevationDataMap->getElevationAtPoint(x-1,y);
    
    double dHy = height_y2 + height_y1 - 2.0*height_xy0;
    double dHx = height_x2 + height_x1- 2.0*height_xy0;

    // Slopes (note resX is dependent on y value)
    double gradientY = dHy/(2*_elevationDataMap->getYResolutionAtPixel());
    double gradientX = dHx/(2*_elevationDataMap->getXResolutionAtPixel(y));

    // Find the angle of the scalar of the gradient (radians 0 to 1/2PI)
    double gradient = qAtan( qSqrt(gradientX*gradientX + gradientY*gradientY));
    // Convert angle to degrees
    double slope = gradient*180.0/M_PI;

    return slope;

}

double SlopeAnalysisMapO2::mapSlopeToScore(double slope) {
    // Linear analysis
    return qPow((-1.0/90.0*slope+1.0),6.0);
    //return (-100/90*slope+100)/100;
    //return qPow((-1/90*slope+1),16);
}
