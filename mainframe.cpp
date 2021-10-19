#include "mainframe.h"

MainFrame::MainFrame(const wxString& title, wxLogic& logic)
    : wxFrame(NULL, wxID_ANY, title)
    , logic_(logic) {

/* buttons and edits style */
#if defined(__WINDOWS__)
    style_ = wxNO_BORDER;
#else
    style_ = wxSTATIC_BORDER;
#endif

#if defined(__WINDOWS__)
    SetIcon(wxICON(icon)); // windows only. icon file mt be located
#endif
    pnl = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER | style_);

    treeCtrl = new wxTreeCtrl(pnl, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                              wxTR_DEFAULT_STYLE | wxTR_MULTIPLE | style_, wxDefaultValidator, wxTreeCtrlNameStr);

    edtSearch = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | style_,
                               wxDefaultValidator, wxTextCtrlNameStr);
    btnSearch = new wxButton(pnl, wxID_ANY, "Search", wxDefaultPosition, wxDefaultSize, style_);

    searchBarSizer = new wxBoxSizer(wxHORIZONTAL);
    searchBarSizer->Add(edtSearch, 1, wxEXPAND, 0);
    searchBarSizer->Add(btnSearch, 0, wxEXPAND, 0);

    btnCut = new wxButton(pnl, wxID_ANY, "Cut", wxDefaultPosition, wxDefaultSize, style_);
    btnAdd = new wxButton(pnl, wxID_ANY, "Add", wxDefaultPosition, wxDefaultSize, style_);
    btnDel = new wxButton(pnl, wxID_ANY, "Delete", wxDefaultPosition, wxDefaultSize, style_);
    // btnExport = new wxButton(pnl, wxID_ANY, "Export", wxDefaultPosition, wxDefaultSize, style_);
    // btnImport = new wxButton(pnl, wxID_ANY, "Import", wxDefaultPosition, wxDefaultSize, style_);

    btnGoto = new wxButton(pnl, wxID_ANY, "Goto ID", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | style_,
                           wxDefaultValidator, wxTextCtrlNameStr);

    gsTopBtns = new wxGridSizer(1, 3, 1, 1); // 3 - buttons count in grid
    gsTopBtns->Add(btnAdd, 1, wxEXPAND, 0);
    gsTopBtns->Add(btnCut, 1, wxEXPAND, 0);
    gsTopBtns->Add(btnDel, 1, wxEXPAND, 0);
    // gsTopBtns->Add(btnExport, 1, wxEXPAND, 0);
    // gsTopBtns->Add(btnImport, 1, wxEXPAND, 0);

    lblName = new wxStaticText(pnl, wxID_ANY, "Name:");
    edtName = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | style_,
                             wxDefaultValidator, wxTextCtrlNameStr);

    nameBarSizer = new wxBoxSizer(wxVERTICAL);
    nameBarSizer->Add(gsTopBtns, 0, wxEXPAND, 0);
    nameBarSizer->AddSpacer(5);
    nameBarSizer->Add(lblName, 0, wxEXPAND, 0);
    nameBarSizer->Add(edtName, 0, wxEXPAND, 0);

    lblId = new wxStaticText(pnl, wxID_ANY, "");
    edtId = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, style_,
                           wxDefaultValidator, wxTextCtrlNameStr);
    edtId->Refresh();

    idBarSizer = new wxBoxSizer(wxVERTICAL);
    idBarSizer->Add(btnGoto, 0, wxEXPAND, 0);
    idBarSizer->AddSpacer(5);
    idBarSizer->Add(lblId, 0, wxEXPAND, 0);
    idBarSizer->Add(edtId, 0, wxEXPAND, 0);

    lblsBarSizer = new wxBoxSizer(wxHORIZONTAL);
    lblsBarSizer->Add(nameBarSizer, 1, wxEXPAND, 0);
    lblsBarSizer->AddSpacer(5);
    lblsBarSizer->Add(idBarSizer, 0, wxEXPAND, 0);

    lblComtsName = new wxStaticText(pnl, wxID_ANY, "Comments");
    tc = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize,
                        wxTE_MULTILINE | style_, // wxTE_RICH
                        wxDefaultValidator, wxTextCtrlNameStr);

    lblListName = new wxStaticText(pnl, wxID_ANY, "Attached objects", wxDefaultPosition, wxDefaultSize, style_);
    btnNewObject = new wxButton(pnl, wxID_ANY, "New", wxDefaultPosition, wxDefaultSize, style_);
    btnaddObjectsToItem = new wxButton(pnl, wxID_ANY, "Add", wxDefaultPosition, wxDefaultSize, style_);
    btnDelObject = new wxButton(pnl, wxID_ANY, "Delete", wxDefaultPosition, wxDefaultSize, style_);
    btnRenameObj = new wxButton(pnl, wxID_ANY, "Rename", wxDefaultPosition, wxDefaultSize, style_);
    btnPhoto = new wxButton(pnl, wxID_ANY, "Photo", wxDefaultPosition, wxDefaultSize, style_);
    btnLink = new wxButton(pnl, wxID_ANY, "Link to...", wxDefaultPosition, wxDefaultSize, style_);

    gsObjBtns = new wxGridSizer(1, 6, 1, 1);

    gsObjBtns->Add(btnNewObject, 1, wxEXPAND, 0);
    gsObjBtns->Add(btnaddObjectsToItem, 1, wxEXPAND, 0);
    gsObjBtns->Add(btnDelObject, 1, wxEXPAND, 0);
    gsObjBtns->Add(btnRenameObj, 1, wxEXPAND, 0);
    gsObjBtns->Add(btnPhoto, 1, wxEXPAND, 0);
    gsObjBtns->Add(btnLink, 1, wxEXPAND, 0);

    listBox = new wxListBox(pnl, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, style_);

    btnSaveItemData = new wxButton(pnl, wxID_ANY, "Save", wxDefaultPosition, wxDefaultSize, style_);

    leftSizer = new wxBoxSizer(wxVERTICAL);
    leftSizer->Add(treeCtrl, 1, wxEXPAND, 0);
    leftSizer->AddSpacer(2);
    leftSizer->Add(searchBarSizer, 0, wxEXPAND, 0);

    mainSizer = new wxBoxSizer(wxHORIZONTAL);

    mainSizer->Add(leftSizer, 2, wxEXPAND, 0);

    mainSizer->AddSpacer(5);

    rightSizer = new wxBoxSizer(wxVERTICAL);
    rightSizer->Add(lblsBarSizer, 0, wxEXPAND, 0);
    rightSizer->AddSpacer(10);
    rightSizer->Add(lblComtsName);
    rightSizer->Add(tc, 3, wxEXPAND, 0);
    rightSizer->AddSpacer(5);
    rightSizer->Add(btnSaveItemData, 0, wxEXPAND, 0);

    rightSizer->AddSpacer(20);
    rightSizer->Add(lblListName);

    rightSizer->AddSpacer(2);
    rightSizer->Add(listBox, 1, wxEXPAND, 0);
    rightSizer->AddSpacer(2);
    rightSizer->Add(gsObjBtns, 0, wxEXPAND, 0);

    mainSizer->Add(rightSizer, 3, wxEXPAND, 0);

    borderSizer = new wxBoxSizer(wxVERTICAL);
    borderSizer->Add(mainSizer, 1, wxEXPAND | wxALL, 2);

    pnl->SetSizer(borderSizer);

    mainSizer->Fit(this); // resize (fit) main window based on elements inside sizer

    SetMinSize(wxSize(800, 800));
    Centre(wxHORIZONTAL);

    BindEvents();

    logic_.SetTree(treeCtrl);

    logic_.LoadTree();

    auto root_item = treeCtrl->GetRootItem();
    treeCtrl->SelectItem(root_item);
    treeCtrl->SetFocusedItem(root_item);
}

