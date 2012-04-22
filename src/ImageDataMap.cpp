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

#include "ImageDataMap.h"

#include <QDebug>
#include <QPixmap>
#include <QPainter>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QGraphicsPixmapItem>

#include "MapTile.h"

ImageDataMap::ImageDataMap(QString configPath, QSettings *settings, QObject *parent) : DataMap(settings,parent)
{
    // Init
    _name = "Image Map";
    _configPath = configPath;
    _configDir = QFileInfo(configPath).absoluteDir().path()+QDir::separator();
    _mapSettings = new QSettings(configPath,QSettings::IniFormat,this);
    _imagePath = _configDir+_mapSettings->value("map_path").toString();
    _tilesPath = _imagePath + QString(".tiles");
    _tileID = "ImageDataMap";

    // Map meta
    _projection = _mapSettings->value("projection","equirectangular").toString();
    _centerLatitudeDeg = _mapSettings->value("center_latitude_deg","0").toDouble();
    _centerLongitudeDeg = _mapSettings->value("center_longitude_deg","0").toDouble();
    _resolutionPxPerDeg = _mapSettings->value("resolution_pixperdeg","16.0").toDouble();
    _scaleKmPerPx = _mapSettings->value("scale_kmperpix","1.8952094015093").toDouble();
    _maximumLatitudeDeg = _mapSettings->value("maximum_latitude_deg","-60.0").toDouble();
    _minimumLatitudeDeg = _mapSettings->value("minimum_latitude_deg","-90.0").toDouble();
    _easternmostLongitudeDeg = _mapSettings->value("easternmost_longitude_deg","360.0").toDouble();
    _westernmostLongitudeDeg = _mapSettings->value("westernmost_longitude_deg","0.0").toDouble();
    _projectionRadiusMeter = _mapSettings->value("projection_radius_meter","1737400").toDouble();
    _projectionWidthPixel = _mapSettings->value("projection_width_px","0").toDouble();
    _projectionHeightPixel = _mapSettings->value("projection_height_px","0").toDouble();

    qDebug() << "New map" << _configPath;
}

ImageDataMap::~ImageDataMap() {

}



void ImageDataMap::load() {

    emit mapLoading(0);

    // Do we have tiles?
    _tileSize = _mapSettings->value("tile_size","128").toInt();
    if(!QFile::exists(_tilesPath) || _mapSettings->value(QString("%1_tiles_generated").arg(_tileID),"false") != "true") {
        qDebug() << "Will generate tiles for" << _imagePath;
        generateTileImages();
    }

    // Set tile settings
    _tilesX = _mapSettings->value("tiles_x","0").toInt();
    _tilesY = _mapSettings->value("tiles_y","0").toInt();

    // Create tile objects
    createTiles();

    emit mapLoading(1);
}

void ImageDataMap::paintTileImage(QImage &pixmap, QPixmap &tile, int tileX, int tileY, int tileWidth, int tileHeight) {
    QPainter painter;
    painter.begin(&tile);
    painter.drawImage(0,0,pixmap,tileX,tileY,tileWidth,tileHeight);
    painter.end();
}


void ImageDataMap::generateTileImages() {

    // Load pixmap
    QImage pixmap(_imagePath);

    // Init
    int tilesX = qCeil((double)pixmap.width() / (double)_tileSize);
    int tilesY = qCeil((double)pixmap.height() / (double)_tileSize);
    int tileCount = 0;
    int tileTotal = tilesX * tilesY;
    double lastPercent = -1;

    QDir dir;
    dir.mkpath(_tilesPath);

    // Loop tile grid
    for(int y = 0; y < tilesY; y++) {
        for(int x = 0; x < tilesX; x++) {
            // Init
            tileCount++;
            QString tilePath = _tilesPath + QDir::separator() + (QString("%1_%2_%3.png").arg(_tileID).arg(x).arg(y));
            qDebug() << "Processing tile" << tileCount << "of" << (tilesX*tilesY) << "...";
            int xx = x*_tileSize;
            int yy = y*_tileSize;
            int width = _tileSize;
            int height = _tileSize;
            if(xx + width > pixmap.width()) width = _tileSize - (xx + width - pixmap.width());
            if(yy + height > pixmap.height()) height = _tileSize - (yy + height - pixmap.height());
            double percent = (double)tileCount/(double)tileTotal;

            // Draw tile
            QPixmap tile(width,height);
            paintTileImage(pixmap,tile,xx,yy,width,height);

            // Save to file
            tile.save(tilePath);

            // Show progress
            if(percent != lastPercent) {
                emit mapLoading(percent);
                lastPercent = percent;
            }
        }
    }

    // Save to settings
    //_mapSettings->setValue("tile_path",_tilesPath);
    _mapSettings->setValue("tile_size",_tileSize);
    _mapSettings->setValue("tiles_x",tilesX);
    _mapSettings->setValue("tiles_y",tilesY);
    _mapSettings->setValue(QString("%1_tiles_generated").arg(_tileID),"true");
    //_mapSettings->setValue("map_path",_imagePath);
    _mapSettings->setValue("map_width",pixmap.width());
    _mapSettings->setValue("map_height",pixmap.height());
    _mapSettings->setValue("projection_width_px",pixmap.width());
    _mapSettings->setValue("projection_height_px",pixmap.height());
    _mapSettings->sync();
}

void ImageDataMap::loadTileImage(int tileX, int tileY, QImage &image) {
    QString tilePath = _tilesPath + (QString("/%1_%2_%3.png").arg(_tileID).arg(tileX).arg(tileY));
    image.load(tilePath);
}

LatLong ImageDataMap::getLatLongAtPixel(int x, int y) {
    //TODO: refine this for all the map meta and projection types
    LatLong latlong;
    latlong.longitude = x/_resolutionPxPerDeg;
    latlong.latitude = -y/_resolutionPxPerDeg + 90;
    return latlong;
}
