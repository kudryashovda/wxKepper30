#include "wxLogic.h"

using namespace std;

void wxLogic::SetTree(wxTreeCtrl* treeCtrl) {
    treeCtrl_ = treeCtrl;
}

wxTreeItemId wxLogic::AppendTreeItem(const wxTreeItemId& target, const wxString& name, const wxString& comment) {
    if (target == nullptr) {
        return nullptr;
    }

    auto target_item_id = wxitem_to_id_.at(target);

    long new_item_id;

    if (!id_to_info_.at(0).comment.ToLong(&new_item_id)) {
        new_item_id = 1;
    }

    ids_.push_back(new_item_id);

    auto new_item = CreateNewTreeItem(target, name, new_item_id);

    id_to_info_[new_item_id].parent_id = target_item_id;
    id_to_info_[new_item_id].name = name;
    id_to_info_[new_item_id].comment = comment;
    id_to_info_[new_item_id].status = ItemStatus::Normal;

    // To internal storage config
    id_to_info_[0].comment.clear();
    id_to_info_[0].comment << (new_item_id + 1);

    this->SaveTree();

    return new_item;
}

void wxLogic::UpdateTreeItem(const wxTreeItemId& target, const wxString& name, const wxString& comment) {
    if (target == nullptr) {
        return;
    }

    long target_item_id = wxitem_to_id_.at(target);

    id_to_info_[target_item_id].name = name;
    id_to_info_[target_item_id].comment = comment;

    this->SaveTree();
}

const TreeItem& wxLogic::GetTreeItemInfo(const wxTreeItemId& item) {
    auto item_id = wxitem_to_id_.at(item);

    const auto& info = id_to_info_.at(item_id);

    return info;
}

const TreeItem& wxLogic::GetTreeItemInfo(long item_id) {
    return id_to_info_.at(item_id);
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

        char status = 'U';
        if (item.status == ItemStatus::Normal) {
            status = 'N';
        } else if (item.status == ItemStatus::Archived) {
            status = 'A';
        };
       
        os << id << '\t' << item.parent_id << '\t' << item.name << '\t' << item.comment << '\t' << status << '\n';
    }
}

wxTreeItemId wxLogic::GetParentTreeItemPtrById(long item_id) {
    long parent_id = id_to_info_.at(item_id).parent_id;

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
    // settings
    auto settings = wxLogic::tokenizer(line, "\t");
    id_to_info_[root_id].comment = settings[3];

    while (std::getline(is, line)) {
        if (line.empty()) {
            continue;
        }

        TreeItem ti;

        auto tokens = wxLogic::tokenizer(line, "\t");

        ti.id = wxAtoi(tokens[0]);
        ti.parent_id = wxAtoi(tokens[1]);

        ti.name = tokens[2];
        ti.comment = tokens[3];

        wxString raw_status = tokens[4];
        ti.status = ItemStatus::Normal;
        if (raw_status == "A") {
            ti.status = ItemStatus::Archived;
        }

        // Remove screen slashes
        ti.name = utils::TransformToSpecialChars(ti.name.ToStdString());
        ti.comment = utils::TransformToSpecialChars(ti.comment.ToStdString());

        ids_.push_back(ti.id);
        id_to_info_[ti.id] = ti;
    }

    // Fill tree
    for (long i = 1; i < ids_.size(); ++i) {
        auto item_id = ids_.at(i);
        auto item_info = id_to_info_.at(item_id);

        if (item_info.wxitem != nullptr || item_info.status == ItemStatus::Archived) {
            continue;
        }

        auto parent_item_ptr = GetParentTreeItemPtrById(item_id);

        if (parent_item_ptr != nullptr) {
            CreateNewTreeItem(parent_item_ptr, item_info.name, item_id);
        } else {
            long j = i;

            while (parent_item_ptr == nullptr && (j + 1) < ids_.size()) {
                ++j;
                if (id_to_info_.at(ids_.at(j)).wxitem == nullptr) {
                    parent_item_ptr = GetParentTreeItemPtrById(ids_.at(j));
                }
            }

            if (j == ids_.size()) {
                throw std::logic_error("bad file");
            }

            item_id = ids_.at(j);
            wxString current_name = id_to_info_.at(item_id).name;

            if (id_to_info_.at(item_id).status == ItemStatus::Archived) {
                continue;
            }

            CreateNewTreeItem(parent_item_ptr, current_name, item_id);

            --i; // step back
        }
    }

    treeCtrl_->Expand(treeCtrl_->GetRootItem());
}

