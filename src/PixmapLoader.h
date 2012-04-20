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

#ifndef PIXMAPLOADER_H
#define PIXMAPLOADER_H

#include <QRunnable>
#include <QPixmap>
#include <QFile>
#include <QByteArray>
#include <QDebug>
#include <QCoreApplication>

class PixmapLoader : public QObject
{
    Q_OBJECT

private:
    QPixmap *_pixmap;

public:
    PixmapLoader(QPixmap *pixmap) {
	_pixmap = pixmap;
    };

    ~PixmapLoader() {

    };

    void run() {

    };

    void loadFromFile(QString filePath) {

	QFile file(filePath);
	file.open(QIODevice::ReadOnly);

	QByteArray bytes;
	qint64 fileSize = file.size();
	qint64 bytesRead = 0;
	char buffer[256];
	int lastPercent = -1;
	while(!file.atEnd()) {
	    qint64 len = file.read(buffer,256);
	    bytesRead += len;
	    bytes.append(buffer,len);
	    int percent = (int)(((double)bytesRead/(double)fileSize)*100);
	    if(percent != lastPercent) {
		lastPercent = percent;
		emit loadProgress(percent);
		QCoreApplication::processEvents();
	    }
	}

	_pixmap->loadFromData(bytes);
	emit loadFinished();
    }

signals:
    void loadFinished();
    void loadProgress(int percent);

};

#endif // PIXMAPLOADER_H
