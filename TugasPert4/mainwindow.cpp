#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    m_drawingCanvas = new DrawingCanvas(this);
    m_drawingCanvas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(m_drawingCanvas);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(10, 10, 10, 10);
    buttonLayout->setSpacing(15);

    m_calculateFastButton = new QPushButton("Run Fast Convex Hull");
    m_calculateFastButton->setFont(QFont("Inter", 10, QFont::Bold));
    m_calculateFastButton->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; padding: 10px 15px; border-radius: 8px; }"
        "QPushButton:hover { background-color: #45a049; }"
        );
    buttonLayout->addWidget(m_calculateFastButton);

    m_calculateSlowButton = new QPushButton("Run Slow Convex Hull");
    m_calculateSlowButton->setFont(QFont("Inter", 10, QFont::Bold));
    m_calculateSlowButton->setStyleSheet(
        "QPushButton { background-color: #2196F3; color: white; padding: 10px 15px; border-radius: 8px; }"
        "QPushButton:hover { background-color: #1e88e5; }"
        );
    buttonLayout->addWidget(m_calculateSlowButton);

    m_resetButton = new QPushButton("Clear Canvas");
    m_resetButton->setFont(QFont("Inter", 10));
    m_resetButton->setStyleSheet(
        "QPushButton { background-color: #f44336; color: white; padding: 10px 15px; border-radius: 8px; }"
        "QPushButton:hover { background-color: #da190b; }"
        );
    buttonLayout->addWidget(m_resetButton);

    mainLayout->addLayout(buttonLayout);

    connect(m_calculateFastButton, &QPushButton::clicked, m_drawingCanvas, &DrawingCanvas::calculateFastHull);
    connect(m_calculateSlowButton, &QPushButton::clicked, m_drawingCanvas, &DrawingCanvas::calculateSlowHull);
    connect(m_resetButton, &QPushButton::clicked, m_drawingCanvas, &DrawingCanvas::resetPoints);
}

MainWindow::~MainWindow()
{
}
