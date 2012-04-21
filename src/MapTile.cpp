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

#include "MapTile.h"

#include <QDebug>

#include "DataMap.h"
#include "MapTileLoader.h"

MapTile::MapTile(DataMap *map, int tileX, int tileY, int size, QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    // Init
    _item = NULL;
    _isLoaded = false;
    _isLoading = false;
    _pixmapLoaded = false;
    _imageLoaded = false;
    _size = size;
    _tileX = tileX;
    _tileY = tileY;
    _map = map;
    //this->setVisible(false);
}

MapTile::~MapTile() {

}




QPixmap& MapTile::pixmap() {
    //while(_isLoading) {} // Wait if we are already loading...
    if(!_pixmapLoaded) {
	qDebug() << "Loading" << _map->name() << "tile pixmap" << _tileX << _tileY << "...";
    _pixmap = QPixmap::fromImage(image());
	_pixmapLoaded = true;
    }
    return _pixmap;
}



QImage& MapTile::image() {
    while(_isLoading) {} // Wait if we are already loading...
    if(!_imageLoaded) {
	loadData();
    }
    return _image;
}

QRectF MapTile::boundingRect() const {
    return QRectF(0,0,_size,_size);
}

void MapTile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    // Do we need to load and are we not already loading?
    if(_isLoaded == false && _isLoading == false){

	// Init
	_isLoading = true;

	// Load the tile in a seperate thread...
	MapTileLoader *loader = new MapTileLoader(this,_item);
	loader->setAutoDelete(true);
	QThreadPool::globalInstance()->start(loader);

    }

    // Are we loaded and ready to display?
    if(_isLoaded) {
	if(_item == NULL) {
	    _item = new QGraphicsPixmapItem(this->pixmap());
	}
	_item->paint(painter, option, widget);
    }
}

void MapTile::loadData() {
    _isLoading = true;
    _isLoaded = false;

    _map->registerTileLoading(this);

    qDebug() << "Loading" << _map->name() << "tile image" << _tileX << _tileY << "...";
    _map->loadTileImage(_tileX,_tileY,_image);
    _imageLoaded = true;

    _isLoaded = true;
    _isLoading = false;

    _map->registerTileLoaded(this);

}

void MapTile::redraw() {
    this->update(0,0,_size,_size);
}

/*
void MapTile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if(!_item){
	_item = new QGraphicsPixmapItem(this->pixmap());
    }
    _item->paint(painter, option, widget);
}*/
