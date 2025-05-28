#include "clickabletextedit.h"

ClickableTextEdit::ClickableTextEdit(QWidget *parent)
    : QTextEdit(parent)
{
    setReadOnly(true);
}

void ClickableTextEdit::mousePressEvent(QMouseEvent *event)
{
    emit clickedAt(event); // Kirim sinyal klik
    QTextEdit::mousePressEvent(event); // Tetap proses seperti biasa
}

void ClickableTextEdit::wheelEvent(QWheelEvent *event)
{
    emit userScrolled(); // sinyal bahwa user scroll manual
    QTextEdit::wheelEvent(event); // biar tetap bisa scroll
}
