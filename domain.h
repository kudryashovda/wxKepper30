#pragma once

#include <wx/treectrl.h>

struct Settings {
    wxString dataPath;
    wxString dataFilename;
    wxString templateFileName;
    wxString userName;
};

struct sTreeItem {
    sTreeItem(){};
    sTreeItem(wxTreeItemId itemId_,
              size_t id_,
              size_t pid_,
              wxString user_,
              size_t fid_,
              wxString name_,
              wxString comments_)
        : itemId(itemId_)
        , id(id_)
        , pid(pid_)
        , user(user_)
        , fid(fid_)
        , name(name_)
        , comments(comments_){};

    wxTreeItemId itemId = NULL;
    size_t id = 0;
    size_t pid = 0;
    wxString user = "USER";
    size_t fid = 0; // fixed id
    wxString name = "";
    wxString comments = "";

    wxString getObjPath(const wxString& data_path, wxChar dir_separator) {
        return wxString::Format(wxT("%s%s%zu%c"), data_path, user, fid, dir_separator);
    }

    bool operator==(const sTreeItem& rhs) {
        bool res = (itemId == rhs.itemId && id == rhs.id && pid == rhs.pid && fid == rhs.fid && name == rhs.name && comments == rhs.comments);
        return res;
    }
};


inline sTreeItem ST_ERROR(NULL, 0, 0, "USER", 0, "Error", "Error");
inline sTreeItem ST_ROOT(NULL, 0, 0, "USER", 0, "Root", "");

/* class to store item's data inside treectrl */
class MyTreeItemData : public wxTreeItemData {
private:
    sTreeItem pdata;

public:
    MyTreeItemData(const sTreeItem& st)
        : pdata(st){}; // constructor

    const sTreeItem& GetStruct() const {
        return pdata;
    }
};