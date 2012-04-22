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

#include "SpatialAnalysisMap.h"

SpatialAnalysisMap::SpatialAnalysisMap(ElevationDataMap *elevationDataMap, QSettings *settings, QObject *parent) :
        AnalysisMap(elevationDataMap,settings,parent)
{
    // Init
    _name = "Spatial Analysis";
    _elevationDataMap = elevationDataMap;
}

double SpatialAnalysisMap::calculateScoreForPixel(int x, int y) {
    // Set tolerance of what is an acceptable height difference (in meters)
    double heightTolerance = 300;
    // Counter for acceptable points
    int goodPoints = 0;
    // Loop through surrounding pixels
    int range = 10;
    int x0,y0;
    for (x0 = -range; x0 < range+1; x0++) {
        for (y0 = -range; y0 < range+1; y0++) {
            // Find difference in elevation to current point
            // If difference < tolerance ==> it's a good point
            if ( qAbs(_elevationDataMap->getElevationAtPoint(x-x0,y-y0) - _elevationDataMap->getElevationAtPoint(x,y) ) < heightTolerance ) {
                goodPoints+= 1;
            }
        }
    }
    // The score (between 0..1)
    return goodPoints/qPow((2*range+1),2);
}


