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

void saveBrunch(wxString filename, const wxTreeCtrl* treeCtrl, const wxTreeItemId& item) {
    wxVector<sTreeItem> v;

    size_t init_id = 0;
    utils::getAllItemsData(treeCtrl, item, v, init_id);

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

sTreeItem s_tokenizer(wxString str, wxString delim, size_t elements) {
    wxVector<wxString> vs;
    size_t pos;
    wxString token;

    if (str.Find(delim) == wxNOT_FOUND)
        return ST_ERROR; // if no delim string exit

    do {
        pos = str.find_first_of(delim);
        token = str.substr(0, pos);
        vs.push_back(token);
        str = str.substr(pos + 1);
    } while (pos != wxString::npos);

    if (vs.size() != elements)
        return ST_ERROR;

    sTreeItem st(NULL, wxAtoi(vs[0]), wxAtoi(vs[1]), vs[2], wxAtoi(vs[3]), vs[4], vs[5]);

    return st;
}

void loadTree(wxString filename, wxTreeCtrl* treeCtrl, const wxTreeItemId& item2connect, size_t elements) {

    wxVector<sTreeItem> vs;
    wxTextFile tfile(filename);
    if (!tfile.Exists())
        return;

    tfile.Open();

    for (size_t i = 1, s = tfile.GetLineCount(); i < s; ++i) {
        sTreeItem sti = s_tokenizer(tfile[i], '\t', elements);
        if (sti == ST_ERROR)
            continue;
        vs.push_back(sti);
    }

    tfile.Close();

    if (vs.size() == 0)
        return;

    for (auto& it : vs) {
        it.itemId = treeCtrl->AppendItem(item2connect, it.name);
        utils::setTreeItemData(treeCtrl, it.itemId, it);
    }

    for (auto& it : vs) {      // pid
        for (auto& it2 : vs) { // id
            if (it.pid == it2.id) {
                treeCtrl->Delete(it.itemId);
                it.itemId = treeCtrl->AppendItem(it2.itemId, it.name);
                utils::setTreeItemData(treeCtrl, it.itemId, it);
            }
        }
    }
}

Settings getSettingsFromFile(wxString fn, wxChar dir_separator) {
    Settings iniSettings;

    iniSettings.dataPath = wxGetCwd() + dir_separator;
    iniSettings.dataFilename = "data.txt";
    iniSettings.templateFileName = "templates.txt";
    iniSettings.userName = "USER";

    wxTextFile tfile(fn);
    if (!tfile.Exists())
        return iniSettings;

    tfile.Open();

    wxVector<wxString> vs;
    wxString line_str = "";

    for (size_t i = 0, s = tfile.GetLineCount(); i < s; ++i) {
        line_str = tfile[i];
        line_str.ToUTF8();
        vs.push_back(line_str);
    }

    tfile.Close();

    if (vs.size() != 4)
        return iniSettings;

    iniSettings.dataPath = vs[0];
    iniSettings.dataFilename = vs[1];
    iniSettings.templateFileName = vs[2];
    iniSettings.userName = vs[3];

    return iniSettings;
}


} // namespace utils