DlgAppendItem::DlgAppendItem(wxWindow* parent, wxWindowID id, const wxString& title, long style_)
    : wxDialog(parent, id, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
    , style_(style_) {
    wxBoxSizer* borderSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);

    dlgEdtText = new wxTextCtrl(this, wxID_ANY, "Name", wxDefaultPosition, wxDefaultSize, style_);
    dlgComments = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(100, 60), wxTE_MULTILINE | style_);

    hSizer->Add(dlgEdtText, 1, wxEXPAND, 0);
    hSizer->AddSpacer(5);

    vSizer->Add(hSizer, 0, wxEXPAND | wxBOTTOM, 5);

    vSizer->Add(dlgComments, 1, wxEXPAND | wxBOTTOM, 5);

    vSizer->Add(CreateButtonSizer(wxYES | wxCANCEL | style_), 0, wxCENTRE, 0);

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

    SetClientSize(ws); // ClientToWindowSize(ws));

    dlgEdtText->SetFocus();
    dlgEdtText->SelectAll();
}

void MainFrame::BindEvents() {
    btnAdd->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::OnBtnAddClick, this);
    treeCtrl->Bind(wxEVT_TREE_SEL_CHANGED, &MainFrame::onTreeItemClick, this);
    // btnExport->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnExport, this);
    // btnImport->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnImport, this);
    btnDel->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnDel, this);
    btnCut->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnCut, this);
    btnSaveItemData->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnSaveItemData, this);
    btnGoto->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnGotoId, this);
}

