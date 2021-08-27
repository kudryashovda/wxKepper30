#include "wxLogic.h"

void wxLogic::AddTree(wxTreeCtrl* treeCtrl) {
    treeCtrl_ = treeCtrl;
}

wxTreeItemId wxLogic::CreateRootItem(const wxString& str) {
    auto root_item = treeCtrl_->AddRoot(str);

    id_to_wxitem_.push_back({ 0, 0, root_item, str, "", ItemStatus::Normal });
    wxitem_to_id_[root_item] = 0;

    return root_item;
}

wxTreeItemId wxLogic::AppendTreeItem(const wxTreeItemId& target, const wxString& name, const wxString& comment) {
    size_t target_item_id = wxitem_to_id_.at(target);

    auto new_item = treeCtrl_->AppendItem(target, name);
    size_t new_item_id = id_to_wxitem_.size();

    id_to_wxitem_.push_back({ new_item_id, target_item_id, new_item, name, comment, ItemStatus::Normal });
    wxitem_to_id_[new_item] = new_item_id;

    return new_item;
}

TreeItem wxLogic::GetTreeItemInfo(const wxTreeItemId& item) {
    auto item_id = wxitem_to_id_.at(item);

    return id_to_wxitem_.at(item_id);
}

void wxLogic::SaveTree() {
    wxTextFile tfile("test.txt");
    if (not tfile.Exists()) {
        tfile.Create();
    }

    tfile.Open();
    tfile.Clear();

    for (auto item : id_to_wxitem_) {
        item.name.ToUTF8();
        item.name.Replace('\\', "\\\\");
        item.name.Replace('\t', "\\t");

        item.comment.ToUTF8();
        item.comment.Replace('\\', "\\\\");
        item.comment.Replace('\t', "\\t");
        item.comment.Replace('\n', "\\n");

        wxChar status = 'U';
        if (item.status == ItemStatus::Normal) {
            status = 'N';
        } else if (item.status == ItemStatus::Deleted) {
            status = 'D';
        };

        wxString line = wxString::Format(wxT("%zu\t%zu\t%s\t%s\t%c"), item.id, item.parent_id, item.name, item.comment, status);
        tfile.AddLine(line);
    }

    tfile.Write();
    tfile.Close();
}

void wxLogic::LoadTree() {

    // auto root_item = treeCtrl_->GetRootItem();
    treeCtrl_->DeleteAllItems();
    id_to_wxitem_.clear();
    wxitem_to_id_.clear();

    // id_to_wxitem_.push_back({0,0, root_item, "Root", "", ItemStatus::Normal});
    // wxitem_to_id_[root_item] = 0;
    CreateRootItem("Root");

    wxVector<TreeItem> vs;

    wxTextFile tfile("test.txt");
    if (!tfile.Exists())
        return;

    tfile.Open();

    for (size_t i = 1; i < tfile.GetLineCount(); ++i) {
        auto ti = this->tokenizer(tfile[i], "\t");

        ti.name.Replace("\\t", '\t');
        ti.name.Replace("\\\\", '\\');

        ti.comment.Replace("\\t", '\t');
        ti.comment.Replace("\\n", '\n');
        ti.comment.Replace("\\\\", '\\');

        id_to_wxitem_.push_back(ti);
    }

    tfile.Close();

    // Fill tree
    for (size_t id = 1; id < id_to_wxitem_.size(); ++id) {
        auto item = id_to_wxitem_.at(id);

        auto parent_id = item.parent_id;
        auto parent_item = id_to_wxitem_.at(parent_id).wxitem;

        if (item.status == ItemStatus::Normal) {
            auto new_item = treeCtrl_->AppendItem(parent_item, item.name);

            id_to_wxitem_[item.id].wxitem = new_item;
            wxitem_to_id_[new_item] = item.id;
        }
    }

    treeCtrl_->Expand(treeCtrl_->GetRootItem());
}

TreeItem wxLogic::tokenizer(wxString str, wxString delim) {
    wxVector<wxString> vs;

    size_t pos;
    wxString token;
    do {
        pos = str.find_first_of(delim);
        token = str.substr(0, pos);
        vs.push_back(token);
        str = str.substr(pos + 1);
    } while (pos != wxString::npos);

    int id = wxAtoi(vs[0]);
    int parent_id = wxAtoi(vs[1]);
    wxString raw_name = vs[2];
    wxString raw_comment = vs[3];
    wxString raw_status = vs[4];

    ItemStatus status;
    if (raw_status == "N") {
        status = ItemStatus::Normal;
    } else if (raw_status == "D") {
        status = ItemStatus::Deleted;
    }

    TreeItem ti{ id, parent_id, NULL, raw_name, raw_comment, status };

    return ti;
}