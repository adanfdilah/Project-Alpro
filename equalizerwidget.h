#ifndef EQUALIZERWIDGET_H
#define EQUALIZERWIDGET_H

#include <QWidget>
#include <QTimer>

class EqualizerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EqualizerWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<int> barHeights;
    QTimer *animationTimer;

private slots:
    void updateBars();
};

#endif // EQUALIZERWIDGET_H
