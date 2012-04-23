/*
* Optimal Lunar Landing Analysis
* https://github.com/dankrusi/optimal-lunar-landing-analysis
*
* Contributor(s):
* Dan Krusi <dan.krusi@nerves.ch> (original author)
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

#ifndef ANALYSISMAP_H
#define ANALYSISMAP_H

#include "DataMap.h"

class AnalysisMap : public DataMap
{

Q_OBJECT

private:
    DataMap *_dataMap;
    int _resolution;

public:
    explicit AnalysisMap(DataMap *dataMap, QSettings *settings, QObject *parent = 0);
    virtual void load();
    virtual void loadTileImage(int tileX, int tileY, QImage &image);
    virtual double calculateScoreForPixel(int x, int y) = 0; // Implementing classes should return a value between 0.0 to 1.0 where 1.0 is the highest score possible

    inline double getScoreForPixel(int x, int y) {
        QRgb p = getDataAtPixel(x,y);
        return qRed(p)/255.0;
    }

signals:

public slots:

};

#endif // ANALYSISMAP_H
