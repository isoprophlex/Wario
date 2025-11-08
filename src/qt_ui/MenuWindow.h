#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>

class MenuWindow : public QMainWindow {
    Q_OBJECT
public:
    MenuWindow(QWidget* parent = nullptr);
    ~MenuWindow() = default;

    private slots:
        void onPlay();

private:
    QPushButton* playBtn;
    QComboBox* difficultyBox;
    QLabel* title;
};

#endif // MENUWINDOW_H
