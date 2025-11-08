#include "MenuWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QApplication>
#include <QProcess>
#include <thread>
#include <memory>
#include <iostream>
#include "../engines/GameEngine.h"

MenuWindow::MenuWindow(QWidget* parent) : QMainWindow(parent) {
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    setWindowTitle("FPS 2.5D - Menu");
    resize(420, 220);

    title = new QLabel("FPS 2.5D - Shooter", this);
    QFont f = title->font();
    f.setPointSize(18);
    f.setBold(true);
    title->setFont(f);
    title->setAlignment(Qt::AlignCenter);

    difficultyBox = new QComboBox(this);
    difficultyBox->addItem("Facil");
    difficultyBox->addItem("Normal");
    difficultyBox->addItem("Dificil");

    playBtn = new QPushButton("Jugar", this);
    connect(playBtn, &QPushButton::clicked, this, &MenuWindow::onPlay);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(title);
    layout->addSpacing(12);
    layout->addWidget(difficultyBox);
    layout->addSpacing(8);
    layout->addWidget(playBtn);
    layout->setAlignment(playBtn, Qt::AlignHCenter);

    central->setLayout(layout);
}

void MenuWindow::onPlay() {
    QString diff = difficultyBox->currentText();
    int difficulty = 1;
    if (diff == "Facil") difficulty = 0;
    else if (diff == "Normal") difficulty = 1;
    else difficulty = 2;

    // Ejecutar el binario del juego en un proceso separado
    QString program = QApplication::applicationFilePath();
    QStringList args;
    args << "--play" << QString::number(difficulty);

    QProcess* process = new QProcess(this);
    process->start(program, args);
}
