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

#include "../include/EventTableModel.h"

EventTableModel::EventTableModel(QWidget *parent) : QSqlTableModel(parent)
{
}

QVariant EventTableModel::data(const QModelIndex &index, int role) const 
{
  if (index.column() == 7) 
    { // This is always the column with the boolean variable
      if (role == Qt::CheckStateRole) 
	{ // Only do the below when we are setting the checkbox.
	  // We want to fetch the state of the boolean from the sql table.
	  QSqlQuery *query = new QSqlQuery;
	  int order = index.row() + 1;
	  query->prepare("SELECT mark FROM incidents WHERE ch_order = :order");
	  query->bindValue(":order", order);
	  query->exec();
	  query->first();
	  int mark = query->value(0).toInt();
	  // Return the appropriate check state based on the state of mar.
	  if (mark == 1) 
	    {
	      delete query;
	      return Qt::Checked;
	    }
	  else if (mark == 0) 
	    {
	      delete query;
	      return Qt::Unchecked;
	    }
	  delete query;
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
      // I just want the tool tip to show the data in the column.
      const QString original = QSqlTableModel::data(index, Qt::DisplayRole).toString();
      QString toolTip = breakString(original); // breakString() breaks the text in smaller lines.
      return toolTip;
      // I want to make sure that broken lines have a space between them.
    }
  else if (role == Qt::DisplayRole) 
    {
      const QString original = QSqlTableModel::data(index, Qt::DisplayRole).toString();
      QString shownText = fixBreakLines(original);
      return shownText;
    }
  else 
    {
      /* 
	 In all other cases, we want the default behaviour of this function. 
	 This can be done easily by returning the default version of the function,
	 rather than the re-implemented version we have here.
      */
      return QSqlTableModel::data(index, role);
    }
  
  return QVariant(); // This prevents a compiler warning.
}

bool EventTableModel::setData(const QModelIndex & index,
			      const QVariant & value, int role) 
{
  /* 
     Let's check whether the selected column is the column with our boolean variable
     (always column 7), and whether we are trying to set data under the 
     Qt::CheckStateRole.
  */
  if (index.column() == 7 && role == Qt::CheckStateRole) 
    {
      // Writing the data when the check box is set to checked.
      if (value == Qt::Checked) 
	{
	  QSqlQuery *query = new QSqlQuery;
	  int order = index.row() + 1;
	  query->prepare("UPDATE incidents SET mark = 1 WHERE ch_order = :order");
	  query->bindValue(":order", order);
	  query->exec();
	  delete query;
	  return true;
	  // Writing the data when the check box is set to unchecked
	}
      else if (value == Qt::Unchecked) 
	{
	  QSqlQuery *query = new QSqlQuery;
	  int order = index.row() + 1;
	  query->prepare("UPDATE incidents SET mark = 0 WHERE ch_order = :order");
	  query->bindValue(":order", order);
	  query->exec();
	  delete query;
	  return true;
	}
    }
  // In all other situations revert to default behaviour.
  return QSqlTableModel::setData(index, value, role);
}

/* 
   To make sure that the column with the pretend boolean is recognised
   as a 'checkable' column, we need to re-implement the flags() function
   and create a special case for the target column.
*/
Qt::ItemFlags EventTableModel::flags(const QModelIndex & index) const 
{
  // Column 7 always records the mark variable (our boolean).
  if (index.column() == 7) 
    {
      // Make sure that this item is checkable.
      return QSqlTableModel::flags(index) | Qt::ItemIsUserCheckable;
    }
  // Default behaviour in all other cases.
  return QSqlTableModel::flags(index);
}
