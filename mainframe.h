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
    fs::path GetFilenameFromListbox(wxTreeItemId selected_item, int item_idx);

public:
    void OnBtnAddClick(wxCommandEvent& event);
    void onTreeItemClick(wxCommandEvent& event);
    // void onPressbtnExport(wxCommandEvent& event);
    // void onPressbtnImport(wxCommandEvent& event);
    void onPressbtnDel(wxCommandEvent& event);
    void onPressbtnCut(wxCommandEvent& event);
    void onPressbtnSaveItemData(wxCommandEvent& event);
    void onPressbtnGotoId(wxCommandEvent& event);

    void onPressbtnCreateFile(wxCommandEvent& event);
    void onBoxItemDblClick(wxCommandEvent& event);
    void onPressbtnDelFile(wxCommandEvent& event);

    void ShowCard(const TreeItem& info);
    void AppendItems(const wxArrayTreeItemIds& selected_items, long count);
    void expandAllParents(wxTreeItemId item);
    void UpdateFileBox(wxListBox* lbox);
};

class DlgAppendItem : public wxDialog {
public:
    DlgAppendItem(wxWindow* parent, wxWindowID id, const wxString& title, long my_style);

    wxTextCtrl *dlgEdtText, *dlgComments;
private:
    long style_;
};