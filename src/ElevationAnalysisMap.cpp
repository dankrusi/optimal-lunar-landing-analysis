/*
* Optimal Lunar Landing Analysis
* https://github.com/dankrusi/optimal-lunar-landing-analysis
*
* Contributor(s):
* Dan Krusi <dan.krusi@nerves.ch> (original author)
* Stephan Krusi <stephan.krusi@gmail> (co-author)
* Nikolay Khanenya <Nick3.Khanenya@gmail.com>
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

#include "ElevationAnalysisMap.h"

ElevationAnalysisMap::ElevationAnalysisMap(ElevationDataMap *elevationMap, QSettings *settings, QObject *parent) :
    AnalysisMap(elevationMap,settings,parent)
{
    // Init
    _name = "Elevation Analysis";
    _elevationMap = elevationMap;
}

double ElevationAnalysisMap::calculateScoreForPixel(int x, int y) {

    int elevation = _elevationMap->getElevationAtPoint(x,y);

    int minElevation = -9150;
    int maxElevation = 10760;

    return (double)(elevation - minElevation) / (double)(maxElevation - minElevation);

};

