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
#include <QtCore/qmath.h>

#include "DataMap.h"

struct LatLong {
    double latitude;
    double longitude;
};

class ImageDataMap : public DataMap
{

    Q_OBJECT

protected:
    // Map variables
    QString _imagePath;
    QString _tilesPath;
    QString _configPath;
    QString _configDir;
    QSettings *_mapSettings;
    QString _tileID;

    // Map meta
    QString _projection;
    double _centerLatitudeDeg;
    double _centerLongitudeDeg;
    double _resolutionPxPerDeg;
    double _scaleKmPerPx;
    double _maximumLatitudeDeg;
    double _minimumLatitudeDeg;
    double _easternmostLongitudeDeg;
    double _westernmostLongitudeDeg;
    double _projectionRadiusMeter;
    double _projectionWidthPixel;
    double _projectionHeightPixel;

public:
    ImageDataMap(QString configPath, QSettings *settings, QObject *parent = 0);
    ~ImageDataMap();
    virtual void load();
    virtual void loadTileImage(int tileX, int tileY, QImage &image);
    void generateTileImages();
    virtual void paintTileImage(QImage &pixmap, QPixmap &tile, int tileX, int tileY, int tileWidth, int tileHeight);
    LatLong getLatLongAtPixel(int x, int y);

    inline double getXResolutionAtPixel(int y) {
        return ( 2.0*M_PI*_projectionRadiusMeter*qCos( M_PI*(0.5-(y/_projectionHeightPixel)) ) ) / _projectionWidthPixel;
    };

    inline double getYResolutionAtPixel() {
        return (M_PI*_projectionRadiusMeter) / _projectionHeightPixel;
    };

};

#endif // IMAGEDATAMAP_H
