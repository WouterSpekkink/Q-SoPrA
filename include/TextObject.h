#ifndef TEXTOBJECT_H
#define TEXTOBJECT_H

#include <QGraphicsTextItem>

class TextObject : public QGraphicsTextItem {

public:
  TextObject(const QString &text = QString(),
	     QGraphicsTextItem *parent = 0);
  ~TextObject() {};
 
};

#endif
