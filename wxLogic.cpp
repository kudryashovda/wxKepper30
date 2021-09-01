#include "wxLogic.h"

void wxLogic::SetTree(wxTreeCtrl* treeCtrl) {
    treeCtrl_ = treeCtrl;
}

wxTreeItemId wxLogic::AppendTreeItem(const wxTreeItemId& target, const wxString& name, const wxString& comment) {
    if (target == NULL) {
        return NULL;
    }

    int target_item_id = wxitem_to_id_.at(target);

    auto new_item = treeCtrl_->AppendItem(target, name);
    int new_item_id = ids_.size();
    ids_.push_back(new_item_id);

    id_to_info_[new_item_id] = { new_item_id, target_item_id, new_item, name, comment, ItemStatus::Normal };
    wxitem_to_id_[new_item] = new_item_id;

    return new_item;
}

const TreeItem& wxLogic::GetTreeItemInfo(const wxTreeItemId& item) {
    auto item_id = wxitem_to_id_.at(item);

    const auto& info = id_to_info_.at(item_id);

    return info;
}

void wxLogic::SaveTree() {
    if (!fs::exists(db_path_)) {
        fs::create_directories(db_workdir_);
    }

    std::ofstream os(db_path_);

    for (auto id : ids_) {
        TreeItem item = id_to_info_.at(id); // not ref

        item.name.ToUTF8();
        item.name = utils::ScreenSpecialChars(item.name.ToStdString());

        item.comment.ToUTF8();
        item.comment = utils::ScreenSpecialChars(item.comment.ToStdString());

        wxChar status = 'U';
        if (item.status == ItemStatus::Normal) {
            status = 'N';
        } else if (item.status == ItemStatus::Deleted) {
            status = 'D';
        };

        wxString line = wxString::Format(wxT("%d\t%d\t%s\t%s\t%c"), id, item.parent_id, item.name, item.comment, status);
        os << line << '\n';
    }
}

wxTreeItemId wxLogic::GetParentTreeItemPtrById(int item_id) {
    int parent_id = id_to_info_.at(item_id).parent_id;

    return id_to_info_.at(parent_id).wxitem;
}

void wxLogic::LoadTree() {
    if (!treeCtrl_->IsEmpty()) {
        treeCtrl_->DeleteAllItems();

        ids_.clear();
        id_to_info_.clear();
        wxitem_to_id_.clear();
    }

    auto root_item_ptr = treeCtrl_->AddRoot("Root");

    int root_id = 0;
    ids_.push_back(root_id);
    id_to_info_[root_id] = { root_id, 0, root_item_ptr, "Root", "", ItemStatus::Normal };
    wxitem_to_id_[root_item_ptr] = root_id;

    wxVector<TreeItem> vs;

    if (!fs::exists(db_path_)) {
        return;
    }

    std::ifstream is(db_path_);
    std::string line;
    std::getline(is, line); // pass root string

    while (std::getline(is, line)) {
        if (line.empty()) {
            continue;
        }

        TreeItem ti;

        auto tokens = this->tokenizer(line, "\t");

        ti.id = wxAtoi(tokens[0]);
        ti.parent_id = wxAtoi(tokens[1]);

        ti.name = tokens[2];
        ti.comment = tokens[3];

        wxString raw_status = tokens[4];
        ti.status = ItemStatus::Normal;
        if (raw_status == "D") {
            ti.status = ItemStatus::Deleted;
        }

        // Remove screen slashes
        ti.name = utils::TransformToSpecialChars(ti.name.ToStdString());
        ti.comment = utils::TransformToSpecialChars(ti.comment.ToStdString());

        ids_.push_back(ti.id);
        id_to_info_[ti.id] = ti;
    }

    // Fill tree
    for (size_t i = 1; i < ids_.size(); ++i) {
        auto item_id = ids_.at(i);
        auto item_info = id_to_info_.at(item_id);

        if (item_info.wxitem != NULL || item_info.status == ItemStatus::Deleted) {
            continue;
        }

        auto parent_item_ptr = GetParentTreeItemPtrById(item_id);

        if (parent_item_ptr != NULL) {
            CreateNewTreeItem(parent_item_ptr, item_info.name, item_id);
        } else {
            int j = i;

            while (parent_item_ptr == NULL && (j + 1) < ids_.size()) {
                ++j;
                if (id_to_info_.at(ids_.at(j)).wxitem == NULL) {
                    parent_item_ptr = GetParentTreeItemPtrById(ids_.at(j));
                }
            }

            if (j == ids_.size()) {
                throw std::logic_error("bad file");
            }

            item_id = ids_.at(j);
            wxString current_name = id_to_info_.at(item_id).name;

            if (id_to_info_.at(item_id).status == ItemStatus::Deleted) {
                continue;
            }

            CreateNewTreeItem(parent_item_ptr, current_name, item_id);

            --i; // step back
        }
    }

    treeCtrl_->Expand(treeCtrl_->GetRootItem());
}

void wxLogic::CreateNewTreeItem(wxTreeItemId parent_ptr, const wxString& name, int item_id) {
    auto new_item_ptr = treeCtrl_->AppendItem(parent_ptr, name);

    id_to_info_[item_id].wxitem = new_item_ptr;
    wxitem_to_id_[new_item_ptr] = item_id;
}

wxVector<wxString> wxLogic::tokenizer(wxString str, wxString delim) {
    wxVector<wxString> vs;

    size_t pos;
    wxString token;
    do {
        pos = str.find_first_of(delim);
        token = str.substr(0, pos);
        vs.push_back(token);
        str = str.substr(pos + 1);
    } while (pos != wxString::npos);

    return vs;
}

void wxLogic::DeleteItem(wxTreeItemId item_ptr) {
    if (item_ptr == NULL) {
        return;
    }

    auto item_id = wxitem_to_id_.at(item_ptr);

    id_to_info_[item_id].status = ItemStatus::Deleted;
    id_to_info_[item_id].name.Clear();
    id_to_info_[item_id].comment.Clear();

    treeCtrl_->Delete(item_ptr);
}