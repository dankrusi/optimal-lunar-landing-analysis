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

#include "AnalysisMap.h"

#include <QPainter>
#include <QRunnable>
#include <QCoreApplication>
#include <QDebug>

AnalysisMap::AnalysisMap(DataMap *dataMap, QSettings *settings, QObject *parent) :
    DataMap(settings,parent)
{
    // Init
    _dataMap = dataMap;
}

void AnalysisMap::load() {

    // Set tile settings
    _tileSize = _dataMap->tileSize();
    _tilesX = _dataMap->tilesX();
    _tilesY = _dataMap->tilesY();

    // Create tile objects
    createTiles();
}

void AnalysisMap::loadTileImage(int tileX, int tileY, QImage &image) {

    // New code: optimized for speed and direct memory access

    // Create image
    image = QImage(this->tileSize(),this->tileSize(),QImage::Format_ARGB32);

    // Scan image lines...
    for (int y = 0; y < image.height(); ++y) {
      QRgb *row = (QRgb*)image.scanLine(y);
      for (int x = 0; x < image.width(); ++x) {

	  // Grab score
	  int xx = tileX*_dataMap->tileSize() + x;
	  int yy = tileY*_dataMap->tileSize() + y;
	  double score = calculateScoreForPoint(xx,yy);
	  if(score < 0) score = 0;
	  if(score > 1) score = 1;

      // Alter pixels of image directly   for (int x = 0; x < image.width(); ++x) {
      unsigned char scoreByte = 255 - score*255;
      ((unsigned char*)&row[x])[0] = 0;             // B
      ((unsigned char*)&row[x])[1] = 0;             // G
      ((unsigned char*)&row[x])[2] = scoreByte;     // R
      ((unsigned char*)&row[x])[3] = 255;           // A
      }
    }


    // Old code:

    /*
    // Create image
    image = QImage(this->tileSize(),this->tileSize(),QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    // Paint it according to score
    QPainter painter;
    painter.begin(&image);

    for(int x = 0; x < image.width(); x++) {
	for(int y = 0; y < image.width(); y++) {
	    int xx = tileX*_dataMap->tileSize() + x;
	    int yy = tileY*_dataMap->tileSize() + y;
	    double score = calculateScoreForPoint(xx,yy);
	    if(score < 0) score = 0;
	    if(score > 1) score = 1;
	    int scoreByte = score * 255;
	    scoreByte = 0;
	    painter.setPen(qRgba(200,200,200,50));
	    //painter.setPen(Qt::red);
	    painter.drawRect(x,y,1,1);
	}
    }

    painter.end();
    */
}
