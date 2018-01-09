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
