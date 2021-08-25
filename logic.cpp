#include "logic.h"

namespace logic {

sTreeItem getTreeItemData(const wxTreeCtrl* treeCtrl, const wxTreeItemId& item) {
    MyTreeItemData* data = (MyTreeItemData*)treeCtrl->GetItemData(item);

    if (data == nullptr)
        return ST_ERROR;

    return data->GetStruct();
}

void getAllItemsData(const wxTreeCtrl* treeCtrl, const wxTreeItemId& srcItem, wxVector<sTreeItem>& vs, size_t& init_id) {
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

/* Danger! Recursia */
void getTextAllChilds(const wxTreeCtrl* treeCtrl, const wxTreeItemId& rootItem, wxString& str, wxString& tab) {
    wxTreeItemIdValue cookie;
    sTreeItem s = logic::getTreeItemData(treeCtrl, rootItem);

    // wxString comments = s.comments;
    s.comments.Replace("/n", '\n' + tab + '\t');
    // s.comments.Prepend(tab);

    // str += tab + "'--" + s.name + "(id=" + std::to_string(s.id) +  ')' + '\n' + tab + s.comments + '\n';
    str += tab + "|--" + s.name + "(id=" + std::to_string(s.id) + ')' + '\t' + s.comments + '\n';

    wxTreeItemId rootChild = treeCtrl->GetFirstChild(rootItem, cookie);
    while (rootChild.IsOk()) {
        tab += '\t';
        getTextAllChilds(treeCtrl, rootChild, str, tab); // danger - recursia
        rootChild = treeCtrl->GetNextChild(rootItem, cookie);
    }
    tab.RemoveLast();
}

size_t getMaxFid(const wxTreeCtrl* treeCtrl, wxString user) {
    size_t maxFid = 0;
    size_t init_id = 0;

    wxVector<sTreeItem> v;
    logic::getAllItemsData(treeCtrl, treeCtrl->GetRootItem(), v, init_id);

    for (auto it : v)
        if (it.fid > maxFid && it.user == user)
            maxFid = it.fid;

    return maxFid;
}

void saveBrunch(wxString filename, const wxTreeCtrl* treeCtrl, const wxTreeItemId& item) {
    wxVector<sTreeItem> v;

    size_t init_id = 0;
    logic::getAllItemsData(treeCtrl, item, v, init_id);

    wxTextFile tfile(filename);
    if (not tfile.Exists())
        tfile.Create();
    tfile.Open();
    tfile.Clear();

    for (auto it : v) {
        it.name.ToUTF8();
        it.name.Replace('\t', " ");

        it.comments.ToUTF8();
        it.comments.Replace('\t', " ");
        it.comments.Replace('\n', "/n"); // danger - real info may corrupt

        wxString str = wxString::Format(wxT("%zu\t%zu\t%s\t%zu\t%s\t%s"), it.id, it.pid, it.user, it.fid, it.name, it.comments);
        tfile.AddLine(str);
    }

    tfile.Write();
    tfile.Close();
}

void setTreeItemData(wxTreeCtrl* treeCtrl, const wxTreeItemId& item, const sTreeItem& st) {
    treeCtrl->SetItemData(item, new MyTreeItemData(st));
    treeCtrl->SetItemText(item, st.name);
}

void loadTree(wxString filename, wxTreeCtrl* treeCtrl, const wxTreeItemId& item2connect, size_t elements) {

    wxVector<sTreeItem> vs;
    wxTextFile tfile(filename);
    if (!tfile.Exists())
        return;

    tfile.Open();

    for (size_t i = 1, s = tfile.GetLineCount(); i < s; ++i) {
        sTreeItem sti = utils::s_tokenizer(tfile[i], '\t', elements);
        if (sti == ST_ERROR)
            continue;
        vs.push_back(sti);
    }

    tfile.Close();

    if (vs.size() == 0)
        return;

    for (auto& it : vs) {
        it.itemId = treeCtrl->AppendItem(item2connect, it.name);
        logic::setTreeItemData(treeCtrl, it.itemId, it);
    }

    for (auto& it : vs) {      // pid
        for (auto& it2 : vs) { // id
            if (it.pid == it2.id) {
                treeCtrl->Delete(it.itemId);
                it.itemId = treeCtrl->AppendItem(it2.itemId, it.name);
                logic::setTreeItemData(treeCtrl, it.itemId, it);
            }
        }
    }
}

void makeReport(wxWindow* parent, const wxTreeCtrl* treeCtrl) {
    wxArrayTreeItemIds tvi;
    size_t size = treeCtrl->GetSelections(tvi);
    if (size == 0)
        return;

    wxFileDialog* saveReportDialog = new wxFileDialog(parent, _("Save as..."), wxEmptyString, "report.txt", "",
                                                      wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);

    if (saveReportDialog->ShowModal() != wxID_OK)
        return;

    wxString fn = saveReportDialog->GetPath();

    wxTextFile tfile(fn); // add date
    if (not tfile.Exists())
        tfile.Create();
    tfile.Open();
    tfile.Clear();

    tfile.AddLine("Report"); // add date
    tfile.AddLine("");

    for (auto it : tvi) {
        wxString str = "";
        wxString tab = "";
        getTextAllChilds(treeCtrl, it, str, tab);
        tfile.AddLine(str);
        tfile.AddLine("");
    }

    tfile.Write();
    tfile.Close();

    wxMessageBox("The report was saved");
}

} // namespace logic