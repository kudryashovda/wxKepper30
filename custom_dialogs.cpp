#include "custom_dialogs.h"

/* It is a small dialog when Add new sample is pressed */

/* It is an implementation of a small dialog when Add new sample is pressed */
/* The dialog makes it possible to add multiple items with one content */
DlgAppendItem::DlgAppendItem(wxWindow* parent, wxWindowID id, const wxString& title, long my_style)
    : wxDialog(parent, id, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER) {
    wxBoxSizer* borderSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);

    dlgEdtText = new wxTextCtrl(this, wxID_ANY, "Name", wxDefaultPosition, wxDefaultSize, my_style);
    dlgComments = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(100, 60), wxTE_MULTILINE | my_style);

    hSizer->Add(dlgEdtText, 1, wxEXPAND, 0);
    hSizer->AddSpacer(5);

    vSizer->Add(hSizer, 0, wxEXPAND | wxBOTTOM, 5);

    vSizer->Add(dlgComments, 1, wxEXPAND | wxBOTTOM, 5);

    vSizer->Add(CreateButtonSizer(wxYES | wxCANCEL | my_style), 0, wxCENTRE, 0);

    borderSizer->Add(vSizer, 1, wxEXPAND | wxALL, 10);

    SetSizer(borderSizer);

    borderSizer->Fit(this);

    wxSize ws = this->GetSize();
    ws.SetHeight(ws.GetHeight() * 2);
    ws.SetWidth(ws.GetWidth() * 2);

#if defined(__WINDOWS__)
    this->SetMinClientSize(ws);
#else
    SetMinSize(ws); // linux
#endif

    SetClientSize(ws); //ClientToWindowSize(ws));

    dlgEdtText->SetFocus();
    dlgEdtText->SelectAll();
}
/* It is an implementation of a small dialog when Add new sample is pressed */