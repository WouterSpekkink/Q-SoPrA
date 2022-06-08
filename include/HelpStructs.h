#ifndef HELPSTRUCTS_H
#define HELPSTRUCTS_H

struct find_by_id
{
  find_by_id(const int & id) : id(id) {}
  bool operator()(IncidentNode* incident)
  {
    return incident->getId() == id;
  }
  private:
    int id;
};

struct find_by_incident_nodes
{
  find_by_incident_nodes(const QPair<int, int> &pair) : pair(pair) {}
  bool operator()(Linkage* linkage)
  {
    IncidentNode *source = qgraphicsitem_cast<IncidentNode*>(linkage->getStart());
    IncidentNode *target = qgraphicsitem_cast<IncidentNode*>(linkage->getEnd());
    if (source->getId() == pair.first &&
        target->getId() == pair.second)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  private:
    QPair<int, int> pair;
};

#endif // HELPSTRUCTS_H
