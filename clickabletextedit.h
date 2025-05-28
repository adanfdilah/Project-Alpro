#ifndef CLICKABLETEXTEDIT_H
#define CLICKABLETEXTEDIT_H

#include <QTextEdit>
#include <QMouseEvent>

class ClickableTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit ClickableTextEdit(QWidget *parent = nullptr);

signals:
    void clickedAt(QMouseEvent *event);
    void userScrolled();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

};

#endif // CLICKABLETEXTEDIT_H
