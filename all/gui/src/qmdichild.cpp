#include "qmdichild.h"

QMdichild::QMdichild()
{
    setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;
}

QMdichild::~QMdichild()
{

}
void QMdichild::newFile()
{
    static int sequenceNumber = 1;

    isUntitled = true;
    curFile = tr("document%1.txt").arg(sequenceNumber++);
    setWindowTitle(curFile + "[*]");

    connect(document(), SIGNAL(contentsChanged()),
        this, SLOT(documentWasModified()));
}
void QMdichild::documentWasModified()
{
    setWindowModified(document()->isModified());
}