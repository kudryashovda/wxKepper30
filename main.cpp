// bigger dlg new Item

#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/listbox.h>
#include <wx/numdlg.h>
#include <wx/textfile.h>
#include <wx/treectrl.h>
#include <wx/wx.h>

// Global vars
wxString DATA_PATH = "";
wxString TEMPLATE_PATH = "";

wxString FILE_NAME = "null";         // dataFile location
wxString TEMPLATE_FILENAME = "null"; // template location
/* real filepath is formed in wxFrame costructor */

wxChar DIR_SEPARATOR = wxFileName::GetPathSeparator();

wxString USER = "USER";

wxString VERSION = "21.4a";
bool STATE_TRIAL = false;
wxString TRIAL_DATE = "9-29-2020 23:59:59"; // for trial

/* buttons and edits style */
#if defined(__WINDOWS__)
long MY_STYLE = wxNO_BORDER;
#else
long MY_STYLE = wxSTATIC_BORDER;
#endif

/* elements in popup menu */
#define ID_ADD_ITEM 2001
#define ID_CUT_ITEM 2002
#define ID_DELETE_ITEM 2003
#define ID_EXPORT_BRUNCH 2004
#define ID_IMPORT_BRUNCH 2005
#define ID_RENAME_ITEM 2006
#define ID_MAKE_REPORT 2007
#define ID_DUBLICATE_ITEM 2008
#define ID_TEST 2009
#define ID_SORT 2010
/* elements in popup menu */

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
    wxString user = USER;
    size_t fid = 0; // fixed id
    wxString name = "";
    wxString comments = "";

    wxString getObjPath() {
        return wxString::Format(wxT("%s%s%zu%c"), DATA_PATH, user, fid, DIR_SEPARATOR);
    }

    bool operator==(const sTreeItem& rhs) {
        bool res = (itemId == rhs.itemId && id == rhs.id && pid == rhs.pid && fid == rhs.fid && name == rhs.name && comments == rhs.comments);
        return res;
    }
};

sTreeItem ST_ERROR(NULL, 0, 0, USER, 0, "Error", "Error");
sTreeItem ST_ROOT(NULL, 0, 0, USER, 0, "Root", "");

struct settings {
    wxString dataPath;
    wxString dataFilename;
    wxString templateFileName;
    wxString userName;
} iniSettings;

#define ELEMENTS_IN_FILE_STRING 6

wxTreeItemId DRAGGED_ITEM;

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

IMPLEMENT_APP(MyApp)

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);

private:
    /* define widgets */
    wxPanel *pnl, *pnlCard;

    wxTreeCtrl* treeCtrl;

    wxButton *btnCut, *btnAdd, *btnDel, *btnSaveItemData, *btnSearch, *btnaddObjectsToItem, *btnDelObject,
        *btnRenameObj, *btnNewObject, *btnGoto, *btnPhoto, *btnExport, *btnImport, *btnLink;

    wxStaticText *lblName, *lblComtsName, *lblListName, *lblId;

    wxBoxSizer *borderSizer, *mainSizer, *leftSizer, *rightSizer, *nameBarSizer, *lblsBarSizer, *searchBarSizer,
        *idBarSizer;

    wxGridSizer *gsTopBtns, *gsObjBtns;

    wxTextCtrl *tc, *edtName, *edtId, *edtSearch;

    wxListBox* listBox;

    /* Events */
    void onCreateFrame();
    void OnListRightClick(wxTreeEvent& evt);
    void OnPopupClick(wxCommandEvent& evt);
    void OnBeginDrag(wxTreeEvent& event);
    void OnEndDrag(wxTreeEvent& event);
    void onTreeItemClick(wxCommandEvent& event);
    void onTreeItemStartChanging(wxCommandEvent& event);
    void onPressbtnAdd(wxCommandEvent& event);
    void onPressbtnCut(wxCommandEvent& event);
    void onPressbtnSaveItemData(wxCommandEvent& event);
    void onPressbtnDel(wxCommandEvent& event);
    void onPressbtnExport(wxCommandEvent& event);
    void onPressbtnImport(wxCommandEvent& event);
    void onPressbtnGotoId(wxCommandEvent& event);
    void onPressbtnSearch(wxCommandEvent& event);
    void onPressTCkey(wxKeyEvent& event);

    void onBoxItemDblClick(wxCommandEvent& event);
    void onPressbtnaddObjectsToItem(wxCommandEvent& event);
    void onPressbtnDelObject(wxCommandEvent& event);
    void onPressbtnNewObject(wxCommandEvent& event);
    void onPressbtnRenameObject(wxCommandEvent& event);
    void onPressbtnPhoto(wxCommandEvent& event);
    void onPressBtnLink(wxCommandEvent& event);

    void bindEvents();
    void checkTrial();
    settings getSettingsFromFile(wxString str);
    void appendNewItem(wxTreeItemId& item);
    sTreeItem getTreeItemData(const wxTreeItemId& item);
    void setTreeItemData(const wxTreeItemId& item, const sTreeItem& st);
    void saveBrunch(wxString filename, const wxTreeItemId& item);
    void saveTree();
    void loadTree(wxString filename, const wxTreeItemId& item2connect);
    void getAllItemsData(const wxTreeItemId& srcItem, wxVector<sTreeItem>& vs, size_t& init_id);
    sTreeItem s_tokenizer(wxString str, wxString delim);
    void showTreeItemData(const wxTreeItemId& srcItem);
    size_t getMaxFid();
    void moveAllChilds(const wxTreeItemId& srcItem, const wxTreeItemId& destItem);
    void deleteItems();
    void cut();
    void renameItem(wxTreeItemId& item);
    void importDialogStart();
    void exportDialogStart();
    void exportSelectedBrunch(const wxTreeItemId& item, wxString fn);
    void importSelectedBrunch(const wxTreeItemId& item, wxString fn);
    void addDelimiterString(wxString& string);
    void showItemDataById(size_t itemId);
    void expandAllParents(const wxTreeItemId item);
    void searchStr();
    void dublicate();
    void makeReport();
    void getTextAllChilds(const wxTreeItemId& rootItem, wxString& str, wxString& tab);
    void moveUp();
    void moveDown();
    void sortItems(const wxTreeItemId& rootItem);
    void saveTextToItem(const wxTreeItemId item);

    void addObjectsToItem(const wxTreeItemId& item);
    void delObject(const wxTreeItemId& item, wxString objName);
    void renameObject(const wxTreeItemId& item, wxString objName, wxString newObjectName);
    void createNewObjectFile(const wxTreeItemId& item, wxString objName);
    void takePhoto();
};

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

