#pragma once

#include "wx/datetime.h"
#include <wx/treectrl.h>
#include "domain.h"

namespace utils {

bool CheckTrial(const wxString& trial_date);
void getAllItemsData(const wxTreeCtrl* treeCtrl, const wxTreeItemId& srcItem, wxVector<sTreeItem>& vs, size_t& init_id);
sTreeItem getTreeItemData(const wxTreeCtrl* treeCtrl, const wxTreeItemId& item);
void setTreeItemData(wxTreeCtrl* treeCtrl, const wxTreeItemId& item, const sTreeItem& st);


}