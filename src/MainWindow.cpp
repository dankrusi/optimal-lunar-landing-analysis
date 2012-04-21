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

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGLWidget>
#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <QDataStream>
#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QAction>
#include <QFileDialog>
#include <QInputDialog>

#include "SlopeAnalysisMap.h"
#include "ExponentialSlopeAnalysisMap.h"
#include "CombinedAnalysisMap.h"
#include "ElevationAnalysisMap.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Init
    _tilesLoading = 0;
    _tilesLoaded = 0;
    _settings = new QSettings("OptimalLunarLandingAnalysis.config",QSettings::NativeFormat,this);
    _settings->setValue("first_run","false");
    _settings->sync();

    // GUI
    ui->setupUi(this);
    this->setWindowTitle("Optimal Lunar Landing Analysis");
    ui->viewport->setMouseTracking(true);
    ui->viewport->setInteractive(true);
    ui->viewport->setMouseTracking(true);
    ui->viewport->viewport()->setMouseTracking(true);
    on_zoomResetButton_clicked(); // reset zoom button

    // Status bar
    _progressLabel = new QLabel("",this);
    ui->statusBar->addPermanentWidget(_progressLabel);

    // Actions
    // Icons: http://standards.freedesktop.org/icon-naming-spec/icon-naming-spec-latest.html
    QMenu *menuFile = new QMenu(tr("&File"), this);
    menuBar()->addMenu(menuFile);
    registerAction(menuFile,tr("&Load Map..."),QIcon::fromTheme("document-open"),SLOT(openMapFile()));
    registerAction(menuFile,tr("&New Map..."),QIcon::fromTheme("document-new"),SLOT(newMapFile()));
    registerAction(menuFile,tr("&Exit"),QIcon::fromTheme("application-exit"),SLOT(close()));
    ui->mainToolBar->addSeparator();
    QMenu *menuView = new QMenu(tr("&View"), this);
    menuBar()->addMenu(menuView);
    registerAction(menuView,tr("&Redraw"),QIcon::fromTheme("view-refresh"),SLOT(redrawViewport()));
    registerAction(menuView,tr("&Zoom Out"),QIcon::fromTheme("zoom-out"),SLOT(on_zoomOutButton_clicked()));
    registerAction(menuView,tr("&Zoom In"),QIcon::fromTheme("zoom-in"),SLOT(on_zoomInButton_clicked()));
    registerAction(menuView,tr("&Zoom 1:1"),QIcon::fromTheme("zoom-original"),SLOT(on_zoomResetButton_clicked()));

    // Scene
    _scene = new ResponsiveGraphicsScene(this);
    connect(_scene,SIGNAL(mouseMoved(int,int)),this,SLOT(viewportCursorMoved(int,int)));
    connect(_scene,SIGNAL(mouseReleased()),this,SLOT(redrawViewport()));

    // Accelerated graphics
    if(_settings->value("use_opengl","false") == "true") ui->viewport->setViewport(new QGLWidget());

    // Connections
    //connect(ui->zoom,SIGNAL(sliderMoved(int)),this,SLOT(zoomViewport(int)));

    // Autostart
    QCoreApplication::processEvents();
    QTimer::singleShot(100, this, SLOT(autoStart()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::registerDataMap(DataMap *map) {

    // Connections
    connect(map,SIGNAL(tileLoading(MapTile*)),this,SLOT(tileLoading(MapTile*)));
    connect(map,SIGNAL(tileLoaded(MapTile*)),this,SLOT(tileLoaded(MapTile*)));
    connect(map,SIGNAL(mapLoading(double)),this,SLOT(mapLoading(double)));

    // Load the map and register it
    map->load();
    _dataMaps.append(map);

    // Update scene
    _scene->addItem(map->layer());
    ui->viewport->setScene(_scene);

    // Register in GUI
    DataMapListWidgetItem *item = new DataMapListWidgetItem(map->name(),ui->dataMapsList);
    item->setCheckState(Qt::Checked);
    item->map = map;
}

void MainWindow::registerAnalysisMap(AnalysisMap *map) {

    // Connections
    connect(map,SIGNAL(tileLoading(MapTile*)),this,SLOT(tileLoading(MapTile*)));
    connect(map,SIGNAL(tileLoaded(MapTile*)),this,SLOT(tileLoaded(MapTile*)));
    connect(map,SIGNAL(mapLoading(double)),this,SLOT(mapLoading(double)));

    // Load the map and register it
    map->load();
    _analysisMaps.append(map);

    // Update scene
    map->layer()->setVisible(false);
    _scene->addItem(map->layer());

    // Register in GUI
    DataMapListWidgetItem *item = new DataMapListWidgetItem(map->name(),ui->analysisMapsList);
    item->setCheckState(Qt::Unchecked);
    item->map = map;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}





void MainWindow::registerAction(QMenu *menu, QString title, QIcon icon, const char *member) {
    QAction *a;
    a = new QAction(title, this);
    a->setIcon(icon);
    a->setPriority(QAction::LowPriority);
    //a->setShortcut(QKeySequence::New);
    connect(a, SIGNAL(triggered()), this, member);
    ui->mainToolBar->addAction(a);
    menu->addAction(a);
}







void MainWindow::autoStart() {
    // Load the last opened map file
    openMapFile(_settings->value("last_loaded_map","").toString());
}

void MainWindow::redrawViewport() {
    ui->viewport->viewport()->update();
}

void MainWindow::showLoadProgress(int percent) {
    ui->statusBar->showMessage(QString("Loading %1%").arg(percent));
}

void MainWindow::viewportCursorMoved(int x,int y) {
    int elevation = _elevationMap->getElevationAtPoint(x,y);

    int minElevation = -9150;
    int maxElevation = 10760;

    double percent = (double)(elevation - minElevation) / (double)(maxElevation - minElevation);

    ui->statusBar->showMessage(QString("Elevation: %1 %2%").arg(elevation).arg(percent));
}

void MainWindow::updateLoadingStatus() {
    int totalTiles = _tilesLoaded+_tilesLoading;
    _progressLabel->setText(QString("Loading tile %1/%2").arg(_tilesLoaded).arg(totalTiles));
}

void MainWindow::tileLoading(MapTile *tile) {
    _tilesLoading++;
}

void MainWindow::tileLoaded(MapTile *tile) {
    _tilesLoaded++;
    _tilesLoading--;
    this->redrawViewport();
    updateLoadingStatus();
    QCoreApplication::processEvents();
}

void MainWindow::mapLoading(double progress) {
    int percent = (int)(progress*100);
    _progressLabel->setText(QString("Loading map %1%").arg(percent));
    QCoreApplication::processEvents();
}

void MainWindow::newMapFile() {
    // Get file path
    QString filePath = QFileDialog::getOpenFileName(this,tr("New Map File"), "", tr("Image Files (*.tif *.tiff *.png *.jpg *.bmp)"));
    if(filePath.isEmpty()) return;

    // Get tile size
    bool ok;
    int tileSize = QInputDialog::getInt(this, tr("New Map File"),tr("Tile size:"), 256, 64, 1024, 64, &ok);
    if(!ok) return;

    // Create new map file
    QString mapPath = filePath + ".map";
    qDebug() << "Creating new map " << filePath;
    QFile::remove(mapPath);
    QSettings settings(mapPath,QSettings::NativeFormat,this);
    settings.setValue("map_path",filePath);
    settings.setValue("tile_size",tileSize);
    settings.sync();

    // Now load it...
    openMapFile(mapPath);
}

void MainWindow::openMapFile() {
    // Open file
    QString filePath = QFileDialog::getOpenFileName(this,tr("Open Map"), "", tr("Map Files (*.map)"));
    if(filePath.isEmpty()) return;

    // Load it
    openMapFile(filePath);
}

void MainWindow::openMapFile(QString filePath) {
    // Check if exists
    if(!QFile::exists(filePath)) return;

    // Register as last used
    _settings->setValue("last_loaded_map",filePath);

    // Load maps
    //TODO
    _elevationMap = new ElevationDataMap(filePath,_settings,this);
    registerDataMap(_elevationMap);

    ElevationAnalysisMap *elevationAnalysisMap = new ElevationAnalysisMap(_elevationMap,_settings,this);
    registerAnalysisMap(elevationAnalysisMap);

    SlopeAnalysisMap *slopeMap = new SlopeAnalysisMap(elevationAnalysisMap,_settings,this);
    registerAnalysisMap(slopeMap);

    ExponentialSlopeAnalysisMap *expSlopeMap = new ExponentialSlopeAnalysisMap(elevationAnalysisMap,_settings,this);
    registerAnalysisMap(expSlopeMap);

    CombinedAnalysisMap *combinedMap = new CombinedAnalysisMap(_elevationMap,_settings,this);
    registerAnalysisMap(combinedMap);
}






void MainWindow::on_zoomSlider_valueChanged(int value) {
    double scale = (double)value / (double)ui->zoomSlider->maximum() + ZOOM_SLIDER_OFFSET;
    if(scale > 1.0-ZOOM_SNAP_TOLERANCE && scale < 1.0+ZOOM_SNAP_TOLERANCE) scale = 1.0;
    QTransform trans;
    trans.scale(scale,scale);
    ui->viewport->setTransform(trans,false);
}

void MainWindow::on_zoomResetButton_clicked() {
    int value = (ZOOM_RESET_RATIO - ZOOM_SLIDER_OFFSET) * ui->zoomSlider->maximum();
    ui->zoomSlider->setValue(value);
}

void MainWindow::on_zoomInButton_clicked() {
    ui->zoomSlider->setValue(ui->zoomSlider->value()+ZOOM_INCREMENT);
}

void MainWindow::on_zoomOutButton_clicked() {
    ui->zoomSlider->setValue(ui->zoomSlider->value()-ZOOM_INCREMENT);
}

void MainWindow::on_dataMapsList_itemChanged(QListWidgetItem *item) {
    DataMapListWidgetItem *dataMapItem = (DataMapListWidgetItem*)item;
    if(dataMapItem->map) {
	dataMapItem->map->layer()->setVisible(dataMapItem->checkState() == Qt::Checked);
	redrawViewport();
    }
}

void MainWindow::on_analysisMapsList_itemChanged(QListWidgetItem *item) {
    on_dataMapsList_itemChanged(item);
}

