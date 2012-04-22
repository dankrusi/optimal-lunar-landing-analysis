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

#include "ElevationDataMap.h"

#include <QDebug>
#include <QtCore/qmath.h>
#include <QPixmap>
#include <QPainter>
#include <QDir>
#include <QFile>
#include <QGraphicsPixmapItem>
#include <QVector>

#include "DataMap.h"
#include "MapTile.h"

ElevationDataMap::ElevationDataMap(QString configPath, QSettings *settings, QObject *parent) : ImageDataMap(configPath,settings,parent)
{
    // Init
    _name = "Elevation Map";
    _tileID = "ElevationDataMap";
    _colorTablePath = _mapSettings->value("colortable_path","").toString();
    _colorTable = new QVector<ColorTableRow>();

    // Read the color table
    qDebug() << "Reading color table" << _colorTablePath << "...";
    QFile file(_colorTablePath);
    file.open(QFile::ReadOnly);
    QTextStream stream(&file);
    QString line = stream.readLine(); // Skip first line
    while(!line.isEmpty()) {
        // Read the line
        line = stream.readLine().trimmed().replace(" ","");
        QStringList columns = line.split('\t');
        if(columns.count() == 5) {
            int r = columns.at(1).toInt();
            int g = columns.at(2).toInt();
            int b = columns.at(3).toInt();
            int elevation = columns.at(4).toInt();
            ColorTableRow ctr;
            ctr.color = qRgb(r,g,b);
            ctr.elevation = elevation;
            _colorTable->append(ctr);
        }
    }
    file.close();
    qDebug() << "Table size: " << _colorTable->size();
}

ElevationDataMap::~ElevationDataMap() {
    delete _colorTable;
}

void ElevationDataMap::paintTileImage(QImage &pixmap, QPixmap &tile, int tileX, int tileY, int tileWidth, int tileHeight) {
    QPainter painter;
    painter.begin(&tile);

    // Draw the map using the elevation
    for(int x = 0; x < tileWidth; x++) {
        for(int y = 0; y < tileHeight; y++) {
            QRgb p = pixmap.pixel(tileX+x,tileY+y);
            double e = getElevationPercentFromElevation(getElevationFromColorRelief(p));
            unsigned char eByte = e*255;
            QRgb c = qRgba(eByte,eByte,eByte,255);
            painter.fillRect(x,y,1,1,c);
        }
    }

    painter.end();
}


double ElevationDataMap::getElevationAtPoint(int x, int y) {
    int minElevation = -9150; //TODO: refactor to settings
    int maxElevation = 10760;
    double percent = qRed(getDataAtPixel(x,y)) / 255.0; // The red channel is the height in percent 0...255
    return percent*maxElevation - percent*minElevation + minElevation;
}

double ElevationDataMap::getElevationPercentFromElevation(double elevation) {
    int minElevation = -9150; //TODO: refactor to settings
    int maxElevation = 10760;
    return (double)((int)elevation - minElevation) / (double)(maxElevation - minElevation);
}

// TODO: approximation, not exact
int ElevationDataMap::getElevationFromColorRelief(QRgb color) {
    int R = qRed(color);
    int G = qGreen(color);
    int B = qBlue(color);
    long I;
    long Imin = -1;
    long D, Dmin = LONG_MAX;
    for( I=0 ; I<_colorTable->count() ; I++ )
    {
        ColorTableRow key = _colorTable->at(I);
        int keyR = qRed(key.color);
        int keyG = qGreen(key.color);
        int keyB = qBlue(key.color);
        D = ( R-keyR )*( R-keyR ) + ( G-keyG )*( G-keyG ) + ( B-keyB )*( B-keyB );
        if( D < Dmin )
        {
            Dmin = D;
            Imin = I;
        }
    }
    if(Imin < 0) return -1;
    return( _colorTable->at(Imin).elevation );
}
