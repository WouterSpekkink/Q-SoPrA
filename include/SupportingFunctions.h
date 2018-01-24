#ifndef SUPPORTINGFUNCTIONS_H
#define SUPPORTINGFUNCTIONS_H

#include <QString>
#include <QVector>

QString breakString(QString original);
QString doubleQuote(QString original);
QString removeChar(QString original, QChar chara);
QVector<QString> splitLines(QString original);

#endif 
