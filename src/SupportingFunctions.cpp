#include "../include/SupportingFunctions.h"

/*
  This function supports the creation of tool tips. It wraps longer fragments
  of text by introducing newline symbols after a certain length.
  It does keep words intact.
*/
QString breakString(QString original) {
  QString result = "";
  if (original.length() <= 50) {
    result = original;
  } else {
    QString subString = "";
    int count = 0;
    for (QString::size_type it = 0; it != original.length(); it++) {
      subString.append(original[it]);
      count++;
      if (count >= 50 || original[it] == '\n') {
	if (it != original.length() - 1) {
	  if (original[it + 1] == '\n') {
	    result += subString;
	    subString = "";
	    count = 0;
	  } else if (original[it + 1] == ' ') {
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
QString doubleQuote(QString original) {
  QString copy = original;
  for (QString::size_type i = 0; i != copy.length(); i++) {
    if (copy[i] == '"') {
      copy[i] = '\"';
    }
  }
  return copy;
}

/*
  I encountered an annoying problem where Qt always returns texts from
  QCheckBoxes with ampersands, even when I did not assign one. I therefore 
  wrote this function that ensures that the ampersand is removed whenever one is present.
*/
QString removeChar(QString original, QChar chara) {
  QString newString = QString();
  for (QString::size_type i = 0; i != original.length(); i++) {
    if (original[i] != chara) {
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
QVector<QString> splitLines(QString original) {
  QVector<QString> result;
  QString temp = QString();
  for (QString::size_type i = 0; i != original.length(); i++) {
    if (original[i] != QChar::ParagraphSeparator) {
      temp.append(original[i]);
    } else {
      if (temp.length() > 0) {
	result.push_back(temp);
      }
      temp = "";
    }
  }
  if (temp.length() > 0) {
    result.push_back(temp);
  }
  return result;
}

/* 
   I wrote this function specifically for the data widget.
   I found that text with line breaks is concatenated in the data widget table,
   which looks a bit ugly in my opinion. This function plays a part in fixing that.
*/
QString fixBreakLines(QString original) {
  QString newString = QString();
  for (QString::size_type i = 0; i != original.length(); i++) {
    if (original[i] != '\n') {
      newString.append(original[i]);
    } else {
      newString.append(' ');
    }
  }
  return newString;
}
