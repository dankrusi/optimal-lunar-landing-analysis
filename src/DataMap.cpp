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

#include "DataMap.h"

#include <QDebug>
#include <QCoreApplication>

#include "MapTile.h"

DataMap::DataMap(QSettings *settings, QObject *parent) :
    QObject(parent)
{
    // Init
    _settings = settings;
    _layer = new QGraphicsPixmapItem();
    _tileSize = 0;
    _tilesX = 0;
    _tilesY = 0;
}

DataMap::~DataMap() {

}

QGraphicsItem* DataMap::layer() {
    return _layer;
}

void DataMap::createTiles() {
    // Init
    int tileCount = 0;
    qDebug() << "Found" << _tilesX*_tilesY << "tiles @ " << _tileSize << "x" << _tileSize << "px (" << megaPixles() << "mega pixels )";

    // Loop tile grid
    for(int y = 0; y < _tilesY; y++) {
        for(int x = 0; x < _tilesX; x++) {
            // Init
            tileCount++;
            int xx = x*_tileSize;
            int yy = y*_tileSize;

            // Load data and create tile
            MapTile *tile = new MapTile(this,x,y,_tileSize,_layer);
            tile->setPos(xx,yy);
            _tiles.insert(QString("%1_%2").arg(x).arg(y),tile);
        }
    }
}

QRgb DataMap::getDataAtPixel(int x, int y) {
    MapTile *tile = getTileAtPixel(x,y);
    if(!tile) return 0;
    int xx = x-tile->pos().x();
    int yy = y-tile->pos().y();
    if(xx < 0 || xx > tile->image().width() || yy < 0 || yy > tile->image().height()) return 0;
    return tile->image().pixel(xx,yy);
}

MapTile* DataMap::getTileAtPixel(int x, int y) {
    int tileX = x/_tileSize;
    int tileY = y/_tileSize;
    return _tiles.value(QString("%1_%2").arg(tileX).arg(tileY));
}

double DataMap::megaPixles() {
    return ((_tilesX*_tilesY) * (_tileSize*_tileSize)) / 1000000.0;
}
