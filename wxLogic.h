#pragma once

#include <unordered_map>
#include <map>
#include <vector>

#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/listbox.h>
#include <wx/numdlg.h>
#include <wx/textfile.h>
#include <wx/treectrl.h>
#include <wx/wx.h>

enum class ItemStatus {
    Normal,
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
    void AddTree(wxTreeCtrl* treeCtrl);
    wxTreeItemId CreateRootItem(const wxString& str);
    wxTreeItemId AppendTreeItem(const wxTreeItemId& target, const wxString& name, const wxString& comment);
    TreeItem GetTreeItemInfo(const wxTreeItemId& item);
    void SaveTree();
    void LoadTree();
    TreeItem tokenizer(wxString str, wxString delim);


private:
    wxTreeCtrl* treeCtrl_;
    std::vector<TreeItem> id_to_wxitem_;
    std::map<wxTreeItemId, int> wxitem_to_id_;
};