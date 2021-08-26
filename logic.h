#pragma once

#include "domain.h"
#include "utils.h"
#include <wx/dir.h>
#include <wx/filename.h>

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
    size_t getMaxFid();

    void makeReport();
    void saveBrunch(const wxTreeItemId& head_item);
    void saveTree();
    void loadTree(const wxTreeItemId& item2connect);
    void dublicate(const wxTreeItemId& item);
    void sortItems(const wxTreeItemId& rootItem);
    void setTreeItemData(const wxTreeItemId& item, const sTreeItem& st);
    sTreeItem getTreeItemData(const wxTreeItemId& item);
    void getAllItemsData(const wxTreeItemId& srcItem, wxVector<sTreeItem>& vs, size_t& init_id);
    void showTreeItemData(const wxTreeItemId& item, wxTextCtrl* tc, wxTextCtrl* edtName, wxTextCtrl* edtId, wxListBox* listBox);
    void moveAllChilds(const wxTreeItemId& srcItem, const wxTreeItemId& destItem);
private:
    wxWindow* parent_;
    wxTreeCtrl* treeCtrl_;
    wxString user_name_;

    wxString data_path_;
    wxString filename_;
    size_t elements_ = 0;

    wxChar dir_separator_;
};

} // namespace logic