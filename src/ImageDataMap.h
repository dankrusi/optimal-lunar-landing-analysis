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

#ifndef IMAGEDATAMAP_H
#define IMAGEDATAMAP_H

#include <QObject>
#include <QGraphicsItem>
#include <QSettings>

#include "DataMap.h"

class ImageDataMap : public DataMap
{

    Q_OBJECT

protected:
    QString _imagePath;
    QString _tilesPath;
    QString _configPath;
    QSettings *_mapSettings;

public:
    ImageDataMap(QString configPath, QSettings *settings, QObject *parent = 0);
    ~ImageDataMap();
    virtual void load();
    virtual void loadTileImage(int tileX, int tileY, QImage &image);
    void generateTileImages();

};

#endif // IMAGEDATAMAP_H
