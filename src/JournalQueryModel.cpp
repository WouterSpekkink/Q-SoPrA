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

#include "../include/JournalQueryModel.h"

JournalQueryModel::JournalQueryModel(QWidget *parent) : QSqlQueryModel(parent) 
{
}

QVariant JournalQueryModel::data(const QModelIndex &index, int role) const 
{
  int col = index.column();
  if (role == Qt::CheckStateRole)
    {
      if (this->headerData(col, Qt::Horizontal, Qt::DisplayRole) == "Needs attention")
	{
	  // Only do the below when we are setting the checkbox.
	  // We want to fetch the state of the boolean from the sql table.
	  int checked  = QSqlQueryModel::data(index).toInt();
	  if (checked) 
	    {
	      return Qt::Checked;
	    }
	  else if (!checked)
	    {
	      return Qt::Unchecked;
	    }
	}
      else 
	{
	  /*
	    We return an empty variant in all other cases. This is to prevent, for example,
	    that we also see a '0' or '1' in the same column.
	  */
	  return QVariant();
	}
      // Only do the below if we want to fetch a tool tip.
    }
  else if (role == Qt::ToolTipRole) 
    {
      if (this->headerData(col, Qt::Horizontal, Qt::DisplayRole) == "Needs attention")
	{
	  int checked  = QSqlQueryModel::data(index).toInt();
	  if (checked) 
	    {
	      return "Yes";
	    }
	  else if (!checked) 
	    {
	      return "No";
	    }
	}
      // I just want the tool tip to show the data in the column.
      const QString original = QSqlQueryModel::data(index, Qt::DisplayRole).toString();
      QString toolTip = breakString(original); // breakString() breaks the text in smaller lines.
      return toolTip;
    }
  else 
    {
      /* 
	 In all other cases, we want the default behaviour of this function. 
	 This can be done easily by returning the default version of the function,
	 rather than the re-implemented version we have here.
      */
      return QSqlQueryModel::data(index, role);
    }
  return QVariant(); // This prevents a compiler warning.
}


bool JournalQueryModel::setData(const QModelIndex & index,
				const QVariant & value, int role) 
{
  int col = index.column();
  /* 
     Let's check whether the selected column is the column with our boolean variable
     and whether we are trying to set data under the Qt::CheckStateRole.
  */
  if (role == Qt::CheckStateRole &&
      this->headerData(col, Qt::Horizontal, Qt::DisplayRole) == "Needs attention")
    {
      QString time = this->data(this->index(index.row(), 0), Qt::DisplayRole).toString();
      if (value == Qt::Checked)
	{
	  emit armSelection();
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("UPDATE journal SET mark = 1 WHERE time = :time");
	  query->bindValue(":time", time);
	  query->exec();
	  delete query;
	  setQuery("SELECT time, coder, entry, mark FROM journal");
	  emit setSelection();
	}
      else if (value == Qt::Unchecked) 
	{
	  emit armSelection();
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("UPDATE journal SET mark = 0 WHERE time = :time");
	  query->bindValue(":time", time);
	  query->exec();
	  delete query;
	  setQuery("SELECT time, coder, entry, mark FROM journal");
	  emit setSelection();
	}
    }
  // In all other situations revert to default behaviour.
  return QSqlQueryModel::setData(index, value, role);
}

/* 
   To make sure that the column with the pretend boolean is recognised
   as a 'checkable' column, we need to re-implement the flags() function
   and create a special case for the target column.
*/
Qt::ItemFlags JournalQueryModel::flags(const QModelIndex & index) const 
{
  // Column 4 always records the mark variable (our boolean).
  int col = index.column();
  if (this->headerData(col, Qt::Horizontal, Qt::DisplayRole) == "Needs attention") 
    {
      // Make sure that this item is checkable.
      return QSqlQueryModel::flags(index) | Qt::ItemIsUserCheckable;
    }
  // Default behaviour in all other cases.
  return QSqlQueryModel::flags(index);
}
