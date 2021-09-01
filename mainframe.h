#pragma once

#include "wxLogic.h"
#include <wx/listbox.h>
#include <wx/numdlg.h>
#include <wx/treectrl.h>
#include <wx/wx.h>

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title, wxLogic& logic);

private:
    wxLogic logic_;
    long style_;

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

    void BindEvents();

public:
    void OnBtnAddClick(wxCommandEvent& event);
    void onTreeItemClick(wxCommandEvent& event);
    void onPressbtnExport(wxCommandEvent& event);
    void onPressbtnImport(wxCommandEvent& event);
    void onPressbtnDel(wxCommandEvent & event);

    void ShowCard(const TreeItem& info);
};

class DlgAppendItem : public wxDialog {
public:
    DlgAppendItem(wxWindow* parent, wxWindowID id, const wxString& title, long my_style);

    wxTextCtrl *dlgEdtText, *dlgComments;

private:
    long style_;
};