sTreeItem MainFrame::getTreeItemData(const wxTreeItemId& item) {
    MyTreeItemData* data = (MyTreeItemData*)treeCtrl->GetItemData(item);

    if (data == nullptr)
        return ST_ERROR;

    return data->GetStruct();
}

void MainFrame::setTreeItemData(const wxTreeItemId& item, const sTreeItem& st) {
    treeCtrl->SetItemData(item, new MyTreeItemData(st));
    treeCtrl->SetItemText(item, st.name);
}

/* class to store item's data inside treectrl */

/* It is a small dialog when Add new sample is pressed */
/* The dialog makes it possible to add multiple items with one content */
class DlgAppendItem : public wxDialog {
public:
    DlgAppendItem(wxWindow* parent, wxWindowID id, const wxString& title);

    wxTextCtrl *dlgEdtText, *dlgComments;
    wxButton* btnDate;
    wxComboBox* cbbTemplate;

    void onPressBtnDate(wxCommandEvent& event) {
        event.Skip(true);

        wxDateTime now = wxDateTime::Now();
        wxString str = now.Format(wxT("%d-%m-%Y_%H-%M-%S"), wxDateTime::Local);
        dlgEdtText->SetValue(str);
    }

    /* Template */
    struct str_templ {
        wxString name;
        wxString content;
    };

    wxVector<str_templ> vstr;

    void loadTemplateFromFile(wxString fn, wxVector<str_templ>& vstr) {
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

    void fillTemplateBox() {
        loadTemplateFromFile(TEMPLATE_FILENAME, vstr);

        for (auto it : vstr)
            cbbTemplate->Append(it.name);

        if (cbbTemplate->GetCount() > 0)
            cbbTemplate->Select(0);
    };

    void onSelectTemplate(wxCommandEvent& event) {
        event.Skip(true);

        int pos = cbbTemplate->GetSelection();

        dlgComments->SetValue(vstr[pos].content);
    }
};
/* It is a small dialog when Add new sample is pressed */

/* It is an implementation of a small dialog when Add new sample is pressed */
/* The dialog makes it possible to add multiple items with one content */
DlgAppendItem::DlgAppendItem(wxWindow* parent, wxWindowID id, const wxString& title)
    : wxDialog(parent, id, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER) {
    wxBoxSizer* borderSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* textTemplate = new wxStaticText(this, wxID_ANY, "Template: ");

    cbbTemplate = new wxComboBox(this, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY | MY_STYLE);
    cbbTemplate->Bind(wxEVT_COMMAND_COMBOBOX_SELECTED, &DlgAppendItem::onSelectTemplate, this);

    fillTemplateBox();

    dlgEdtText = new wxTextCtrl(this, wxID_ANY, "Name", wxDefaultPosition, wxDefaultSize, MY_STYLE);
    dlgComments = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(100, 60), wxTE_MULTILINE | MY_STYLE);

    btnDate = new wxButton(this, wxID_ANY, "Date", wxDefaultPosition, wxDefaultSize, MY_STYLE | wxBU_EXACTFIT);
    btnDate->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &DlgAppendItem::onPressBtnDate, this);

    hSizer->Add(dlgEdtText, 1, wxEXPAND, 0);
    hSizer->AddSpacer(5);
    hSizer->Add(btnDate, 0, wxEXPAND, 0);

    vSizer->Add(hSizer, 0, wxEXPAND | wxBOTTOM, 5);

    vSizer->Add(textTemplate);
    vSizer->Add(cbbTemplate, 0, wxEXPAND | wxBOTTOM, 5);
    vSizer->Add(dlgComments, 1, wxEXPAND | wxBOTTOM, 5);

    vSizer->Add(CreateButtonSizer(wxYES | wxCANCEL | MY_STYLE), 0, wxCENTRE, 0);

    borderSizer->Add(vSizer, 1, wxEXPAND | wxALL, 10);

    SetSizer(borderSizer);

    borderSizer->Fit(this);

    wxSize ws = this->GetSize();
    ws.SetHeight(ws.GetHeight() * 2);
    ws.SetWidth(ws.GetWidth() * 2);

    SetMinSize(ws); // linux
    //this->SetMinClientSize(ws);
    SetClientSize(ws); //ClientToWindowSize(ws));

    dlgEdtText->SetFocus();
    dlgEdtText->SelectAll();
}
/* It is an implementation of a small dialog when Add new sample is pressed */

bool MyApp::OnInit() {
    wxSetlocale(LC_ALL, "");

    wxString trialStr = "";
    if (STATE_TRIAL)
        trialStr = " (Trial version. Works until " + TRIAL_DATE + ")";

    wxString progName = "KEEP version " + VERSION + trialStr;
    MainFrame* frame = new MainFrame(progName);
    frame->Show();

    frame->SetPosition(wxPoint(0, 0));
    frame->Center(wxHORIZONTAL);

    return true;
}

void MainFrame::checkTrial() {
    wxDateTime dt_deadline;
    dt_deadline.ParseDateTime(TRIAL_DATE);

    wxDateTime now = wxDateTime::Now();

    if (now < dt_deadline)
        STATE_TRIAL = false;
    else
        STATE_TRIAL = true;
}

