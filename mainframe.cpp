#include "mainframe.h"

using namespace std;

MainFrame::MainFrame(const wxString& title, wxLogic& logic)
    : wxFrame(nullptr, wxID_ANY, title)
    , logic_(logic) {

/* buttons and edits style */
#if defined(__WINDOWS__)
    style_ = wxBORDER_NONE;
#else
    style_ = wxSTATIC_BORDER;
#endif

#if defined(__WINDOWS__)
    SetIcon(wxICON(icon)); // windows only. icon file mt be located
#endif
    wxColor labels_color = wxColour(100, 100, 100);
    pnl = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, style_);

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
    btnGoto = new wxButton(pnl, wxID_ANY, "Goto ID", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | style_,
                           wxDefaultValidator, wxTextCtrlNameStr);

    gsTopBtns = new wxGridSizer(1, 3, 1, 1); // 3 - buttons count in grid
    gsTopBtns->Add(btnAdd, 1, wxEXPAND, 0);
    gsTopBtns->Add(btnCut, 1, wxEXPAND, 0);
    gsTopBtns->Add(btnDel, 1, wxEXPAND, 0);

    lblName = new wxStaticText(pnl, wxID_ANY, "Name:");
    lblName->SetForegroundColour(labels_color);
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
    edtId->SetEditable(false);
    edtId->SetBackgroundColour(edtName->GetBackgroundColour());
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
    lblComtsName->SetForegroundColour(labels_color);
    tc = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize,
                        wxTE_MULTILINE | style_, // wxTE_RICH
                        wxDefaultValidator, wxTextCtrlNameStr);

    lblListName = new wxStaticText(pnl, wxID_ANY, "Attached objects");
    lblListName->SetForegroundColour(labels_color);
    btnNewObject = new wxButton(pnl, wxID_ANY, "New File", wxDefaultPosition, wxDefaultSize, style_);
    btnaddObjectsToItem = new wxButton(pnl, wxID_ANY, "Add File", wxDefaultPosition, wxDefaultSize, style_);
    btnDelObject = new wxButton(pnl, wxID_ANY, "Delete File", wxDefaultPosition, wxDefaultSize, style_);
    btnRenameObj = new wxButton(pnl, wxID_ANY, "Rename File", wxDefaultPosition, wxDefaultSize, style_);

    gsObjBtns = new wxGridSizer(1, 4, 1, 1);

    gsObjBtns->Add(btnNewObject, 1, wxEXPAND, 0);
    gsObjBtns->Add(btnaddObjectsToItem, 1, wxEXPAND, 0);
    gsObjBtns->Add(btnDelObject, 1, wxEXPAND, 0);
    gsObjBtns->Add(btnRenameObj, 1, wxEXPAND, 0);

    listBox = new wxListBox(pnl, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, nullptr, style_);

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

    mainSizer->Fit(this);

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
    auto* borderSizer = new wxBoxSizer(wxVERTICAL);
    auto* vSizer = new wxBoxSizer(wxVERTICAL);
    auto* hSizer = new wxBoxSizer(wxHORIZONTAL);

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

    dlgEdtText->SetFocus();
    dlgEdtText->SelectAll();
}

void MainFrame::BindEvents() {
    btnAdd->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::OnBtnAddClick, this);
    treeCtrl->Bind(wxEVT_TREE_SEL_CHANGED, &MainFrame::onTreeItemClick, this);
    treeCtrl->Bind(wxEVT_TREE_ITEM_MENU, &MainFrame::onTreeItemRightClick, this);

    btnDel->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnDel, this);
    btnCut->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnCut, this);
    btnSaveItemData->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnSaveItemData, this);
    btnGoto->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnGotoId, this);
    btnSearch->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnSearch, this);

    listBox->Bind(wxEVT_LISTBOX_DCLICK, &MainFrame::onBoxItemDblClick, this);
    btnNewObject->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnCreateFile, this);
    btnaddObjectsToItem->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnaddObjectsToItem, this);
    btnDelObject->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnDelFile, this);
    btnRenameObj->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnRenameFile, this);
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
    edtId->SetValue(std::to_wstring(info.id));
    edtName->SetValue(info.name);
    tc->SetValue(info.comment);

    UpdateFileBox(listBox);
}

void MainFrame::UpdateFileBox(wxListBox* lbox) {
    auto selected_item = treeCtrl->GetFocusedItem();
    if (!selected_item) {
        return;
    }

    const auto& info = logic_.GetTreeItemInfo(selected_item);
    listBox->Clear();

    const auto item_path = logic_.GetItemPath(info);
    if (!fs::exists(item_path)) {
        return;
    }

    for (const auto& entry : fs::directory_iterator(item_path)) {
        wxString filename(entry.path().filename());
        lbox->Append(filename);
    }
}

