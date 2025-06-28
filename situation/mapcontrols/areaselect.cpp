#include "areaselect.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QApplication>
#include <QScreen>

AreaSelect::AreaSelect()
{
    // Get screen information for size calculation
    QScreen* screen = QApplication::primaryScreen();
    QSize screenSize = screen->size();
    int screenWidth = screenSize.width();
    int screenHeight = screenSize.height();
    
    // Calculate button size based on screen resolution
    // Base size for 1920x1080 resolution
    double scaleFactor = qMin(screenWidth / 1920.0, screenHeight / 1080.0);
    scaleFactor = qMax(scaleFactor, 0.5); // Minimum scale factor
    scaleFactor = qMin(scaleFactor, 2.0); // Maximum scale factor
    
    int buttonWidth = qRound(80 * scaleFactor);
    int buttonHeight = qRound(30 * scaleFactor);
    int maxButtonWidth = qRound(100 * scaleFactor);
    int maxButtonHeight = qRound(35 * scaleFactor);
    
    // Calculate widget size
    int widgetWidth = qRound(110 * scaleFactor);
    int widgetHeight = qRound(130 * scaleFactor);
    
    // Calculate font size
    int fontSize = qRound(12 * scaleFactor);
    fontSize = qMax(fontSize, 8); // Minimum font size
    fontSize = qMin(fontSize, 16); // Maximum font size
    
    // Create buttons
    m_taskAreaButton = new QPushButton(tr("Misson Area"), this);
    m_noFlyAreaButton = new QPushButton(tr("NoFly Area"), this);
    m_threatAreaButton = new QPushButton(tr("Threat Area"), this);
    
    // Set button properties with calculated sizes
    m_taskAreaButton->setMinimumSize(buttonWidth, buttonHeight);
    m_noFlyAreaButton->setMinimumSize(buttonWidth, buttonHeight);
    m_threatAreaButton->setMinimumSize(buttonWidth, buttonHeight);
    
    m_taskAreaButton->setMaximumSize(maxButtonWidth, maxButtonHeight);
    m_noFlyAreaButton->setMaximumSize(maxButtonWidth, maxButtonHeight);
    m_threatAreaButton->setMaximumSize(maxButtonWidth, maxButtonHeight);
    
    // Set button styles with dynamic font size
    QString buttonStyle = QString(
        "QPushButton {"
        "    background-color: #4a90e2;"
        "    color: white;"
        "    border: 1px solid #357abd;"
        "    border-radius: 4px;"
        "    padding: 5px;"
        "    font-size: %1px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #357abd;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #2d5986;"
        "}").arg(fontSize);
    
    m_taskAreaButton->setStyleSheet(buttonStyle);
    m_noFlyAreaButton->setStyleSheet(buttonStyle);
    m_threatAreaButton->setStyleSheet(buttonStyle);
    
    // Create vertical layout with dynamic spacing
    m_layout = new QVBoxLayout(this);
    int spacing = qRound(8 * scaleFactor);
    int margins = qRound(5 * scaleFactor);
    m_layout->setSpacing(spacing);
    m_layout->setContentsMargins(margins, margins, margins, margins);
    
    // Add buttons to layout
    m_layout->addWidget(m_taskAreaButton);
    m_layout->addWidget(m_noFlyAreaButton);
    m_layout->addWidget(m_threatAreaButton);
    
    // Set layout
    setLayout(m_layout);
    
    // Connect signals
    connect(m_taskAreaButton, &QPushButton::clicked, this, &AreaSelect::onTaskAreaClicked);
    connect(m_noFlyAreaButton, &QPushButton::clicked, this, &AreaSelect::onNoFlyAreaClicked);
    connect(m_threatAreaButton, &QPushButton::clicked, this, &AreaSelect::onThreatAreaClicked);
    
    // Set widget properties with calculated size
    setFixedSize(widgetWidth, widgetHeight);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    
    // Set anchor to left center with dynamic offset
    int anchorOffset = qRound(10 * scaleFactor);
    setAnchor(QPoint(anchorOffset, 0), {Qt::LeftEdge});
}

void AreaSelect::onTaskAreaClicked()
{
    emit taskAreaSelected();
}

void AreaSelect::onNoFlyAreaClicked()
{
    emit noFlyAreaSelected();
}

void AreaSelect::onThreatAreaClicked()
{
    emit threatAreaSelected();
}
