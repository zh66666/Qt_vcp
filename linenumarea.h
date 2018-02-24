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

#ifndef LINENUMAREA_H
#define LINENUMAREA_H

#include <QWidget>
#include "mdichild.h"

class MdiChild;
QT_BEGIN_NAMESPACE
class QWidget;
class QPaintEvent;
QT_END_NAMESPACE

class LineNumArea : public QWidget
{
public:
    LineNumArea(MdiChild* editor);
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent* event);

private:
    MdiChild* childEditor;
};

#endif // LINENUMAREA_H