MainFrame::MainFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title) {

#if defined(__WINDOWS__)
    SetIcon(wxICON(icon)); // windows only. icon file mt be located
#endif

    wxString iniFileLocation = wxGetCwd() + DIR_SEPARATOR + "settings.ini";

    iniSettings = getSettingsFromFile(iniFileLocation);

    DATA_PATH = iniSettings.dataPath;
    USER = iniSettings.userName;

    FILE_NAME = DATA_PATH + iniSettings.dataFilename;
    TEMPLATE_FILENAME = DATA_PATH + iniSettings.templateFileName;

    pnl = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER | MY_STYLE);

    treeCtrl = new wxTreeCtrl(pnl, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                              wxTR_DEFAULT_STYLE | wxTR_MULTIPLE | MY_STYLE, wxDefaultValidator, wxTreeCtrlNameStr);

    edtSearch = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | MY_STYLE,
                               wxDefaultValidator, wxTextCtrlNameStr);
    btnSearch = new wxButton(pnl, wxID_ANY, "Search", wxDefaultPosition, wxDefaultSize, MY_STYLE);

    searchBarSizer = new wxBoxSizer(wxHORIZONTAL);
    searchBarSizer->Add(edtSearch, 1, wxEXPAND, 0);
    searchBarSizer->Add(btnSearch, 0, wxEXPAND, 0);

    btnCut = new wxButton(pnl, wxID_ANY, "Cut", wxDefaultPosition, wxDefaultSize, MY_STYLE);
    btnAdd = new wxButton(pnl, wxID_ANY, "Add", wxDefaultPosition, wxDefaultSize, MY_STYLE);
    btnDel = new wxButton(pnl, wxID_ANY, "Delete", wxDefaultPosition, wxDefaultSize, MY_STYLE);
    btnExport = new wxButton(pnl, wxID_ANY, "Export", wxDefaultPosition, wxDefaultSize, MY_STYLE);
    btnImport = new wxButton(pnl, wxID_ANY, "Import", wxDefaultPosition, wxDefaultSize, MY_STYLE);

    btnGoto = new wxButton(pnl, wxID_ANY, "Goto ID", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | MY_STYLE,
                           wxDefaultValidator, wxTextCtrlNameStr);

    gsTopBtns = new wxGridSizer(1, 5, 1, 1);
    gsTopBtns->Add(btnCut, 1, wxEXPAND, 0);
    gsTopBtns->Add(btnAdd, 1, wxEXPAND, 0);
    gsTopBtns->Add(btnDel, 1, wxEXPAND, 0);
    gsTopBtns->Add(btnExport, 1, wxEXPAND, 0);
    gsTopBtns->Add(btnImport, 1, wxEXPAND, 0);

    lblName = new wxStaticText(pnl, wxID_ANY, "Name:");
    edtName = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | MY_STYLE,
                             wxDefaultValidator, wxTextCtrlNameStr);

    nameBarSizer = new wxBoxSizer(wxVERTICAL);
    nameBarSizer->Add(gsTopBtns, 0, wxEXPAND, 0);
    nameBarSizer->AddSpacer(5);
    nameBarSizer->Add(lblName, 0, wxEXPAND, 0);
    nameBarSizer->Add(edtName, 0, wxEXPAND, 0);

    lblId = new wxStaticText(pnl, wxID_ANY, "");
    edtId = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY | MY_STYLE,
                           wxDefaultValidator, wxTextCtrlNameStr);
    edtId->SetBackgroundColour(*wxWHITE);
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
                        wxTE_MULTILINE | MY_STYLE, // wxTE_RICH
                        wxDefaultValidator, wxTextCtrlNameStr);

    lblListName = new wxStaticText(pnl, wxID_ANY, "Attached objects", wxDefaultPosition, wxDefaultSize, MY_STYLE);
    btnNewObject = new wxButton(pnl, wxID_ANY, "New", wxDefaultPosition, wxDefaultSize, MY_STYLE);
    btnaddObjectsToItem = new wxButton(pnl, wxID_ANY, "Add", wxDefaultPosition, wxDefaultSize, MY_STYLE);
    btnDelObject = new wxButton(pnl, wxID_ANY, "Delete", wxDefaultPosition, wxDefaultSize, MY_STYLE);
    btnRenameObj = new wxButton(pnl, wxID_ANY, "Rename", wxDefaultPosition, wxDefaultSize, MY_STYLE);
    btnPhoto = new wxButton(pnl, wxID_ANY, "Photo", wxDefaultPosition, wxDefaultSize, MY_STYLE);
    btnLink = new wxButton(pnl, wxID_ANY, "Link to...", wxDefaultPosition, wxDefaultSize, MY_STYLE);

    gsObjBtns = new wxGridSizer(1, 6, 1, 1);

    gsObjBtns->Add(btnNewObject, 1, wxEXPAND, 0);
    gsObjBtns->Add(btnaddObjectsToItem, 1, wxEXPAND, 0);
    gsObjBtns->Add(btnDelObject, 1, wxEXPAND, 0);
    gsObjBtns->Add(btnRenameObj, 1, wxEXPAND, 0);
    gsObjBtns->Add(btnPhoto, 1, wxEXPAND, 0);
    gsObjBtns->Add(btnLink, 1, wxEXPAND, 0);

    listBox = new wxListBox(pnl, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, MY_STYLE);

    btnSaveItemData = new wxButton(pnl, wxID_ANY, "Save", wxDefaultPosition, wxDefaultSize, MY_STYLE);

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

    wxSize ws = this->GetSize();
    ws.SetHeight(ws.GetWidth() / 4 * 3);

    SetMinSize(ws);

    onCreateFrame();
}

void MainFrame::bindEvents() {
    treeCtrl->Bind(wxEVT_TREE_ITEM_MENU, &MainFrame::OnListRightClick, this);
    treeCtrl->Bind(wxEVT_TREE_BEGIN_DRAG, &MainFrame::OnBeginDrag, this);
    treeCtrl->Bind(wxEVT_TREE_END_DRAG, &MainFrame::OnEndDrag, this);
    treeCtrl->Bind(wxEVT_TREE_SEL_CHANGED, &MainFrame::onTreeItemClick, this);
    treeCtrl->Bind(wxEVT_TREE_SEL_CHANGING, &MainFrame::onTreeItemStartChanging, this);

    treeCtrl->Bind(wxEVT_KEY_DOWN, &MainFrame::onPressTCkey, this);

    btnAdd->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnAdd, this);
    btnCut->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnCut, this);
    btnSaveItemData->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnSaveItemData, this);
    btnDel->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnDel, this);
    btnExport->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnExport, this);
    btnImport->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnImport, this);
    btnGoto->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnGotoId, this);
    btnSearch->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnSearch, this);

    listBox->Bind(wxEVT_LISTBOX_DCLICK, &MainFrame::onBoxItemDblClick, this);
    btnNewObject->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnNewObject, this);
    btnaddObjectsToItem->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnaddObjectsToItem, this);
    btnDelObject->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnDelObject, this);
    btnRenameObj->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnRenameObject, this);
    btnPhoto->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressbtnPhoto, this);
    btnLink->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::onPressBtnLink, this);
}