void MainFrame::AppendItems(const wxArrayTreeItemIds& selected_items, long count) {
    if (selected_items.empty()) {
        return;
    }

    DlgAppendItem dlg(this, wxID_ANY, "Add object", this->style_);
    const auto dlg_size = wxSize(400, 400);
    dlg.SetMinSize(dlg_size);
    dlg.SetSize(dlg_size);

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

    CutItems(selected_items);
}

void MainFrame::CutItems(const wxArrayTreeItemIds& selected_items) {
    wxNumberEntryDialog dlgCut(this, "Cut the sample into", "Number of samples", "Cut", 1, 0, 100);
    if (dlgCut.ShowModal() != wxID_OK) {
        return;
    }
    auto items_count = dlgCut.GetValue();

    AppendItems(selected_items, items_count);
}

void MainFrame::onTreeItemClick(wxCommandEvent& event) {
    auto selected_item = treeCtrl->GetFocusedItem();

    if (selected_item == treeCtrl->GetRootItem()) {
        tc->Clear();
        edtName->Clear();
        listBox->Clear();

        return;
    }

    auto info = logic_.GetTreeItemInfo(selected_item);

    this->ShowCard(info);
}

void MainFrame::onPressbtnDel(wxCommandEvent& event) {
    auto selected_item = treeCtrl->GetFocusedItem();
    DeleteItem(selected_item);
}

