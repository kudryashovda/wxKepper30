#pragma once

#include <fstream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <experimental/filesystem>

#include <algorithm>
#include <wx/numdlg.h>
#include <wx/treectrl.h>
#include <wx/wx.h>

#include "utils.h"

namespace fs = std::experimental::filesystem;

enum class ItemStatus {
    Normal,
    Archived,
    Deleted
};

struct TreeItem {
    long id;
    long parent_id;
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
    const TreeItem& GetTreeItemInfo(long item_id);
    void SaveTree();
    void LoadTree();
    static wxVector<wxString> tokenizer(wxString str, const wxString& delim);
    wxTreeItemId GetParentTreeItemPtrById(long item_id);
    wxTreeItemId CreateNewTreeItem(wxTreeItemId parent_ptr, const wxString& name, long item_id);
    int DeleteItem(wxTreeItemId item_ptr);
    bool ItemHasChild(wxTreeItemId item_ptr);
    bool IsItemIdExists(long item_id);
    void CreateFile(wxTreeItemId item_ptr, const std::string& filename);
    fs::path GetItemPath(const TreeItem& info);
    fs::path GetItemPath(wxTreeItemId item_ptr);
    static bool IsValidFilename(const std::string& filename);

private:
    const fs::path workdir_ = fs::current_path();
    const fs::path db_path_ = workdir_ / "data" / "db.dat";
    const fs::path db_workdir_ = db_path_.parent_path();
    const fs::path files_workdir_ = db_path_.parent_path();

    wxTreeCtrl* treeCtrl_;
    std::vector<long> ids_;
    std::map<long, TreeItem> id_to_info_;
    std::map<wxTreeItemId, long> wxitem_to_id_;
};