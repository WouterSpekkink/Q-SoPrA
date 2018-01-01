#include "../include/SupportingFunctions.h"

QString breakString(QString original) {
  QString result = "";
  if (original.length() <= 50) {
    result = original;
  } else {
    QString subString = "";
    for (QString::size_type it = 0; it != original.length(); it++) {
      subString.append(original[it]);
      if (subString.length() >= 50) {
	if (it != original.length() - 1) {
	  if (original[it + 1] == ' ') {
	    subString.append("\n");
	    result += subString;
	    subString = "";
	  }
	}
      }
    }
    result += subString;
  }
  return result;
}
