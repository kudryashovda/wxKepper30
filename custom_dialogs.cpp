#include "custom_dialogs.h"

void DlgAppendItem::onPressBtnDate(wxCommandEvent& event) {
    event.Skip(true);

    wxDateTime now = wxDateTime::Now();
    wxString str = now.Format(wxT("%d-%m-%Y_%H-%M-%S"), wxDateTime::Local);
    dlgEdtText->SetValue(str);
}

void DlgAppendItem::loadTemplateFromFile(wxString fn, wxVector<str_templ>& vstr) {
    vstr.clear();

    wxTextFile tfile(fn);
    if (!tfile.Exists())
        return;

    tfile.Open();

    str_templ line_str;
    for (size_t i = 0, s = tfile.GetLineCount(); i < s; ++i) {
        line_str.name = tfile[i].Before('\t');
        line_str.content = tfile[i].After('\t');
        line_str.content.Replace("\\n", "\n");
        vstr.push_back(line_str);
    }

    tfile.Close();
};

void DlgAppendItem::fillTemplateBox() {

};

void DlgAppendItem::onSelectTemplate(wxCommandEvent& event) {
    event.Skip(true);

    int pos = cbbTemplate->GetSelection();

    dlgComments->SetValue(vstr[pos].content);
}

/* It is a small dialog when Add new sample is pressed */

/* It is an implementation of a small dialog when Add new sample is pressed */
/* The dialog makes it possible to add multiple items with one content */
DlgAppendItem::DlgAppendItem(wxWindow* parent, wxWindowID id, const wxString& title, long my_style)
    : wxDialog(parent, id, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER) {
    wxBoxSizer* borderSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* textTemplate = new wxStaticText(this, wxID_ANY, "Template: ");

    cbbTemplate = new wxComboBox(this, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY | my_style);
    cbbTemplate->Bind(wxEVT_COMMAND_COMBOBOX_SELECTED, &DlgAppendItem::onSelectTemplate, this);

    fillTemplateBox();

    dlgEdtText = new wxTextCtrl(this, wxID_ANY, "Name", wxDefaultPosition, wxDefaultSize, my_style);
    dlgComments = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(100, 60), wxTE_MULTILINE | my_style);

    btnDate = new wxButton(this, wxID_ANY, "Date", wxDefaultPosition, wxDefaultSize, my_style | wxBU_EXACTFIT);
    btnDate->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &DlgAppendItem::onPressBtnDate, this);

    hSizer->Add(dlgEdtText, 1, wxEXPAND, 0);
    hSizer->AddSpacer(5);
    hSizer->Add(btnDate, 0, wxEXPAND, 0);

    vSizer->Add(hSizer, 0, wxEXPAND | wxBOTTOM, 5);

    vSizer->Add(textTemplate);
    vSizer->Add(cbbTemplate, 0, wxEXPAND | wxBOTTOM, 5);
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