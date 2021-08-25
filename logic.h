#pragma once

#include "domain.h"
#include "utils.h"

namespace logic {

sTreeItem getTreeItemData(const wxTreeCtrl* treeCtrl, const wxTreeItemId& item);
void getAllItemsData(const wxTreeCtrl* treeCtrl, const wxTreeItemId& srcItem, wxVector<sTreeItem>& vs, size_t& init_id);
void saveBrunch(wxString filename, const wxTreeCtrl* treeCtrl, const wxTreeItemId& item);
size_t getMaxFid(const wxTreeCtrl* treeCtrl, wxString user);
void getTextAllChilds(const wxTreeCtrl* treeCtrl, const wxTreeItemId& rootItem, wxString& str, wxString& tab);
void makeReport(wxWindow *parent, const wxTreeCtrl* treeCtrl);
void setTreeItemData(wxTreeCtrl* treeCtrl, const wxTreeItemId& item, const sTreeItem& st);
void loadTree(wxString filename, wxTreeCtrl* treeCtrl, const wxTreeItemId& item2connect, size_t elements);

}