#pragma once

#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/listbox.h>
#include <wx/numdlg.h>
#include <wx/textfile.h>
#include <wx/treectrl.h>
#include <wx/wx.h>

class DlgAppendItem : public wxDialog {
public:
    DlgAppendItem(wxWindow* parent, wxWindowID id, const wxString& title, long my_style);

    void onPressBtnDate(wxCommandEvent& event);

    wxTextCtrl *dlgEdtText, *dlgComments;
    wxButton* btnDate;
    wxComboBox* cbbTemplate;

    /* Template */
    struct str_templ {
        wxString name;
        wxString content;
    };

    wxVector<str_templ> vstr;

    void loadTemplateFromFile(wxString fn, wxVector<str_templ>& vstr);
    void fillTemplateBox();
    void onSelectTemplate(wxCommandEvent& event);
};