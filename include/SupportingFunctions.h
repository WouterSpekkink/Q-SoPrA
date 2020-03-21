/*

Qualitative Social Process Analysis (Q-SoPrA)
Copyright (C) 2019 University of Manchester  

This file is part of Q-SoPrA.

Q-SoPrA is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Q-SoPrA is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Q-SoPrA.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef SUPPORTINGFUNCTIONS_H
#define SUPPORTINGFUNCTIONS_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QSet>

QString breakString(QString original);
QString doubleQuote(QString original);
QString removeChar(QString original, QChar chara);
QVector<QString> splitLines(QString original);
QString fixBreakLines(QString original);
void findPastPaths(QSet<int> *paths,
		   QMap<int, QSet<int>> *headsMap,
		   int currentIncident);
void findFuturePaths(QSet<int> *paths,
		     QMap<int, QSet<int>> *tailsMap,
		     int currentIncident);

#endif 