void MainFrame::DeleteItem(wxTreeItemId item) {

    auto* dlgItem = new wxMessageDialog(
        nullptr, wxT("Are you sure to delete item?"), wxT("Question"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
    if (dlgItem->ShowModal() == wxID_NO) {
        dlgItem->Destroy();
        return;
    }
    dlgItem->Destroy();

    if (!item) {
        return;
    }

    auto status = logic_.DeleteItem(item);

    if (status == 0) {
        treeCtrl->Delete(item);
    }
}

void MainFrame::expandAllParents(wxTreeItemId item) {
    do {
        if (treeCtrl->ItemHasChildren(item)) {
            treeCtrl->Expand(item);
        }

        item = treeCtrl->GetItemParent(item);
    } while (item.IsOk());
}

void MainFrame::onPressbtnGotoId(wxCommandEvent& event) {
    wxNumberEntryDialog dlg(this, "", "Enter ID number", "Goto ID", 0, 0, std::numeric_limits<int>::max());
    if (dlg.ShowModal() != wxID_OK)
        return;

    auto item_id = dlg.GetValue();
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

void MainFrame::onPressbtnCreateFile(wxCommandEvent& event) {
    const auto selected_item = treeCtrl->GetFocusedItem();
    if (selected_item == treeCtrl->GetRootItem()) {
        return;
    }

    const wxString default_filename = "newfile.txt";
    wxTextEntryDialog dlg(this, "Enter filename", "Create empty text file", default_filename);
    if (dlg.ShowModal() != wxID_OK) {
        return;
    }

    const wstring filename = dlg.GetValue().ToStdWstring();

    if (filename.empty() || !utils::IsValidFilename(filename)) {
        wxMessageBox("Invalid filename", "Warning");
        return;
    }

    logic_.CreateFile(selected_item, filename);

    UpdateFileBox(listBox);
}

fs::path MainFrame::GetFilenameFromListbox(wxTreeItemId selected_item, wxListBox* lbox) {
    fs::path empty;

    if (!selected_item.IsOk() || listBox->IsEmpty()) {
        return empty;
    }

    const int selected_file_idx = lbox->GetSelection();
    if (selected_file_idx == -1) {
        return empty;
    }

    const wxString filename = lbox->GetString(selected_file_idx);
    return logic_.GetItemPath(selected_item) / filename.ToStdWstring();
}

void MainFrame::onBoxItemDblClick(wxCommandEvent& event) {
    const auto selected_item = treeCtrl->GetFocusedItem();
    if (!selected_item.IsOk()) {
        return;
    }

    const auto path = GetFilenameFromListbox(selected_item, listBox);
    if (path.empty() || !exists(path)) {
        wxMessageBox("No files found on disk", "Warning");
        return;
    }

    /* To enable open files and folder with non-latin chars add wxSetlocale(LC_ALL, "") to to Init foo */
    wxLaunchDefaultApplication(L"\"" + path.wstring() + L"\"");
}

void MainFrame::onPressbtnDelFile(wxCommandEvent& event) {
    const auto selected_item = treeCtrl->GetFocusedItem();
    if (!selected_item.IsOk() || listBox->IsEmpty()) {
        return;
    }

    const auto path = GetFilenameFromListbox(selected_item, listBox);
    if (path.empty() || !exists(path)) {
        return;
    }

    wxMessageDialog dlg(nullptr, wxT("Are you sure to delete file?"), wxT("Question"),
                        wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
    if (dlg.ShowModal() == wxID_NO) {
        return;
    }

    remove(path);

    const auto parent_path = path.parent_path();
    if (fs::is_empty(parent_path)) {
        remove(parent_path);
    }

    UpdateFileBox(listBox);
}

void MainFrame::onPressbtnaddObjectsToItem(wxCommandEvent& event) {
    wxTreeItemId item = treeCtrl->GetFocusedItem();
    if (!item.IsOk() || item == treeCtrl->GetRootItem()) {
        return;
    }

    wxFileDialog openFileDialog(
        this, _("Attach file"), "", "", "", wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
    if (openFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    /* allow add multiple files */
    wxArrayString sourceFileNames;
    openFileDialog.GetPaths(sourceFileNames); // an array of full paths

    vector<fs::path> paths;
    for (const auto& path : sourceFileNames) {
        paths.emplace_back(path.ToStdWstring());
    }

    logic_.CopyFiles(item, paths);

    UpdateFileBox(listBox);
}

void MainFrame::onPressbtnRenameFile(wxCommandEvent& event) {
    const auto selected_item = treeCtrl->GetFocusedItem();
    if (!selected_item.IsOk() || selected_item == treeCtrl->GetRootItem()) {
        return;
    }

    const auto path = GetFilenameFromListbox(selected_item, listBox);
    if (path.empty() || !exists(path)) {
        return;
    }

    wxTextEntryDialog dlg(this, "Enter new name", "Rename", path.filename().wstring());
    if (dlg.ShowModal() != wxID_OK) {
        return;
    }

    const wstring new_name = dlg.GetValue().ToStdWstring();
    if (new_name.empty()) {
        return;
    }

    if (!utils::IsValidFilename(new_name)) {
        wxMessageBox("Incorrect filename", "Warning");
        return;
    }

    fs::rename(path, path.parent_path() / new_name);

    UpdateFileBox(listBox);
}

void MainFrame::onTreeItemRightClick(wxTreeEvent& evt) {
    wxMenu mnu;
    mnu.Append(ID_ADD_ITEM, "Add new item");
    mnu.Append(ID_DUBLICATE_ITEM, "Dublicate item");
    mnu.Append(ID_CUT_ITEM, "Cut item...");
    mnu.Append(ID_DELETE_ITEM, "Delete item...");
    mnu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::onPopupClick), NULL, this);
    PopupMenu(&mnu);
}

void MainFrame::onPopupClick(wxCommandEvent& evt) {
    wxTreeItemId selected_item = treeCtrl->GetFocusedItem();

    wxArrayTreeItemIds selected_items;
    treeCtrl->GetSelections(selected_items);

    switch (evt.GetId()) {
    case ID_ADD_ITEM:
        AppendItems(selected_items, 1);
        break;
    case ID_CUT_ITEM:
        CutItems(selected_items);
        break;
    case ID_DUBLICATE_ITEM:
        DuplicateItem(selected_item);
        break;
    case ID_DELETE_ITEM:
        DeleteItem(selected_item);
        break;
    }
}

void MainFrame::DuplicateItem(wxTreeItemId item) {
    if (item == treeCtrl->GetRootItem()) {
        return;
    }

    const auto& item_info = logic_.GetTreeItemInfo(item);
    const auto parent_item = logic_.GetParentTreeItemPtrById(item_info.id);

    logic_.AppendTreeItem(parent_item, item_info.name, item_info.comment);
}

void MainFrame::Search() {
    const wxString pattern = edtSearch->GetValue();
    const auto found = logic_.Search(pattern.ToStdWstring());
    if (found.empty()) {
        return;
    }

    for (const auto& tree_item : found) {
        expandAllParents(tree_item);
        treeCtrl->SelectItem(tree_item);
        // treeCtrl->SetItemBackgroundColour(tree_item, wxColour(255, 255, 0));
    }
}

void MainFrame::onPressbtnSearch(wxCommandEvent& event) {
    Search();
}
