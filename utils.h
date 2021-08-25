#pragma once

#include "domain.h"
#include "wx/datetime.h"
#include <wx/filedlg.h>
#include <wx/textfile.h>
#include <wx/treectrl.h>
#include <wx/wx.h>

namespace utils {

Settings getSettingsFromFile(wxString fn, wxChar dir_separator);
bool CheckTrial(const wxString& trial_date);
sTreeItem s_tokenizer(wxString str, wxString delim, size_t elements);

} // namespace utils