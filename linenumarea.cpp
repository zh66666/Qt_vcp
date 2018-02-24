/**
 ** -------------------------------
 ** Create By:   Ryan LEUNG
 ** Date:        2009-09-23
 ** -------------------------------
 ** -------------------------------
 ** Modified by: Ryan LEUNG
 ** Date:        2010-01-11
 ** Content:     Add comments
 ** -------------------------------
 ** -------------------------------
 ** Modified by: Ryan LEUNG
 ** Date:        2010-01-13
 ** Content:     Add line number and syntax-highlighting
 ** -------------------------------
 **/

#include "linenumarea.h"

LineNumArea::LineNumArea(MdiChild* editor)
        : QWidget(editor)
{
    childEditor = editor;
}

QSize LineNumArea::sizeHint() const
{
    return QSize(childEditor->lineNumAreaWidth(), 0);
}

void LineNumArea::paintEvent(QPaintEvent* event)
{
    childEditor->lineNumAreaPaintEvent(event);
}
