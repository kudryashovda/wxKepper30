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

void TreeHolder::saveBrunch(const wxTreeItemId& head_item) {
    wxVector<sTreeItem> v;

    size_t init_id = 0;
    logic::getAllItemsData(treeCtrl_, head_item, v, init_id);

    wxTextFile tfile(filename_);
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

void TreeHolder::loadTree(const wxTreeItemId& item2connect) {

    wxVector<sTreeItem> vs;
    wxTextFile tfile(filename_);
    if (!tfile.Exists())
        return;

    tfile.Open();

    for (size_t i = 1, s = tfile.GetLineCount(); i < s; ++i) {
        sTreeItem sti = utils::s_tokenizer(tfile[i], '\t', elements_);
        if (sti == ST_ERROR)
            continue;
        vs.push_back(sti);
    }

    tfile.Close();

    if (vs.size() == 0)
        return;

    for (auto& it : vs) {
        it.itemId = treeCtrl_->AppendItem(item2connect, it.name);
        logic::setTreeItemData(treeCtrl_, it.itemId, it);
    }

    for (auto& it : vs) {      // pid
        for (auto& it2 : vs) { // id
            if (it.pid == it2.id) {
                treeCtrl_->Delete(it.itemId);
                it.itemId = treeCtrl_->AppendItem(it2.itemId, it.name);
                logic::setTreeItemData(treeCtrl_, it.itemId, it);
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

void TreeHolder::makeReport() {
    wxArrayTreeItemIds tvi;
    size_t size = treeCtrl_->GetSelections(tvi);
    if (size == 0)
        return;

    wxFileDialog* saveReportDialog = new wxFileDialog(parent_, _("Save as..."), wxEmptyString, "report.txt", "",
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
        getTextAllChilds(treeCtrl_, it, str, tab);
        tfile.AddLine(str);
        tfile.AddLine("");
    }

    tfile.Write();
    tfile.Close();

    wxMessageBox("The report was saved");
}

void TreeHolder::AppendItem(const wxArrayTreeItemIds& dest_tvi, sTreeItem st) {
    size_t maxId = logic::getMaxFid(treeCtrl_, user_name_);

    for (auto& it : dest_tvi) {
        wxTreeItemId newItem = treeCtrl_->AppendItem(it, st.name);
        st.itemId = newItem;
        st.fid = ++maxId;
        logic::setTreeItemData(treeCtrl_, newItem, st);

        treeCtrl_->Expand(it);
        treeCtrl_->SelectItem(it, false);
        treeCtrl_->SetFocusedItem(newItem);
        // logic::showTreeItemData(treeCtrl, newItem, tc, edtName);
    }

    this->saveTree();
}


void TreeHolder::saveTree() {
    this->saveBrunch(treeCtrl_->GetRootItem());
}

void showTreeItemData(const wxTreeCtrl* treeCtrl, const wxTreeItemId& item, wxTextCtrl* tc, wxTextCtrl* edtName, wxTextCtrl* edtId, wxListBox* listBox, wxString data_path, wxChar dir_separator) {
    sTreeItem sti = logic::getTreeItemData(treeCtrl, item);
    if (sti == ST_ERROR)
        return;

    wxString str = wxString::Format(wxT("%s%s%zu"), "ID: ", sti.user, sti.fid);
    edtId->SetValue(str);
    edtName->ChangeValue(sti.name);

    sti.comments.Replace("/n", "\n"); // danger - real info corrupt
    tc->ChangeValue(sti.comments);

    /* show objects attached */
    wxString itemDirPath = sti.getObjPath(data_path, dir_separator);

    listBox->Clear();
    if (!wxDirExists(itemDirPath))
        return;

    wxArrayString files;
    wxDir::GetAllFiles(itemDirPath, &files); // get full file path

    for (auto it : files) {
        wxString f = it.AfterLast(dir_separator); // show only filename
        listBox->Append(f);
    }

    if (listBox->GetCount() > 0)
        listBox->Select(0);
}

void TreeHolder::sortItems(const wxTreeItemId& head_item) {
    treeCtrl_->SortChildren(head_item);

    this->saveTree();
}

void TreeHolder::dublicate(const wxTreeItemId& item) {
    if (!item.IsOk())
        return;

    wxTreeItemId pItem = treeCtrl_->GetItemParent(item);

    sTreeItem s_item = logic::getTreeItemData(treeCtrl_, item);
    wxTreeItemId newItem = treeCtrl_->AppendItem(pItem, s_item.name);

    s_item.itemId = newItem;
    s_item.fid = logic::getMaxFid(treeCtrl_, user_name_) + 1;

    logic::setTreeItemData(treeCtrl_, newItem, s_item);

    this->saveTree();
}

} // namespace logic