#ifndef QMDICHILD_H
#define QMDICHILD_H

#include <QTextEdit>

class QMdichild : public QTextEdit
{
    Q_OBJECT

public:
    QMdichild();
    ~QMdichild();


    void newFile();

private slots:
    void documentWasModified();
private:
     
    QString curFile;
    bool isUntitled;
    
};

#endif // QMDICHILD_H
