
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

#include "../include/Constants.h"

QString NEW = "New";
QString OLD = "Old";
QString DIRECTED = "Directed";
QString UNDIRECTED = "Undirected";
QString DEFAULT = "-Unselected-";
QString DEFAULT2 = "None selected";
QString UNDIRECTEDEDGE = " <--> ";
QString DIRECTEDEDGE = "--->";
QString UNDIRECTEDTAIL = "<-";
QString DIRECTEDTAIL = "--";
QString DIRECTEDHEAD = "->";
QString PAST = "Present to past";
QString FUTURE = "Past to present";
QString NODIRECTION = "No direction";
QString MANUAL = "Manual coding";
QString ASSISTED = "Assisted coding";
QString ABSTRACTACTION = "Create abstract event";
QString MASSMOVEINCIDENTNODESACTION = "Move incident and followers";
QString MASSMOVEABSTRACTNODESACTION = "Move abstract event and followers";
QString MAKEABSTRACTNODEACTION = "Abstract single event";
QString PATHS = "Paths";
QString PATHSATT = "Paths (attribute)";
QString SEMIPATHS = "Semi-paths";
QString SEMIPATHSATT = "Semi-paths (attribute)";
QString NOCONSTRAINT = "No constraint";
QString NOCONSTRAINTATT = "No constraint (attribute)";
QString DISAGGREGATEACTION = "Disaggregate event";
QString SETWIDTHACTION = "Set width";
QString RECOLOREVENTSACTION = "Recolor selected events";
QString RECOLORLABELSACTION = "Recolor selected labels";
QString COLORLINEAGEACTION = "Color lineage";
QString SEELINKAGEEVIDENCEACTION = "See linkage evidence";
QString REMOVELINKAGEACTION = "Remove linkage";
QString KEEPLINKAGEACTION = "Keep linkage";
QString ACCEPTLINKAGEACTION = "Accept linkage";
QString REJECTLINKAGEACTION = "Reject linkage";
QString ORIGINALPOSACTION = "Return to default position";
QString SETTLEACTION = "Settle in current position";
QString PARALLELACTION = "Make parallel";
QString NORMALIZEACTION = "Normalize distance";
QString CLOSEGAPACTION = "Close gap";
QString CHANGEDESCRIPTIONACTION = "Edit event description";
QString IGNOREMEACTION = "Ignore";
QString REMOVENORMALLINKAGEACTION = "Remove";
QString ENTITY = "Entity";
QString INCIDENT = "Incident";
QString ABSTRACTNODE = "Abstract Node";
QString MERGED = "Merged";
QString TRANSFORMED = "Transformed";
QString EVENTGRAPH = "Event graph";
QString NETWORKGRAPH = "Network graph";
QString OCCURRENCEGRAPH = "Occurrence graph";
QString HIDENODE = "Temporarily hide this node";
QString SETPERSISTENTACTION = "Make node persistent";
QString UNSETPERSISTENTACTION = "Reset node persistence";
QString RECOLORNODESACTION = "Recolor selected nodes";
QString RECOLORNODELABELSACTION = "Recolor labels of selected nodes";
QString ADDLINE = "Add simple line";
QString ADDSINGLEARROW = "Add single arrow";
QString ADDDOUBLEARROW = "Add double arrow";
QString ADDTEXT = "Add free text";
QString ADDELLIPSE = "Add ellipse";
QString ADDRECT = "Add rectangle";
QString TOGGLEARROW1 = "Toggle arrow one";
QString TOGGLEARROW2 = "Toggle arrow two";
QString DELETELINE = "Delete line";
QString CHANGETEXT = "Edit text";
QString ROTATETEXT = "Rotate text";
QString ROTATEELLIPSE = "Rotate ellipse";
QString ROTATERECT = "Rotate rectangle";
QString DELETETEXT = "Delete text";
QString DELETEELLIPSE = "Delete ellipse";
QString DELETERECT = "Delete rectangle";
QString DELETETIMELINE = "Delete timeline";
QString TOGGLEFIRSTTICK = "Toggle first tick";
QString FORCELASTTICK = "Toggle force last tick";
QString ADDLINKAGEACTION = "Add linkage between incidents";
QString ADDLINKAGEEVIDENCEACTION = "Add linkage and mark evidence";;
QString SELECTFOLLOWERSACTION = "Select subsequent incidents/events";
QString SELECTPREDECESSORSACTION = "Select foregoing incidents/events";;
QString SELECTALLCHILDRENACTION = "Select all children";
QString DESELECTALLCHILDRENACTION = "Deselect all children";
QString ENTITIES = "Entities";
QString AUTOASSIGNALLACTION = "Automatically assign all entities based on relationship";
QString AUTOASSIGNSPECIFICACTION = "Automatically assign this entity based on relationship";
QString UNASSIGNALLACTION = "Unassign all";
QString MERGEATTRIBUTESACTION = "Merge attribute with...";
QString COPYDESCRIPTIONTOTEXTACTION = "Copy description to text";
QString COPYOBJECT = "Duplicate object";
QString ADDRELATIONSHIPACTION = "Add relationship";
QString EDITRELATIONSHIPACTION = "Edit relationship";
QString ADDRELATIONSHIPTYPEACTION = "Add relationship type";
QString EDITRELATIONSHIPTYPEACTION = "Edit relationship type";
QString ADDATTRIBUTEACTION = "Add attribute";
QString EDITATTRIBUTEACTION = "Edit attribute";
QString ASSIGNATTRIBUTEACTION = "Assign attribute";
QString UNASSIGNATTRIBUTEACTION = "Unassign attribute";
QString ASSIGNRELATIONSHIPACTION = "Assign relationship";
QString UNASSIGNRELATIONSHIPACTION = "Unassign relationship";
QString DELETEGUIDEACTION = "Delete guide";
QString ONEFORWARD = "Move forward";
QString ONEBACKWARD = "Move backward";
QString BRINGFORWARD = "Send to front";
QString BRINGBACKWARD = "Send to back";
QString SPRINGLAYOUT = "Force-directed";
QString CIRCULARLAYOUT = "Circular";
QString MINIMISELAYOUT = "Contract current layout";
QString REDOLAYOUT = "Redo layout";
QString DATELAYOUT = "Time based layout";
QString DATELAYOUTDAYS = "Time based layout (days)";
QString DATELAYOUTMONTHS = "Time based layout (months)";
QString DATELAYOUTYEARS = "Time based layout (years)";
QString DATELAYOUTDECADES = "Time based layout (decades)";
QString NOOVERLAP = "No overlap";
QString REDUCEOVERLAP = "Reduce overlap";
QString MATCHEVENTGRAPH = "Match event graph";
QString UNRELATEDMODE = "Unrelated";
QString ORIGINMODE = "Origin";
QString ANCESTORMODE = "Ancestor";
QString DESCENDANTMODE = "Descendant";
QString ADDTOCASEACTION = "Add to existing case";
QString REMOVEFROMCASEACTION = "Remove from case";
QString CREATENEWCASEACTION = "Add to new case";
QString SETHIGHLIGHTEDACTION = "Highlight occurrences";
QString UNSETHIGHLIGHTEDACTION = "Remove highlight";

qreal Pi = 3.14;
