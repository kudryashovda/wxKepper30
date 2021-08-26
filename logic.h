#pragma once

#include "custom_dialogs.h"
#include "domain.h"
#include "utils.h"

namespace logic {

class TreeHolder {
public:
    TreeHolder(wxWindow* parent, wxTreeCtrl* treeCtrl, wxString user_name)
        : parent_(parent)
        , treeCtrl_(treeCtrl)
        , user_name_(user_name) {
        dir_separator_ = wxFileName::GetPathSeparator();
    }

    void SetFileName(wxString data_path, wxString filename, size_t elements) {
        data_path_ = data_path;
        filename_ = filename;
        elements_ = elements;
    }

    void AppendItem(const wxArrayTreeItemIds& dest_tvi, sTreeItem st);


    void makeReport();
    void saveBrunch(const wxTreeItemId& head_item);
    void saveTree();
    void loadTree(const wxTreeItemId& item2connect);
    void dublicate(const wxTreeItemId& item);
    void sortItems(const wxTreeItemId& rootItem);


private:
    wxWindow* parent_;
    wxTreeCtrl* treeCtrl_;
    wxString user_name_;

    wxString data_path_;
    wxString filename_;
    size_t elements_ = 0;

    wxChar dir_separator_;
};

sTreeItem getTreeItemData(const wxTreeCtrl* treeCtrl, const wxTreeItemId& item);
void getAllItemsData(const wxTreeCtrl* treeCtrl, const wxTreeItemId& srcItem, wxVector<sTreeItem>& vs, size_t& init_id);
size_t getMaxFid(const wxTreeCtrl* treeCtrl, wxString user);
void getTextAllChilds(const wxTreeCtrl* treeCtrl, const wxTreeItemId& rootItem, wxString& str, wxString& tab);
void makeReport(wxWindow* parent, const wxTreeCtrl* treeCtrl);
void setTreeItemData(wxTreeCtrl* treeCtrl, const wxTreeItemId& item, const sTreeItem& st);

void showTreeItemData(const wxTreeCtrl* treeCtrl, const wxTreeItemId& item, wxTextCtrl* tc, wxTextCtrl* edtName, wxTextCtrl* edtId, wxListBox* listBox, wxString data_path, wxChar dir_separator);
} // namespace logic