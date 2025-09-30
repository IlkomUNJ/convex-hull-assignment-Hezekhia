#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "drawingcanvas.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    DrawingCanvas *m_drawingCanvas;
    QPushButton *m_calculateFastButton;
    QPushButton *m_calculateSlowButton;
    QPushButton *m_resetButton;
};

#endif
