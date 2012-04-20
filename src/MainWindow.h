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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QGraphicsScene>
#include <QListWidgetItem>

#include "SleeperThread.h"
#include "PixmapLoader.h"
#include "ElevationDataMap.h"
#include "AnalysisMap.h"
#include "ResponsiveGraphicsScene.h"
#include "DataMapListWidgetItem.h"

#define ZOOM_RESET_RATIO 1.0
#define ZOOM_SLIDER_OFFSET 0.3
#define ZOOM_INCREMENT 1
#define ZOOM_SNAP_TOLERANCE 0.01
#define DATAMAP_ROLE 1

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {

    Q_OBJECT

private:
    Ui::MainWindow *ui;
    ResponsiveGraphicsScene *_scene;
    ElevationDataMap *_elevationMap;
    QList<DataMap*> _dataMaps;
    QList<AnalysisMap*> _analysisMaps;

private:
    QSettings *_settings;
    QPixmap _heightMap;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    void registerAction(QMenu *menu, QString title, QIcon icon, const char *member);

public slots:
    void autoStart();
    void redrawViewport();
    void registerDataMap(DataMap *map);
    void registerAnalysisMap(AnalysisMap *map);
    void showLoadProgress(int percent);
    void viewportCursorMoved(int x,int y);
    void newMapFile();
    void openMapFile();
    void openMapFile(QString filePath);
    void on_zoomSlider_valueChanged(int value);
    void on_zoomResetButton_clicked();
    void on_zoomInButton_clicked();
    void on_zoomOutButton_clicked();
    void on_dataMapsList_itemChanged(QListWidgetItem *item);
    void on_analysisMapsList_itemChanged(QListWidgetItem *item);
};

#endif // MAINWINDOW_H
