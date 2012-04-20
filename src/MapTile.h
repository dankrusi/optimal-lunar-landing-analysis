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

#ifndef MAPTILE_H
#define MAPTILE_H

#include <QGraphicsItem>
#include <QPixmap>
#include <QImage>
#include <QRunnable>
#include <QThreadPool>

#include "SleeperThread.h"

class DataMap;


class MapTile : public QGraphicsItem
{

private:
    DataMap *_map;
    int _size;
    int _tileX;
    int _tileY;
    bool _pixmapLoaded;
    QPixmap _pixmap;
    bool _imageLoaded;
    QImage _image;
    QGraphicsPixmapItem *_item;
    bool _isLoaded;
    bool _isLoading;

public:
    explicit MapTile(DataMap *map, int tileX, int tileY, int size, QGraphicsItem *parent = 0);
    QPixmap& pixmap();
    QImage& image();
    void loadData();
    void redraw() {
	this->update(0,0,image().width(),image().height());
    }

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:

public slots:

};




class MapTileLoader : public QRunnable {

private:
    QGraphicsPixmapItem *_item;
    MapTile *_tile;

public:
    MapTileLoader(MapTile *tile, QGraphicsPixmapItem *item) : QRunnable() {
	_item = item;
	_tile = tile;
    }

    void run(){
	//SleeperThread::msleep(1000); // Just for testing load delay
	_tile->loadData();
	_tile->redraw();
    }

};

#endif // MAPTILE_H
