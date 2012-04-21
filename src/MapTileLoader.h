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

#ifndef MAPTILELOADER_H
#define MAPTILELOADER_H

#include <QObject>
#include <QGraphicsItem>
#include <QPixmap>
#include <QImage>
#include <QRunnable>
#include <QThreadPool>
#include <QDebug>

class MapTile;

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
        //_tile->setVisible(true);
    }

};

#endif // MAPTILELOADER_H