wxTreeItemId wxLogic::CreateNewTreeItem(wxTreeItemId parent_ptr, const wxString& name, long item_id) {
    auto new_item_ptr = treeCtrl_->AppendItem(parent_ptr, name);

    id_to_info_[item_id].id = item_id;
    id_to_info_[item_id].wxitem = new_item_ptr;
    wxitem_to_id_[new_item_ptr] = item_id;

    return new_item_ptr;
}

wxVector<wxString> wxLogic::tokenizer(wxString str, const wxString& delim) {
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

bool wxLogic::ItemHasChild(wxTreeItemId item_ptr) {
    if (item_ptr == nullptr) {
        return false;
    }

    auto item_id = wxitem_to_id_.at(item_ptr);

    for (const auto& it : id_to_info_) {
        if (it.second.parent_id == item_id) {
            return true;
        }
    }

    return false;
}

int wxLogic::DeleteItem(wxTreeItemId item_ptr) {

    if (item_ptr == nullptr) {
        return -1;
    }

    auto item_id = wxitem_to_id_.at(item_ptr);

    if (item_id == 0) {
        return -1;
    }

    if (ItemHasChild(item_ptr)) {
        return -1;
    }

    id_to_info_.erase(item_id);
    wxitem_to_id_.erase(item_ptr);

    ids_.erase(std::remove(ids_.begin(), ids_.end(), item_id),
               ids_.end());

    this->SaveTree();

    return 0; // is Ok
}

bool wxLogic::IsItemIdExists(long item_id) {
    if (id_to_info_.count(item_id) > 0) {
        return true;
    }

    return false;
}

void wxLogic::CreateFile(wxTreeItemId item_ptr, const string& filename) {
    if (!item_ptr) {
        return;
    }

    const long item_id = wxitem_to_id_.at(item_ptr);

    auto file_path = files_workdir_ / to_string(item_id) / filename;
    if (!fs::exists(file_path.parent_path())) {
        fs::create_directories(file_path.parent_path());
    }

    while (fs::exists(file_path)) {
        auto new_filename = "copy_"s + file_path.filename().string();
        file_path = file_path.parent_path() / new_filename;
    }

    std::ofstream output(file_path);
}

fs::path wxLogic::GetItemPath(const TreeItem& info) {
    return files_workdir_ / std::to_string(info.id);
}

fs::path wxLogic::GetItemPath(wxTreeItemId item_ptr) {
    return GetItemPath(GetTreeItemInfo(item_ptr));
}

bool wxLogic::IsValidFilename(const std::string& filename) {

    const unordered_set<string> illegal_names = { "NUL", "CON", "PRN", "AUX", "COM0", "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9", "LPT0", "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9" };
    if (illegal_names.find(filename) != illegal_names.end()) {
        return false;
    }

    const string illegal_chars = "\\/:?\"<>|$";

    for (char ch : filename) {
        if (illegal_chars.find(ch) != string::npos) {
            return false;
        }
    }

    return true;
}

void wxLogic::CopyFiles(wxTreeItemId item_ptr, const vector<fs::path>& paths) {
    if (!item_ptr) {
        return;
    }

    const long item_id = wxitem_to_id_.at(item_ptr);

    auto files_dir = files_workdir_ / to_string(item_id);
    if (!fs::exists(files_dir)) {
        fs::create_directories(files_dir);
    }

    for (const auto& path : paths) {
        auto dest_path = files_dir / path.filename();

        while (fs::exists(dest_path)) {
            auto new_filename = "copy_"s + dest_path.filename().string();
            dest_path = dest_path.parent_path() / new_filename;
        }

        fs::copy(path, dest_path);
    }
}