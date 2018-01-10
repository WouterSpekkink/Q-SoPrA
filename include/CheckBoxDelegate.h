#ifndef CHECKBOXDELEGATE_H
#define CHECKBOXDELEGATE_H

#include <QStyledItemDelegate>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>

class CheckBoxDelegate : public QStyledItemDelegate {
  Q_OBJECT

public:
  CheckBoxDelegate(QObject *parent = 0);
  ~CheckBoxDelegate() {};

private:
  void paint (QPainter * painter, const QStyleOptionViewItem & option,
	      const QModelIndex & index ) const;

  virtual bool editorEvent(QEvent *event, QAbstractItemModel *model,
			   const QStyleOptionViewItem &option,
			   const QModelIndex &index);

  
};
 

#endif
