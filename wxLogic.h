#pragma once

#include <fstream>
#include <sstream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <experimental/filesystem>

#include <algorithm>
#include <wx/numdlg.h>
#include <wx/treectrl.h>
#include <wx/wx.h>
#include <codecvt>

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
    std::wstring name;
    std::wstring comment;
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

    wxTreeItemId GetParentTreeItemPtrById(long item_id);
    wxTreeItemId CreateNewTreeItem(wxTreeItemId parent_ptr, const wxString& name, long item_id);
    int DeleteItem(wxTreeItemId item_ptr);
    bool ItemHasChild(wxTreeItemId item_ptr);
    bool IsItemIdExists(long item_id);
    void CreateFile(wxTreeItemId item_ptr, const std::wstring& filename);
    void CopyFiles(wxTreeItemId item_ptr, const std::vector<fs::path>& paths);
    fs::path GetItemPath(const TreeItem& info);
    fs::path GetItemPath(wxTreeItemId item_ptr);

private:
    const fs::path workdir_ = fs::current_path();
    const fs::path db_path_ = workdir_ / L"data" / L"db.dat";
    const fs::path db_workdir_ = db_path_.parent_path();
    const fs::path files_workdir_ = db_path_.parent_path();

    wxTreeCtrl* treeCtrl_;
    std::vector<long> ids_;
    std::map<long, TreeItem> id_to_info_;
    std::map<wxTreeItemId, long> wxitem_to_id_;
};