void MainFrame::onCreateFrame() {
    bindEvents();

    wxTreeItemId rootItem = treeCtrl->AddRoot("Root");
    ST_ROOT.itemId = rootItem;
    ST_ROOT.user = USER;
    setTreeItemData(rootItem, ST_ROOT);

    loadTree(FILE_NAME, rootItem);
    treeCtrl->Expand(rootItem);
    treeCtrl->SetFocusedItem(rootItem);
}

// Popup menu start
void MainFrame::OnListRightClick(wxTreeEvent& evt) {
    wxMenu mnu;
    mnu.Append(ID_ADD_ITEM, "Add new item");
    mnu.Append(ID_DUBLICATE_ITEM, "Dublicate item");
    mnu.Append(ID_CUT_ITEM, "Cut item...");
    mnu.Append(ID_DELETE_ITEM, "Delete item...");
    mnu.Append(ID_RENAME_ITEM, "Rename item...");
    mnu.Append(ID_SORT, "Sort...");
    mnu.Append(ID_EXPORT_BRUNCH, "Export brunch...");
    mnu.Append(ID_IMPORT_BRUNCH, "Import brunch...");
    mnu.Append(ID_MAKE_REPORT, "Make report...");
    mnu.Append(ID_TEST, "Move item up...");
    mnu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnPopupClick), NULL, this);
    PopupMenu(&mnu);
}

void MainFrame::OnPopupClick(wxCommandEvent& evt) {
    wxTreeItemId item = treeCtrl->GetFocusedItem();

    switch (evt.GetId()) {
    case ID_ADD_ITEM:
        appendNewItem(item);
        break;
    case ID_DUBLICATE_ITEM:
        dublicate();
        break;
    case ID_DELETE_ITEM:
        deleteItems();
        break;
    case ID_RENAME_ITEM:
        renameItem(item);
        break;
    case ID_EXPORT_BRUNCH:
        exportDialogStart();
        break;
    case ID_IMPORT_BRUNCH:
        importDialogStart();
        break;
    case ID_CUT_ITEM:
        cut();
        break;
    case ID_MAKE_REPORT:
        makeReport();
        break;
    case ID_SORT:
        sortItems(item);
        break;
    }
}
// Popup menu end

void MainFrame::appendNewItem(wxTreeItemId& item) {
    wxArrayTreeItemIds tvi;
    size_t size = treeCtrl->GetSelections(tvi);
    if (size == 0)
        return;

    DlgAppendItem dlg(this, wxID_ANY, "Add object");
    if (dlg.ShowModal() == wxID_CANCEL)
        return;

    sTreeItem st;
    st.name = dlg.dlgEdtText->GetValue();
    st.comments = dlg.dlgComments->GetValue();

    if (st.name == "")
        return;

    size_t maxId = getMaxFid();

    for (auto& it : tvi) {
        wxTreeItemId newItem = treeCtrl->AppendItem(it, st.name);
        st.itemId = newItem;
        st.fid = ++maxId;
        setTreeItemData(newItem, st);

        treeCtrl->Expand(it);
        treeCtrl->SelectItem(it, false);
        treeCtrl->SetFocusedItem(newItem);
        showTreeItemData(newItem);
    }

    saveTree();
}

