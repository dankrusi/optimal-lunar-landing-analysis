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

#include "DataMap.h"
#include "MapTile.h"

ElevationDataMap::ElevationDataMap(QString configPath, QSettings *settings, QObject *parent) : ImageDataMap(configPath,settings,parent)
{
    // Init
    _name = "Elevation Map";
    _colorTablePath = _mapSettings->value("colortable_path","").toString();
    _colorTable = new QList<ColorTableRow>();

    // Read the color table
    qDebug() << "Reading color table" << _colorTablePath;
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
}

ElevationDataMap::~ElevationDataMap() {
    delete _colorTable;
}


double ElevationDataMap::getElevationAtPoint(int x, int y) {
    QRgb p = getDataAtPixel(x,y);
    return qRed(p); //TODO
}


double ElevationDataMap::getElevationFromColor(QRgb c) {

}
