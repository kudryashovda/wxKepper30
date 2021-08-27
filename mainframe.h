#pragma once

#include "wxLogic.h"
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/listbox.h>
#include <wx/numdlg.h>
#include <wx/textfile.h>
#include <wx/treectrl.h>
#include <wx/wx.h>

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title, wxLogic& logic);

private:
    wxLogic logic_;

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
    void InitTreeCtrl();
public:
    void OnBtnAddPress(wxCommandEvent& event);
    void onTreeItemClick(wxCommandEvent& event);
};