void MainFrame::saveBrunch(wxString filename, const wxTreeItemId& item) {
    wxVector<sTreeItem> v;

    size_t init_id = 0;
    getAllItemsData(item, v, init_id);

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

void MainFrame::getAllItemsData(const wxTreeItemId& srcItem, wxVector<sTreeItem>& vs, size_t& init_id) {
    wxTreeItemIdValue cookie;

    sTreeItem ss;
    ss = getTreeItemData(srcItem);

    ss.id = vs.size();
    ss.pid = init_id;

    vs.push_back(ss);

    wxTreeItemId srcChild = treeCtrl->GetFirstChild(srcItem, cookie);
    while (srcChild.IsOk()) {
        getAllItemsData(srcChild, vs, ss.id); // danger - recursia
        srcChild = treeCtrl->GetNextChild(srcItem, cookie);
    }
}

void MainFrame::OnBeginDrag(wxTreeEvent& event) {
    DRAGGED_ITEM = event.GetItem();

    if (DRAGGED_ITEM == treeCtrl->GetRootItem())
        return;

    event.Allow();
}

void MainFrame::OnEndDrag(wxTreeEvent& event) {
    wxMessageDialog* dlg = new wxMessageDialog(
        NULL, wxT("Are you sure to transfer item?"), wxT("Question"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

    if (dlg->ShowModal() == wxID_NO) {
        dlg->Destroy();
        return;
    }
    dlg->Destroy();

    wxTreeItemId itemSrc = DRAGGED_ITEM, itemDst = event.GetItem();

    if (itemSrc == itemDst)
        return;
    if (!itemDst.IsOk())
        return;

    moveAllChilds(itemSrc, itemDst);
    treeCtrl->DeleteChildren(itemSrc);
    treeCtrl->Delete(itemSrc);

    saveTree();

    wxTreeItemIdValue cookie;
    treeCtrl->Expand(itemDst);
    treeCtrl->SetFocusedItem(treeCtrl->GetFirstChild(itemDst, cookie));
}

void MainFrame::loadTree(wxString filename, const wxTreeItemId& item2connect) {

    wxVector<sTreeItem> vs;
    wxTextFile tfile(filename);
    if (!tfile.Exists())
        return;

    tfile.Open();

    for (size_t i = 1, s = tfile.GetLineCount(); i < s; ++i) {
        sTreeItem sti = s_tokenizer(tfile[i], '\t');
        if (sti == ST_ERROR)
            continue;
        vs.push_back(sti);
    }

    tfile.Close();

    if (vs.size() == 0)
        return;

    for (auto& it : vs) {
        it.itemId = treeCtrl->AppendItem(item2connect, it.name);
        setTreeItemData(it.itemId, it);
    }

    for (auto& it : vs) {      // pid
        for (auto& it2 : vs) { // id
            if (it.pid == it2.id) {
                treeCtrl->Delete(it.itemId);
                it.itemId = treeCtrl->AppendItem(it2.itemId, it.name);
                setTreeItemData(it.itemId, it);
            }
        }
    }
}

sTreeItem MainFrame::s_tokenizer(wxString str, wxString delim) {
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

    if (vs.size() != ELEMENTS_IN_FILE_STRING)
        return ST_ERROR;

    sTreeItem st(NULL, wxAtoi(vs[0]), wxAtoi(vs[1]), vs[2], wxAtoi(vs[3]), vs[4], vs[5]);

    return st;
}

void MainFrame::showTreeItemData(const wxTreeItemId& item) {
    sTreeItem sti = getTreeItemData(item);
    if (sti == ST_ERROR)
        return;

    wxString str = wxString::Format(wxT("%s%s%zu"), "ID: ", sti.user, sti.fid);
    edtId->SetValue(str);
    edtName->ChangeValue(sti.name);

    sti.comments.Replace("/n", "\n"); // danger - real info corrupt
    tc->ChangeValue(sti.comments);

    /* show objects attached */
    wxString itemDirPath = sti.getObjPath();

    listBox->Clear();
    if (!wxDirExists(itemDirPath))
        return;

    wxArrayString files;
    wxDir::GetAllFiles(itemDirPath, &files); // get full file path

    for (auto it : files) {
        wxString f = it.AfterLast(DIR_SEPARATOR); // show only filename
        listBox->Append(f);
    }

    if (listBox->GetCount() > 0)
        listBox->Select(0);
}

void MainFrame::saveTextToItem(const wxTreeItemId item) {
    sTreeItem st = getTreeItemData(item);

    st.name = edtName->GetValue();
    st.comments = tc->GetValue();
    if (st.name == "")
        return;

    setTreeItemData(item, st);

    saveTree();

    tc->SetModified(false);
    edtName->SetModified(false);
}

void MainFrame::onTreeItemStartChanging(wxCommandEvent& event) {
    wxTreeItemId item = treeCtrl->GetFocusedItem();
    if (!item.IsOk())
        return;

    if (tc->IsModified() or edtName->IsModified()) {
        saveTextToItem(item);
    }
}

void MainFrame::onTreeItemClick(wxCommandEvent& event) {
    wxTreeItemId item = treeCtrl->GetFocusedItem();

    showTreeItemData(item);
}

void MainFrame::onPressbtnSaveItemData(wxCommandEvent& event) {
    wxTreeItemId item = treeCtrl->GetFocusedItem();
    if (!item.IsOk())
        return;

    saveTextToItem(item);
}

size_t MainFrame::getMaxFid() {
    size_t maxFid = 0;
    size_t init_id = 0;

    wxVector<sTreeItem> v;
    getAllItemsData(treeCtrl->GetRootItem(), v, init_id);

    for (auto it : v)
        if (it.fid > maxFid && it.user == USER)
            maxFid = it.fid;

    return maxFid;
}

void MainFrame::moveAllChilds(const wxTreeItemId& srcItem, const wxTreeItemId& destItem) {
    sTreeItem ss = getTreeItemData(srcItem);
    sTreeItem sd = getTreeItemData(destItem);

    wxTreeItemId destChild = treeCtrl->AppendItem(destItem, ss.name);
    ss.pid = sd.id;
    setTreeItemData(destChild, ss);

    wxTreeItemIdValue cookie;
    wxTreeItemId srcChild = treeCtrl->GetFirstChild(srcItem, cookie);
    while (srcChild.IsOk()) {
        moveAllChilds(srcChild, destChild); // danger - recursia
        srcChild = treeCtrl->GetNextChild(srcItem, cookie);
    }
}

void MainFrame::deleteItems() {
    wxArrayTreeItemIds tvi;
    size_t size = treeCtrl->GetSelections(tvi);
    if (size == 0)
        return;

    /* get parent of the first item being deleted and show it after the deleting*/
    wxTreeItemId firstItemParent = treeCtrl->GetItemParent(tvi[0]);

    wxMessageDialog* dlgItem = new wxMessageDialog(
        NULL, wxT("Are you sure to delete item?"), wxT("Question"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
    if (dlgItem->ShowModal() == wxID_NO) {
        dlgItem->Destroy();
        return;
    }
    dlgItem->Destroy();

    for (auto it : tvi) {
        if (it == treeCtrl->GetRootItem())
            return;
        if (treeCtrl->ItemHasChildren(it))
            return;

        sTreeItem s = getTreeItemData(it);
        wxString fullDir = s.getObjPath();

        if (wxFileName::DirExists(fullDir)) {
            wxMessageDialog* dlgObj = new wxMessageDialog(NULL, wxT("Are you sure to delete also all objects attached to the Item?"),
                                                          wxT("Objects"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
            if (dlgObj->ShowModal() == wxID_NO) {
                dlgObj->Destroy();
                return;
            } else {
                dlgObj->Destroy();
                wxDir::Remove(fullDir, wxPATH_RMDIR_RECURSIVE);
            }
        }

        treeCtrl->Delete(it);
    }

    showTreeItemData(firstItemParent);
    treeCtrl->SelectItem(firstItemParent);

    saveTree();
}

void MainFrame::onPressbtnDel(wxCommandEvent& event) {
    deleteItems();
}

void MainFrame::cut() {
    wxTreeItemId item = treeCtrl->GetFocusedItem();
    if (!item.IsOk())
        return;

    wxNumberEntryDialog dlgCut(this, "Cut the sample into", "Number of samples", "Cut", 1, 0, 100);
    if (dlgCut.ShowModal() != wxID_OK)
        return;
    size_t ni = dlgCut.GetValue();

    // show additional info dialog
    DlgAppendItem dlgInfo(this, wxID_ANY, "Cut details");
    if (dlgInfo.ShowModal() == wxID_CANCEL)
        return;
    wxString prefix = dlgInfo.dlgEdtText->GetValue();
    wxString comments = dlgInfo.dlgComments->GetValue();

    if (prefix == "")
        return;

    sTreeItem s_parent = getTreeItemData(item);
    size_t maxId = getMaxFid();

    wxTreeItemId addedItem;
    for (size_t i = 0; i < ni; ++i) {
        wxString name = wxString::Format(wxT("%s%zu"), prefix, (i + 1));

        addedItem = treeCtrl->AppendItem(item, name);
        size_t nextId = ++maxId;
        sTreeItem s_child(addedItem, nextId, s_parent.id, USER, nextId, name, comments);

        setTreeItemData(addedItem, s_child);
    }

    saveTree();

    treeCtrl->Expand(item);
}

void MainFrame::onPressbtnExport(wxCommandEvent& event) {
    exportDialogStart();
}

void MainFrame::exportDialogStart() {
    wxTreeItemId item = treeCtrl->GetFocusedItem();
    if (!item.IsOk())
        return;

    wxFileDialog* saveBrunchDialog = new wxFileDialog(this, _("Save as..."), wxEmptyString, wxEmptyString, "",
                                                      wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);

    if (saveBrunchDialog->ShowModal() != wxID_OK)
        return;

    wxString fn = saveBrunchDialog->GetPath();

    exportSelectedBrunch(item, fn);

    wxMessageBox("Selected brunch was successfully exported");
}

void MainFrame::exportSelectedBrunch(const wxTreeItemId& item, wxString fn) {

    wxVector<sTreeItem> vs;

    size_t init_id = 0; // first id in exported file

    getAllItemsData(item, vs, init_id);

    saveBrunch(fn, item);
}

void MainFrame::onPressbtnImport(wxCommandEvent& event) {
    importDialogStart();
}

void MainFrame::importDialogStart() {
    wxTreeItemId item = treeCtrl->GetFocusedItem();
    if (!item.IsOk())
        return;

    wxFileDialog* openBrunchDialog = new wxFileDialog(
        this, _("Select file..."), wxEmptyString, wxEmptyString, "", wxFD_OPEN, wxDefaultPosition);

    if (openBrunchDialog->ShowModal() != wxID_OK)
        return;

    wxString fn = openBrunchDialog->GetPath();

    importSelectedBrunch(item, fn);
}

void MainFrame::importSelectedBrunch(const wxTreeItemId& item, wxString fn) {
    loadTree(fn, item);
    saveTree();
}

void MainFrame::saveTree() {
    saveBrunch(FILE_NAME, treeCtrl->GetRootItem());
}

void MainFrame::onPressbtnAdd(wxCommandEvent& event) {
    wxTreeItemId item = treeCtrl->GetFocusedItem();
    appendNewItem(item);
}

void MainFrame::renameItem(wxTreeItemId& item) {
    sTreeItem st = getTreeItemData(item);

    wxTextEntryDialog dlg(this, "Enter new name", "Rename", st.name);
    if (dlg.ShowModal() != wxID_OK)
        return;

    st.name = dlg.GetValue();

    if (st.name == "")
        return;

    setTreeItemData(item, st);
    showTreeItemData(item);

    saveTree();
}

settings MainFrame::getSettingsFromFile(wxString fn) {
    settings iniSettings;

    iniSettings.dataPath = wxGetCwd() + DIR_SEPARATOR;
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

void MainFrame::onBoxItemDblClick(wxCommandEvent& event) {
    wxTreeItemId item = treeCtrl->GetFocusedItem();
    if (!item.IsOk())
        return;

    sTreeItem st = getTreeItemData(item);

    size_t sel = event.GetSelection();
    wxString f = listBox->GetString(sel);

    wxString fullPath = st.getObjPath() + f;
    addDelimiterString(fullPath); // works with spaces on Linux

    /* To enable open files and folder with non-latin chars add wxSetlocale(LC_ALL, "") to to Init foo */
    if (wxLaunchDefaultApplication(fullPath))
        return;

    wxMessageBox("No files found on disk", "Warning");
}

void MainFrame::addDelimiterString(wxString& string) {
    if (!string.StartsWith(wxT("\""))) {
        string.Prepend(wxT("\""));
        string.Append(wxT("\""));
    }
}

void MainFrame::onPressbtnRenameObject(wxCommandEvent& event) {
    if (listBox->IsEmpty())
        return;

    wxTreeItemId item = treeCtrl->GetFocusedItem();

    size_t selPosition = listBox->GetSelection();

    wxString objName = listBox->GetString(selPosition);

    wxTextEntryDialog dlg(this, "Enter new name", "Rename", objName);
    if (dlg.ShowModal() != wxID_OK)
        return;

    wxString newObjectName = dlg.GetValue();
    if (newObjectName == "")
        return;

    renameObject(item, objName, newObjectName);
}

void MainFrame::renameObject(const wxTreeItemId& item, wxString objName, wxString newObjectName) {
    sTreeItem s = getTreeItemData(item);

    wxString oldFullPath = s.getObjPath() + objName;
    wxString newFullPath = s.getObjPath() + newObjectName;

    if (wxFileExists(oldFullPath))
        wxRenameFile(oldFullPath, newFullPath);

    showTreeItemData(item);
}

void MainFrame::onPressbtnaddObjectsToItem(wxCommandEvent& event) {
    wxTreeItemId item = treeCtrl->GetFocusedItem();
    if (!item.IsOk())
        return;

    addObjectsToItem(item);
}

void MainFrame::onPressbtnNewObject(wxCommandEvent& event) {
    wxTreeItemId item = treeCtrl->GetFocusedItem();
    if (!item.IsOk())
        return;

    wxTextEntryDialog dlg(this, "Enter file name", "New file", "newfile.txt");
    if (dlg.ShowModal() != wxID_OK)
        return;

    createNewObjectFile(item, dlg.GetValue());

    showTreeItemData(item);
}

void MainFrame::createNewObjectFile(const wxTreeItemId& item, wxString objName) {
    if (!item.IsOk())
        return;

    sTreeItem s = getTreeItemData(item);

    wxString fullDir = s.getObjPath();
    wxString fullPath = fullDir + objName;

    if (!wxFileName::DirExists(fullDir))
        wxFileName::Mkdir(fullDir);

    wxFile f;
    f.Create(fullPath, true, wxS_DEFAULT);
    f.Close();
}

void MainFrame::addObjectsToItem(const wxTreeItemId& item) {
    if (!item.IsOk())
        return;

    wxFileDialog openFileDialog(
        this, _("Attach file"), "", "", "", wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    /* allow add multiple files */
    wxArrayString sourceFileNames;
    openFileDialog.GetPaths(sourceFileNames); // an array of full paths

    /* set folder name based on id */
    sTreeItem s = getTreeItemData(item);
    wxString destDir = s.getObjPath(); // ends with '/' in Linux

    if (!wxFileName::DirExists(destDir))
        wxFileName::Mkdir(destDir);

    for (auto it : sourceFileNames) {
        wxFileName fn(it);

        wxString destFile = destDir + fn.GetFullName(); // last foo returns filename
        wxCopyFile(it, destFile);
    }

    showTreeItemData(item);
}

void MainFrame::onPressbtnDelObject(wxCommandEvent& event) {
    wxTreeItemId item = treeCtrl->GetFocusedItem();
    if (!item.IsOk())
        return;

    if (listBox->IsEmpty())
        return;

    wxMessageDialog* dlg = new wxMessageDialog(NULL, wxT("Are you sure to delete object?"), wxT("Question"),
                                               wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
    if (dlg->ShowModal() == wxID_NO) {
        dlg->Destroy();
        return;
    }

    dlg->Destroy();

    size_t selPosition = listBox->GetSelection();
    wxString objName = listBox->GetString(selPosition);

    delObject(item, objName);
}

void MainFrame::delObject(const wxTreeItemId& item, wxString objName) {
    if (!item.IsOk())
        return;

    sTreeItem s = getTreeItemData(item);

    wxString fullPath = s.getObjPath() + objName;

    if (wxFileExists(fullPath))
        wxRemoveFile(fullPath);

    showTreeItemData(item);
}

void MainFrame::onPressBtnLink(wxCommandEvent& event) {
    event.Skip();
    // link();
}

void MainFrame::onPressbtnPhoto(wxCommandEvent& event) {
    takePhoto();
}

void MainFrame::takePhoto() {

#if not defined(__WXMSW__)
    wxMessageBox("Sorry, in Linux this function is not yet implemented", "Attention");
#endif

#if defined(__WXMSW__)

    if (!wxFile::Exists("ffmpeg.exe") or !wxFile::Exists("ffplay.exe")) {
        wxMessageBox("FFMPEG or FFPLAY not found", "Attention");
        return;
    }

    wxString cmd0 = "ffmpeg -hide_banner -list_devices true -f dshow -i dummy";

    wxArrayString output;
    wxArrayString errors;
    wxExecute(cmd0, output, errors, wxEXEC_BLOCK);

    wxString name = errors[1];

    if (name.Find("none found") != wxNOT_FOUND) {
        wxMessageBox("Camera not found", "Attention");
        return;
    }

    wxString d = name.BeforeLast(wxT('\"')); // trick [dshow @ 00000000006d24c0]  "A4tech USB2.0 Camera"
    wxString camName = d.AfterLast(wxT('\"'));

    wxString cmd1 = "ffmpeg -hide_banner -f dshow -i video=\"" + camName + "\" -r 10 -q 2 -f mjpeg udp://127.0.0.1:8080 -loglevel quiet";
    wxString cmd2 = "ffplay -hide_banner -left 0 -top 0 udp://127.0.0.1:8080 -loglevel quiet";

    long proc1_id = wxExecute(cmd1, wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
    long proc2_id = wxExecute(cmd2, wxEXEC_ASYNC);

    wxMessageDialog* dlg = new wxMessageDialog(NULL, wxT("Please wait a video to be opened\nAre you sure to take photo?"),
                                               wxT("Question"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

    bool res = dlg->ShowModal() == wxID_YES;
    dlg->Destroy();

    wxKill(proc2_id, wxSIGKILL);
    wxKill(proc1_id, wxSIGKILL);

    if (!res)
        return;

    wxTreeItemId item = treeCtrl->GetFocusedItem();
    sTreeItem s = getTreeItemData(item);

    wxDateTime now = wxDateTime::Now();
    wxString str_time = now.Format(wxT("%d%m%Y_%H%M%S"), wxDateTime::Local);

    wxString fn = wxString::Format(wxT("%s_%zu_%s.jpg"), "photo", s.id, str_time);

    wxString cmd3 = "ffmpeg -hide_banner -f dshow -i video=\"" + camName + "\" -frames:v 1 -q:v 1 -y " + fn;
    wxExecute(cmd3, wxEXEC_BLOCK);

    wxString destDir = s.getObjPath();
    if (!wxFileName::DirExists(destDir))
        wxFileName::Mkdir(destDir);

    wxString sourceFile = wxString::Format(wxT("%s%c%s"), wxGetCwd(), DIR_SEPARATOR, fn);
    wxString destFile = destDir + fn;

    wxCopyFile(sourceFile, destFile);
    wxRemoveFile(sourceFile);

    showTreeItemData(item);

#endif
}

void MainFrame::onPressbtnGotoId(wxCommandEvent& event) {
    wxNumberEntryDialog dlg(this, "", "Enter ID number", "Goto ID", 0, 0, getMaxFid());
    if (dlg.ShowModal() != wxID_OK)
        return;

    size_t ni = dlg.GetValue();

    showItemDataById(ni);
}

void MainFrame::showItemDataById(size_t itemId) {
    treeCtrl->UnselectAll();

    bool idFound = false;
    sTreeItem found;

    wxVector<sTreeItem> vs;
    size_t init_id = 0;
    getAllItemsData(treeCtrl->GetRootItem(), vs, init_id);

    for (auto it : vs)
        if (it.fid == itemId) {
            idFound = true;
            found = it;
            break;
        }

    if (!idFound) {
        wxMessageBox("ID not found", "Warning");
        return;
    }

    expandAllParents(found.itemId);

    treeCtrl->SelectItem(found.itemId, true);
    treeCtrl->SetFocusedItem(found.itemId);

    showTreeItemData(found.itemId);
}

void MainFrame::expandAllParents(wxTreeItemId item) {
    do {
        if (treeCtrl->ItemHasChildren(item))
            treeCtrl->Expand(item);

        item = treeCtrl->GetItemParent(item);
    } while (item.IsOk());
}

void MainFrame::searchStr() {
    treeCtrl->UnselectAll();

    wxVector<sTreeItem> vs;
    wxTreeItemId rootItem = treeCtrl->GetRootItem();
    size_t init_id = 0;
    getAllItemsData(rootItem, vs, init_id);

    wxString str = edtSearch->GetValue();
    if (str == "")
        return;
    str = str.Lower();

    wxString name, comm;
    for (auto it : vs) {
        name = it.name.Lower();
        comm = it.comments.Lower();
        if ((name.Find(str) != wxNOT_FOUND) || (comm.Find(str) != wxNOT_FOUND)) {
            expandAllParents(it.itemId);
            treeCtrl->SelectItem(it.itemId);
            treeCtrl->SetFocusedItem(it.itemId);

            showTreeItemData(it.itemId);
        }
    }
}

void MainFrame::onPressbtnSearch(wxCommandEvent& event) {
    searchStr();
}

void MainFrame::dublicate() {
    wxTreeItemId item = treeCtrl->GetFocusedItem();
    if (!item.IsOk())
        return;

    wxTreeItemId pItem = treeCtrl->GetItemParent(item);

    sTreeItem s_item = getTreeItemData(item);
    wxTreeItemId newItem = treeCtrl->AppendItem(pItem, s_item.name);

    s_item.itemId = newItem;
    s_item.fid = getMaxFid() + 1;

    setTreeItemData(newItem, s_item);

    treeCtrl->UnselectAll();
    treeCtrl->SelectItem(newItem, true);
    treeCtrl->SetFocusedItem(newItem);
    showTreeItemData(newItem);

    saveTree();
}

void MainFrame::makeReport() {
    wxArrayTreeItemIds tvi;
    size_t size = treeCtrl->GetSelections(tvi);
    if (size == 0)
        return;

    wxFileDialog* saveReportDialog = new wxFileDialog(this, _("Save as..."), wxEmptyString, "report.txt", "",
                                                      wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);

    if (saveReportDialog->ShowModal() != wxID_OK)
        return;

    wxString fn = saveReportDialog->GetPath();

    wxTextFile tfile(fn); // add date
    if (not tfile.Exists())
        tfile.Create();
    tfile.Open();
    tfile.Clear();

    tfile.AddLine("Report"); // add date
    tfile.AddLine("");

    for (auto it : tvi) {
        wxString str = "";
        wxString tab = "";
        getTextAllChilds(it, str, tab);
        tfile.AddLine(str);
        tfile.AddLine("");
    }

    tfile.Write();
    tfile.Close();

    wxMessageBox("The report was saved");
}

/* Danger! Recursia */
void MainFrame::getTextAllChilds(const wxTreeItemId& rootItem, wxString& str, wxString& tab) {
    wxTreeItemIdValue cookie;
    sTreeItem s = getTreeItemData(rootItem);

    // wxString comments = s.comments;
    s.comments.Replace("/n", '\n' + tab + '\t');
    // s.comments.Prepend(tab);

    // str += tab + "'--" + s.name + "(id=" + std::to_string(s.id) +  ')' + '\n' + tab + s.comments + '\n';
    str += tab + "|--" + s.name + "(id=" + std::to_string(s.id) + ')' + '\t' + s.comments + '\n';

    wxTreeItemId rootChild = treeCtrl->GetFirstChild(rootItem, cookie);
    while (rootChild.IsOk()) {
        tab += '\t';
        getTextAllChilds(rootChild, str, tab); // danger - recursia
        rootChild = treeCtrl->GetNextChild(rootItem, cookie);
    }
    tab.RemoveLast();
}

void MainFrame::onPressTCkey(wxKeyEvent& event) {
    wxTreeItemId item = treeCtrl->GetFocusedItem();

    const int d_keyCode = 68;
    const int a_keyCode = 65;
    const int u_keyCode = 85;
    const int i_keyCode = 73;

    if (event.GetKeyCode() == d_keyCode && event.GetModifiers() == wxACCEL_CTRL) {
        dublicate();
    }

    if (event.GetKeyCode() == WXK_DELETE) {
        deleteItems();
    }

    if (event.GetKeyCode() == a_keyCode && event.GetModifiers() == wxACCEL_CTRL) {
        appendNewItem(item);
    }

    if (event.GetKeyCode() == u_keyCode && event.GetModifiers() == wxACCEL_CTRL) {
        moveUp();
    }

    if (event.GetKeyCode() == i_keyCode && event.GetModifiers() == wxACCEL_CTRL) {
        moveDown();
    }

    event.Skip();
}

void MainFrame::moveUp() {
    wxTreeItemId item = treeCtrl->GetFocusedItem();
    sTreeItem ss = getTreeItemData(item);

    wxTreeItemId pItem = treeCtrl->GetItemParent(item);

    wxTreeItemId prevSibItem = treeCtrl->GetPrevSibling(item);
    if (!prevSibItem.IsOk())
        return;

    sTreeItem spsi = getTreeItemData(prevSibItem);

    wxTreeItemId newItem = treeCtrl->InsertItem(pItem, item, spsi.name);
    setTreeItemData(newItem, spsi);

    wxString temporaryFile = "tmp.txt";
    exportSelectedBrunch(prevSibItem, temporaryFile);
    treeCtrl->Delete(prevSibItem);
    importSelectedBrunch(newItem, temporaryFile);
}

void MainFrame::moveDown() {
    wxTreeItemId item = treeCtrl->GetFocusedItem();
    sTreeItem ss = getTreeItemData(item);

    wxTreeItemId pItem = treeCtrl->GetItemParent(item);

    wxTreeItemId nextSibItem = treeCtrl->GetNextSibling(item);
    if (!nextSibItem.IsOk())
        return;

    sTreeItem spsi = getTreeItemData(nextSibItem);

    wxTreeItemId newItem = treeCtrl->InsertItem(pItem, nextSibItem, ss.name);
    setTreeItemData(newItem, ss);

    wxString temporaryFile = "tmp.txt";
    exportSelectedBrunch(item, temporaryFile);
    treeCtrl->Delete(item);

    importSelectedBrunch(newItem, temporaryFile);

    treeCtrl->SetFocusedItem(newItem);
}

void MainFrame::onPressbtnCut(wxCommandEvent& event) {
    cut();
}

void MainFrame::sortItems(const wxTreeItemId& rootItem) {
    treeCtrl->SortChildren(rootItem);

    saveTree();
}
