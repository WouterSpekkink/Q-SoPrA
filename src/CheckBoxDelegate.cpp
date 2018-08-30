/* 
   This is a slightly adapted version of the example that is provided here:
   https://wiki.qt.io/Technical_FAQ#How_can_I_align_the_checkboxes_in_a_view.3F
*/

#include "../include/CheckBoxDelegate.h"

CheckBoxDelegate::CheckBoxDelegate(QObject *parent) : QStyledItemDelegate(parent) 
{
}

void CheckBoxDelegate::paint (QPainter * painter, const QStyleOptionViewItem & option,
			      const QModelIndex & index ) const 
{
  QStyleOptionViewItem viewItemOption(option);
  // This basically changes the rectangle in which the check box is drawn.
  const int textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
  QRect newRect = QStyle::alignedRect(option.direction, Qt::AlignCenter,
				      QSize(option.decorationSize.width() +
					    5,option.decorationSize.height()),
				      QRect(option.rect.x() + textMargin, option.rect.y(),
					    option.rect.width() -
					    (2 * textMargin), option.rect.height()));
  viewItemOption.rect = newRect;
  // Draw the check box using the new rectangle.
  QStyledItemDelegate::paint(painter, viewItemOption, index);
}

bool CheckBoxDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
				   const QStyleOptionViewItem &option,
				   const QModelIndex &index) 
{
  Q_ASSERT(event);
  Q_ASSERT(model);

  // make sure that the item is checkable
  Qt::ItemFlags flags = model->flags(index);
  if (!(flags & Qt::ItemIsUserCheckable) || !(flags & Qt::ItemIsEnabled)) 
    {
      return false;
    }

  // make sure that we have a check state
  QVariant value = index.data(Qt::CheckStateRole);
  if (!value.isValid()) 
    {
      return false;
    }

  // make sure that we have the right event type
  if (event->type() == QEvent::MouseButtonRelease) 
    {
      const int textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
      QRect checkRect = QStyle::alignedRect(option.direction, Qt::AlignCenter,
					    option.decorationSize,
					    QRect(option.rect.x() + (2 * textMargin), option.rect.y(),
						  option.rect.width() - (2 * textMargin),
						  option.rect.height()));
      // If the event did not take place in the rectangle that contains our check box
      QMouseEvent *mEvent = (QMouseEvent*) event;
      if (!checkRect.contains(mEvent->pos())) 
	{
	  return false;
	}
      // 
    }
  else if (event->type() == QEvent::KeyPress) 
    {
      if ((static_cast<QKeyEvent*>(event)->key() !=
	   Qt::Key_Space&& static_cast<QKeyEvent*>(event)->key() != Qt::Key_Select) &&
	  (static_cast<QKeyEvent*>(event)->key() !=
	   Qt::Key_Space&& static_cast<QKeyEvent*>(event)->key() != Qt::Key_Space)
	  ) 
	{
	  return false;
	}
    }
  else 
    {
      return false;
    }
  // Determine the new check state
  Qt::CheckState state = (static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked
			  ? Qt::Unchecked : Qt::Checked);
  // And set the new check state by calling the model's setData() function.
  return model->setData(index, state, Qt::CheckStateRole);
}