void MainFrame::onPressbtnSaveItemData(wxCommandEvent& event) {
    wxTreeItemId selected_item = treeCtrl->GetFocusedItem();
    if (!selected_item.IsOk()) {
        return;
    }

    if (selected_item == treeCtrl->GetRootItem()) {
        return;
    }

    wxString name = edtName->GetValue();
    wxString comment = tc->GetValue();

    logic_.UpdateTreeItem(selected_item, name, comment);

    // Update tree item
    treeCtrl->SetItemText(selected_item, name);
}

void MainFrame::ShowCard(const TreeItem& info) {
    edtId->SetValue(std::to_string(info.id));
    edtName->SetValue(info.name);
    tc->SetValue(info.comment);
}

void MainFrame::AppendItems(const wxArrayTreeItemIds& selected_items, int count) {
    if (selected_items.empty()) {
        return;
    }

    DlgAppendItem dlg(this, wxID_ANY, "Add object", this->style_);
    if (dlg.ShowModal() == wxID_CANCEL)
        return;

    wxString name = dlg.dlgEdtText->GetValue();
    wxString comment = dlg.dlgComments->GetValue();

    for (const auto& selected_item : selected_items) {
        for (int i = 0; i < count; ++i) {
            wxString suffix;
            if (count > 1) {
                suffix << (i + 1);
            }
            logic_.AppendTreeItem(selected_item, name + suffix, comment);
            treeCtrl->Expand(selected_item);
        }
    }
}

void MainFrame::OnBtnAddClick(wxCommandEvent& event) {
    wxArrayTreeItemIds selected_items;
    treeCtrl->GetSelections(selected_items);

    AppendItems(selected_items, 1);
}

void MainFrame::onPressbtnCut(wxCommandEvent& event) {
    wxArrayTreeItemIds selected_items;
    treeCtrl->GetSelections(selected_items);

    wxNumberEntryDialog dlgCut(this, "Cut the sample into", "Number of samples", "Cut", 1, 0, 100);
    if (dlgCut.ShowModal() != wxID_OK) {
        return;
    }
    int items_count = dlgCut.GetValue();

    AppendItems(selected_items, items_count);
}

void MainFrame::onTreeItemClick(wxCommandEvent& event) {
    auto selected_item = treeCtrl->GetFocusedItem();

    if (selected_item == treeCtrl->GetRootItem()) {
        tc->Clear();
        edtName->Clear();

        return;
    }

    auto info = logic_.GetTreeItemInfo(selected_item);

    this->ShowCard(info);
}

// void MainFrame::onPressbtnExport(wxCommandEvent& event) {
//     logic_.SaveTree();
// }

// void MainFrame::onPressbtnImport(wxCommandEvent& event) {
//     logic_.LoadTree();
// }

void MainFrame::onPressbtnDel(wxCommandEvent& event) {

    wxMessageDialog* dlgItem = new wxMessageDialog(
        NULL, wxT("Are you sure to delete item?"), wxT("Question"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
    if (dlgItem->ShowModal() == wxID_NO) {
        dlgItem->Destroy();
        return;
    }
    dlgItem->Destroy();

    auto selected_item = treeCtrl->GetFocusedItem();

    if (selected_item == NULL) {
        return;
    }

    auto status = logic_.DeleteItem(selected_item);

    if (status == 0) {
        treeCtrl->Delete(selected_item);
    }
}

void MainFrame::expandAllParents(wxTreeItemId item) {
    do {
        if (treeCtrl->ItemHasChildren(item))
            treeCtrl->Expand(item);

        item = treeCtrl->GetItemParent(item);
    } while (item.IsOk());
}

void MainFrame::onPressbtnGotoId(wxCommandEvent& event) {
    wxNumberEntryDialog dlg(this, "", "Enter ID number", "Goto ID", 0, 0, std::numeric_limits<int>::max());
    if (dlg.ShowModal() != wxID_OK)
        return;

    int item_id = dlg.GetValue();
    if (!logic_.IsItemIdExists(item_id)) {
        wxMessageBox("ID is not found", "Warning");
        return;
    }

    auto info = logic_.GetTreeItemInfo(item_id);

    ShowCard(info);
    expandAllParents(info.wxitem);

    treeCtrl->SelectItem(info.wxitem, true);
    treeCtrl->SetFocusedItem(info.wxitem);
}