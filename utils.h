#pragma once

#include "wx/datetime.h"
#include <wx/treectrl.h>
#include "domain.h"
#include <wx/textfile.h>


namespace utils {

Settings getSettingsFromFile(wxString fn, wxChar dir_separator);
bool CheckTrial(const wxString& trial_date);
void getAllItemsData(const wxTreeCtrl* treeCtrl, const wxTreeItemId& srcItem, wxVector<sTreeItem>& vs, size_t& init_id);
sTreeItem getTreeItemData(const wxTreeCtrl* treeCtrl, const wxTreeItemId& item);
void setTreeItemData(wxTreeCtrl* treeCtrl, const wxTreeItemId& item, const sTreeItem& st);
void saveBrunch(wxString filename, const wxTreeCtrl* treeCtrl, const wxTreeItemId& item);
sTreeItem s_tokenizer(wxString str, wxString delim, size_t elements);
void loadTree(wxString filename, wxTreeCtrl* treeCtrl, const wxTreeItemId& item2connect, size_t elements);

}