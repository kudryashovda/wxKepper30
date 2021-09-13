#pragma once

#include <fstream>
#include <map>
#include <unordered_map>
#include <vector>

#include <filesystem>

#include <algorithm>
#include <wx/numdlg.h>
#include <wx/treectrl.h>
#include <wx/wx.h>

#include "utils.h"

namespace fs = std::filesystem;

enum class ItemStatus {
    Normal,
    Archived,
    Deleted
};

struct TreeItem {
    int id;
    int parent_id;
    wxTreeItemId wxitem;
    wxString name;
    wxString comment;
    ItemStatus status;
};

class wxLogic {
public:
    void SetTree(wxTreeCtrl* treeCtrl);
    wxTreeItemId AppendTreeItem(const wxTreeItemId& target, const wxString& name, const wxString& comment);
    void UpdateTreeItem(const wxTreeItemId& target, const wxString& name, const wxString& comment);

    const TreeItem& GetTreeItemInfo(const wxTreeItemId& item);
    void SaveTree();
    void LoadTree();
    wxVector<wxString> tokenizer(wxString str, wxString delim);
    wxTreeItemId GetParentTreeItemPtrById(int item_id);
    wxTreeItemId CreateNewTreeItem(wxTreeItemId parent_ptr, const wxString& name, int item_id);
    int DeleteItem(wxTreeItemId item_ptr);
    bool ItemHasChild(wxTreeItemId item_ptr);

private:
    const fs::path workdir_ = fs::current_path();
    const fs::path db_path_ = workdir_ / "data" / "db.dat";
    const fs::path db_workdir_ = db_path_.parent_path();

    wxTreeCtrl* treeCtrl_;
    std::vector<int> ids_;
    std::map<int, TreeItem> id_to_info_;
    std::map<wxTreeItemId, int> wxitem_to_id_;
};