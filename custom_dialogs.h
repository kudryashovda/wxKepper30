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

    wxTextCtrl *dlgEdtText, *dlgComments;
};