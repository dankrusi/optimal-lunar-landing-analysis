/*
* Optimal Lunar Landing Analysis
* https://github.com/dankrusi/optimal-lunar-landing-analysis
*
* Contributor(s):
* Dan Krusi <dan.krusi@nerves.ch> (original author)
* Stephan Krusi <stephan.krusi@gmail> (original co-author)
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

#include "MapTile.h"

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
    _resolution = 1; // Must be divisor of tile size

    // Create tile objects
    createTiles();
}

void AnalysisMap::loadTileImage(int tileX, int tileY, QImage &image) {

    // New code: optimized for speed and direct memory access

    // Create image
    image = QImage(this->tileSize(),this->tileSize(),QImage::Format_ARGB32);

    // Init
    double score = 0;
    int tileSize = _dataMap->tileSize();

    // Switch on resolution, since the systems are quite differerent
    if(_resolution == 1) {

	// Scan image lines...
	for (int y = 0; y < image.height(); y++) {
	    QRgb *row = (QRgb*)image.scanLine(y);
	    for (int x = 0; x < image.width(); x++) {

		// Grab score
		int xx = tileX*tileSize + x;
		int yy = tileY*tileSize + y;
		score = calculateScoreForPixel(xx,yy);
		if(score < 0) score = 0;
		else if(score > 1) score = 1;

		// Alter pixels of image directly
		unsigned char scoreByte = score*255;
		((unsigned char*)&row[x])[0] = 0;	    // B
		((unsigned char*)&row[x])[1] = 0;	    // G
		((unsigned char*)&row[x])[2] = scoreByte;   // R
		((unsigned char*)&row[x])[3] = 255;         // A
	    }
	}

    } else {

	// Loop over blocks of _resolution
	for (int resTileX = 0; resTileX < image.width()/_resolution; resTileX++) {
	    for (int resTileY = 0; resTileY < image.height()/_resolution; resTileY++) {
		int resX = resTileX*_resolution;
		int resY = resTileY*_resolution;
		int xx = tileX*tileSize + resX;
		int yy = tileY*tileSize + resY;
		score = calculateScoreForPixel(xx,yy);
		if(score < 0) score = 0;
		else if(score > 1) score = 1;

		// Scan image lines...
		for (int y = resY; y < resY+_resolution; y++) {
		    QRgb *row = (QRgb*)image.scanLine(y);
		    for (int x = resX; x < resX+_resolution; x++) {
			// Alter pixels of image directly
			unsigned char scoreByte = score*255;
			((unsigned char*)&row[x])[0] = 0;	    // B
			((unsigned char*)&row[x])[1] = 0;	    // G
			((unsigned char*)&row[x])[2] = scoreByte;   // R
			((unsigned char*)&row[x])[3] = 255;         // A
		    }
		}
	    }
	}
    }
}

void AnalysisMap::setResolution(int resolution) {
    qDebug() << "Changing resolution to" << resolution;

    // Set
    _resolution = resolution; //TODO: make sure it is valid?

    // Tell all tiles
    foreach(MapTile *tile,_tiles) {
	tile->dropData();
    }
}

