#include "wxLogic.h"

void wxLogic::AddTree(wxTreeCtrl* treeCtrl) {
    treeCtrl_ = treeCtrl;
}

wxTreeItemId wxLogic::CreateRootItem(const wxString& str) {
    auto root_item = treeCtrl_->AddRoot(str);

    id_to_wxitem_.push_back({ 0, 0, root_item, str, "" });
    wxitem_to_id_[root_item] = 0;

    return root_item;
}

wxTreeItemId wxLogic::AppendTreeItem(const wxTreeItemId& target, const wxString& name, const wxString& comment) {
    size_t target_item_id = wxitem_to_id_.at(target);

    auto new_item = treeCtrl_->AppendItem(target, name);
    size_t new_item_id = id_to_wxitem_.size();

    id_to_wxitem_.push_back({ new_item_id, target_item_id, new_item, name, comment });
    wxitem_to_id_[new_item] = new_item_id;

    return new_item;
}

TreeItem wxLogic::GetTreeItemInfo(const wxTreeItemId& item) {
    auto item_id = wxitem_to_id_.at(item);

    return id_to_wxitem_.at(item_id);
}
