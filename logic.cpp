#include "logic.h"

namespace logic {

sTreeItem TreeHolder::getTreeItemData(const wxTreeItemId& item) {
    MyTreeItemData* data = (MyTreeItemData*)treeCtrl_->GetItemData(item);

    if (data == nullptr)
        return ST_ERROR;

    return data->GetStruct();
}

void TreeHolder::getAllItemsData(const wxTreeItemId& srcItem, wxVector<sTreeItem>& vs, size_t& init_id) {
    wxTreeItemIdValue cookie;

    sTreeItem ss;
    ss = this->getTreeItemData(srcItem);

    ss.id = vs.size();
    ss.pid = init_id;

    vs.push_back(ss);

    wxTreeItemId srcChild = treeCtrl_->GetFirstChild(srcItem, cookie);
    while (srcChild.IsOk()) {
        this->getAllItemsData(srcChild, vs, ss.id); // danger - recursia
        srcChild = treeCtrl_->GetNextChild(srcItem, cookie);
    }
}

size_t TreeHolder::getMaxFid() {
    size_t maxFid = 0;
    size_t init_id = 0;

    wxVector<sTreeItem> v;
    this->getAllItemsData(treeCtrl_->GetRootItem(), v, init_id);

    for (auto it : v)
        if (it.fid > maxFid && it.user == user_name_)
            maxFid = it.fid;

    return maxFid;
}

void TreeHolder::saveBrunch(const wxTreeItemId& head_item) {
    wxVector<sTreeItem> v;

    size_t init_id = 0;
    this->getAllItemsData(head_item, v, init_id);

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

void TreeHolder::setTreeItemData(const wxTreeItemId& item, const sTreeItem& st) {
    treeCtrl_->SetItemData(item, new MyTreeItemData(st));
    treeCtrl_->SetItemText(item, st.name);
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
        this->setTreeItemData(it.itemId, it);
    }

    for (auto& it : vs) {      // pid
        for (auto& it2 : vs) { // id
            if (it.pid == it2.id) {
                treeCtrl_->Delete(it.itemId);
                it.itemId = treeCtrl_->AppendItem(it2.itemId, it.name);
                this->setTreeItemData(it.itemId, it);
            }
        }
    }
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
        // getTextAllChilds(treeCtrl_, it, str, tab);
        tfile.AddLine(str);
        tfile.AddLine("");
    }

    tfile.Write();
    tfile.Close();

    wxMessageBox("The report was saved");
}

void TreeHolder::AppendItem(const wxArrayTreeItemIds& dest_tvi, sTreeItem st) {
    size_t maxId = this->getMaxFid();

    for (auto& it : dest_tvi) {
        wxTreeItemId newItem = treeCtrl_->AppendItem(it, st.name);
        st.itemId = newItem;
        st.fid = ++maxId;
        this->setTreeItemData(newItem, st);

        treeCtrl_->Expand(it);
        treeCtrl_->SelectItem(it, false);
        treeCtrl_->SetFocusedItem(newItem);
    }

    this->saveTree();
}

void TreeHolder::saveTree() {
    this->saveBrunch(treeCtrl_->GetRootItem());
}

void TreeHolder::showTreeItemData(const wxTreeItemId& item, wxTextCtrl* tc, wxTextCtrl* edtName, wxTextCtrl* edtId, wxListBox* listBox) {
    sTreeItem sti = this->getTreeItemData(item);
    if (sti == ST_ERROR)
        return;

    wxString str = wxString::Format(wxT("%s%s%zu"), "ID: ", sti.user, sti.fid);
    edtId->SetValue(str);
    edtName->ChangeValue(sti.name);

    sti.comments.Replace("/n", "\n"); // danger - real info corrupt
    tc->ChangeValue(sti.comments);

    /* show objects attached */
    wxString itemDirPath = sti.getObjPath(data_path_, dir_separator_);

    listBox->Clear();
    if (!wxDirExists(itemDirPath))
        return;

    wxArrayString files;
    wxDir::GetAllFiles(itemDirPath, &files); // get full file path

    for (auto it : files) {
        wxString f = it.AfterLast(dir_separator_); // show only filename
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

    sTreeItem s_item = this->getTreeItemData(item);
    wxTreeItemId newItem = treeCtrl_->AppendItem(pItem, s_item.name);

    s_item.itemId = newItem;
    s_item.fid = this->getMaxFid() + 1;

    this->setTreeItemData(newItem, s_item);

    this->saveTree();
}

void TreeHolder::moveAllChilds(const wxTreeItemId& srcItem, const wxTreeItemId& destItem) {
    sTreeItem ss = this->getTreeItemData(srcItem);
    sTreeItem sd = this->getTreeItemData(destItem);

    wxTreeItemId destChild = treeCtrl_->AppendItem(destItem, ss.name);
    ss.pid = sd.id;
    this->setTreeItemData(destChild, ss);

    wxTreeItemIdValue cookie;
    wxTreeItemId srcChild = treeCtrl_->GetFirstChild(srcItem, cookie);
    while (srcChild.IsOk()) {
        this->moveAllChilds(srcChild, destChild); // danger - recursia
        srcChild = treeCtrl_->GetNextChild(srcItem, cookie);
    }
}

} // namespace logic