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

#include "../include/SupportingFunctions.h"

/*
  This function supports the creation of tool tips. It wraps longer fragments
  of text by introducing newline symbols after a certain length.
  It does keep words intact.
*/
QString breakString(QString original) 
{
  QString result = "";
  if (original.length() <= 50) 
    {
      result = original;
    }
  else 
    {
      QString subString = "";
      int count = 0;
      for (QString::size_type it = 0; it != original.length(); it++) 
	{
	  subString.append(original[it]);
	  count++;
	  if (count >= 50 || original[it] == '\n') 
	    {
	      if (it != original.length() - 1) 
		{
		  if (original[it + 1] == '\n') 
		    {
		      result += subString;
		      subString = "";
		      count = 0;
		    }
		  else if (original[it + 1] == ' ') 
		    {
		      subString.append("\n");
		      result += subString;
		      subString = "";
		      count = 0;
		    }
		}
	    }
	}
      result += subString;
    }
  return result;
}

/*
  This function is used before exporting strings to csv-files.
  It looks for embedded double quotes and ensures that they are duplicated.
  This is to comply to the csv-standard, so that exported csv-files are read correctly.
*/
QString doubleQuote(QString original) 
{
  QString copy = QString();
  for (QString::size_type i = 0; i != original.length(); i++) 
    {
      copy.append(original[i]);
      if (original[i] == '"') 
	{
	  copy.append(original[i]);
	}
    }
  return copy;
}

/*
  I encountered an annoying problem where Qt always returns texts from
  QCheckBoxes with ampersands, even when I did not assign one. I therefore 
  wrote this function that ensures that the ampersand is removed whenever one is present.
*/
QString removeChar(QString original, QChar chara) 
{
  QString newString = QString();
  for (QString::size_type i = 0; i != original.length(); i++) 
    {
      if (original[i] != chara) 
	{
	  newString.append(original[i]);
	}
    }
  return newString;
}

/*
  I found out that the find() function of QTextEdit fields does not attempt
  to find words across paragraph/line breaks. That was a problem for my
  text highlighting function, because the text to be highlighted might contain
  multiple lines. 

  In this case, the solution I came up with is to break the text to be highlighted
  into separate lines. QTextCursor->selectedText() uses QChar::ParagraphSeparator 
  as newline symbols. Therefore, in the function below, we use this symbol as
  the line break symbol. We copy a string onto an empty one until we encounter the
  line break. Then we push the string to a vector, and continue with the next line
  if there is any.

  The highlighting function iterators through the entire vector of lines, and hightlights
  each separately. 

  Perhaps a cleaner solution would be to reimplement the find() function in a 
  subclassed version of QTextEdit.
*/
QVector<QString> splitLines(QString original) 
{
  QVector<QString> result;
  QString temp = QString();
  for (QString::size_type i = 0; i != original.length(); i++) 
    {
      if (original[i] != QChar::ParagraphSeparator) 
	{
	  temp.append(original[i]);
	}
      else 
	{
	  if (temp.length() > 0) 
	    {
	      result.push_back(temp);
	    }
	  temp = "";
	}
    }
  if (temp.length() > 0) 
    {
      result.push_back(temp);
    }
  return result;
}

/* 
   I wrote this function specifically for the data widget.
   I found that text with line breaks is concatenated in the data widget table,
   which looks a bit ugly in my opinion. This function plays a part in fixing that.
*/
QString fixBreakLines(QString original) 
{
  QString newString = QString();
  for (QString::size_type i = 0; i != original.length(); i++) 
    {
      if (original[i] != '\n') 
	{
	  newString.append(original[i]);
	}
      else 
	{
	  newString.append(' ');
	}
    }
  return newString;
}


void findHeads(QSet<int> *paths,
	       QMap<int, QSet<int>> *headsMap,
	       int currentIncident) 
{
  QSet<int> heads = headsMap->value(currentIncident);
  QSetIterator<int> hIt(heads);
  while (hIt.hasNext())
    {
      int currentHead = hIt.next();
      paths->insert(currentHead);
      findHeads(paths, headsMap, currentHead);
    }
}

void findTails(QSet<int> *paths,
	       QMap<int, QSet<int>> *tailsMap,
	       int currentIncident) 
{
  QSet<int> tails = tailsMap->value(currentIncident);
  QSetIterator<int> tIt(tails);
  while (tIt.hasNext())
    {
      int currentTail = tIt.next();
      paths->insert(currentTail);
      findTails(paths, tailsMap, currentTail);
    }
}

void findBoth(QSet<int> *paths,
	      QMap<int, QSet<int>> *tailsMap,
	      QMap<int, QSet<int>> *headsMap,
	      int currentIncident,
	      QSet<int> *incidentIds)
{
  QSet<int> tails = tailsMap->value(currentIncident);
  QSetIterator<int> tIt(tails);
  QSet<int> heads = headsMap->value(currentIncident);
  QSetIterator<int> hIt(heads);
  while (tIt.hasNext())
    {
      int currentTail = tIt.next();
      if (!paths->contains(currentTail) && incidentIds->contains(currentTail))
	{
	  paths->insert(currentTail);;
	  findBoth(paths, tailsMap, headsMap, currentTail, incidentIds);
	}
    }
  while (hIt.hasNext())
    {
      int currentHead = hIt.next();
      if (!paths->contains(currentHead) && incidentIds->contains(currentHead))
	{
	  paths->insert(currentHead);
	  findBoth(paths, tailsMap, headsMap, currentHead, incidentIds);
	}
    }
}
