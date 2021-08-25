#include "utils.h"
namespace utils {

bool CheckTrial(const wxString& trial_date) {
    wxDateTime dt_deadline;
    dt_deadline.ParseDateTime(trial_date);

    wxDateTime now = wxDateTime::Now();

    if (now < dt_deadline)
        return false;
    else
        return true;
}

sTreeItem getTreeItemData(const wxTreeCtrl* treeCtrl, const wxTreeItemId& item) {
    MyTreeItemData* data = (MyTreeItemData*)treeCtrl->GetItemData(item);

    if (data == nullptr)
        return ST_ERROR;

    return data->GetStruct();
}

void setTreeItemData(wxTreeCtrl* treeCtrl, const wxTreeItemId& item, const sTreeItem& st) {
    treeCtrl->SetItemData(item, new MyTreeItemData(st));
    treeCtrl->SetItemText(item, st.name);
}

void getAllItemsData(const wxTreeCtrl* treeCtrl,  const wxTreeItemId& srcItem, wxVector<sTreeItem>& vs, size_t& init_id) {
    wxTreeItemIdValue cookie;

    sTreeItem ss;
    ss = getTreeItemData(treeCtrl, srcItem);

    ss.id = vs.size();
    ss.pid = init_id;

    vs.push_back(ss);

    wxTreeItemId srcChild = treeCtrl->GetFirstChild(srcItem, cookie);
    while (srcChild.IsOk()) {
        getAllItemsData(treeCtrl, srcChild, vs, ss.id); // danger - recursia
        srcChild = treeCtrl->GetNextChild(srcItem, cookie);
    }
}

} // namespace utils