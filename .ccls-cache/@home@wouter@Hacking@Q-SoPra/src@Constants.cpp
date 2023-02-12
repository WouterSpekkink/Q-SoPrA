
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

const QString NEW = "New";
const QString OLD = "Old";
const QString DIRECTED = "Directed";
const QString UNDIRECTED = "Undirected";
const QString DEFAULT = "-Unselected-";
const QString DEFAULT2 = "None selected";
const QString UNDIRECTEDEDGE = " <--> ";
const QString DIRECTEDEDGE = "--->";
const QString UNDIRECTEDTAIL = "<-";
const QString DIRECTEDTAIL = "--";
const QString DIRECTEDHEAD = "->";
const QString PAST = "Present to past";
const QString FUTURE = "Past to present";
const QString NODIRECTION = "No direction";
const QString MANUAL = "Manual coding";
const QString ASSISTED = "Assisted coding";
const QString ABSTRACTACTION = "Create abstract event";
const QString MASSMOVEINCIDENTNODESACTION = "Move incident and followers";
const QString MASSMOVEABSTRACTNODESACTION = "Move abstract event and followers";
const QString MAKEABSTRACTNODEACTION = "Abstract single event";
const QString PATHS = "Paths";
const QString PATHSATT = "Paths (attribute)";
const QString SEMIPATHS = "Semi-paths";
const QString NOCONSTRAINT = "No constraint";
const QString COMPOSITEEVENT = "Composite event";
const QString DISAGGREGATEACTION = "Disaggregate event";
const QString SETWIDTHACTION = "Set width";
const QString RECOLOREVENTSACTION = "Recolor selected events";
const QString RECOLORLABELSACTION = "Recolor selected labels";
const QString COLORLINEAGEACTION = "Color lineage";
const QString SEELINKAGEEVIDENCEACTION = "See linkage evidence";
const QString REMOVELINKAGEACTION = "Remove linkage";
const QString KEEPLINKAGEACTION = "Keep linkage";
const QString ACCEPTLINKAGEACTION = "Accept linkage";
const QString REJECTLINKAGEACTION = "Reject linkage";
const QString ORIGINALPOSACTION = "Return to default position";
const QString SETTLEACTION = "Settle in current position";
const QString PARALLELACTION = "Make parallel";
const QString NORMALIZEACTION = "Normalize distance";
const QString CLOSEGAPACTION = "Close gap";
const QString CHANGEDESCRIPTIONACTION = "Edit event description";
const QString IGNOREMEACTION = "Ignore";
const QString REMOVENORMALLINKAGEACTION = "Remove";
const QString ENTITY = "Entity";
const QString INCIDENT = "Incident";
const QString ABSTRACTNODE = "Abstract Node";
const QString MERGED = "Merged";
const QString TRANSFORMED = "Transformed";
const QString EVENTGRAPH = "Event graph";
const QString NETWORKGRAPH = "Network graph";
const QString OCCURRENCEGRAPH = "Occurrence graph";
const QString HIDENODE = "Temporarily hide this node";
const QString SETPERSISTENTACTION = "Make node persistent";
const QString UNSETPERSISTENTACTION = "Reset node persistence";
const QString RECOLORNODESACTION = "Recolor selected nodes";
const QString RECOLORNODELABELSACTION = "Recolor labels of selected nodes";
const QString ADDLINE = "Add simple line";
const QString ADDSINGLEARROW = "Add single arrow";
const QString ADDDOUBLEARROW = "Add double arrow";
const QString ADDTEXT = "Add free text";
const QString ADDELLIPSE = "Add ellipse";
const QString ADDRECT = "Add rectangle";
const QString TOGGLEARROW1 = "Toggle arrow one";
const QString TOGGLEARROW2 = "Toggle arrow two";
const QString DELETELINE = "Delete line";
const QString CHANGETEXT = "Edit text";
const QString ROTATETEXT = "Rotate text";
const QString ROTATEELLIPSE = "Rotate ellipse";
const QString ROTATERECT = "Rotate rectangle";
const QString DELETETEXT = "Delete text";
const QString DELETEELLIPSE = "Delete ellipse";
const QString DELETERECT = "Delete rectangle";
const QString DELETETIMELINE = "Delete timeline";
const QString TOGGLEFIRSTTICK = "Toggle first tick";
const QString FORCELASTTICK = "Toggle force last tick";
const QString ADDLINKAGEACTION = "Add linkage between incidents";
const QString ADDLINKAGEEVIDENCEACTION = "Add linkage and mark evidence";;
const QString SELECTFOLLOWERSACTION = "Select subsequent incidents/events";
const QString SELECTPREDECESSORSACTION = "Select foregoing incidents/events";;
const QString SELECTALLCHILDRENACTION = "Select all children";
const QString DESELECTALLCHILDRENACTION = "Deselect all children";
const QString ENTITIES = "Entities";
const QString AUTOASSIGNALLACTION = "Automatically assign all entities based on relationship";
const QString AUTOASSIGNSPECIFICACTION = "Automatically assign this entity based on relationship";
const QString UNASSIGNALLACTION = "Unassign all";
const QString MERGEATTRIBUTESACTION = "Merge attribute with...";
const QString COPYDESCRIPTIONTOTEXTACTION = "Copy description to text";
const QString COPYOBJECT = "Duplicate object";
const QString ADDRELATIONSHIPACTION = "Add relationship";
const QString EDITRELATIONSHIPACTION = "Edit relationship";
const QString ADDRELATIONSHIPTYPEACTION = "Add relationship type";
const QString EDITRELATIONSHIPTYPEACTION = "Edit relationship type";
const QString ADDATTRIBUTEACTION = "Add attribute";
const QString EDITATTRIBUTEACTION = "Edit attribute";
const QString ASSIGNATTRIBUTEACTION = "Assign attribute";
const QString UNASSIGNATTRIBUTEACTION = "Unassign attribute";
const QString ASSIGNRELATIONSHIPACTION = "Assign relationship";
const QString UNASSIGNRELATIONSHIPACTION = "Unassign relationship";
const QString DELETEGUIDEACTION = "Delete guide";
const QString ONEFORWARD = "Move forward";
const QString ONEBACKWARD = "Move backward";
const QString BRINGFORWARD = "Send to front";
const QString BRINGBACKWARD = "Send to back";
const QString SPRINGLAYOUT = "Force-directed";
const QString CIRCULARLAYOUT = "Circular";
const QString MINIMISELAYOUT = "Contract current layout";
const QString REDOLAYOUT = "Redo layout";
const QString DATELAYOUT = "Time based layout";
const QString DATELAYOUTDAYS = "Time based layout (days)";
const QString DATELAYOUTMONTHS = "Time based layout (months)";
const QString DATELAYOUTYEARS = "Time based layout (years)";
const QString DATELAYOUTDECADES = "Time based layout (decades)";
const QString NOOVERLAP = "No overlap";
const QString REDUCEOVERLAP = "Reduce overlap";
const QString MATCHEVENTGRAPH = "Match event graph";
const QString UNRELATEDMODE = "Unrelated";
const QString ORIGINMODE = "Origin";
const QString ANCESTORMODE = "Ancestor";
const QString DESCENDANTMODE = "Descendant";
const QString ADDTOCASEACTION = "Add to existing case";
const QString REMOVEFROMCASEACTION = "Remove from case";
const QString CREATENEWCASEACTION = "Add to new case";
const QString SETHIGHLIGHTEDACTION = "Highlight occurrences";
const QString UNSETHIGHLIGHTEDACTION = "Remove highlight";
const QString SELECTTAILACTION = "Set as tail incident";
const QString SELECTHEADACTION = "Set as head incident";
const QString OUTCOME = "OUTCOME";

const qreal Pi = 3.14;
