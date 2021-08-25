#include "utils.h"
namespace utils {

bool CheckTrial(const wxString& trial_date) {
    wxDateTime dt_deadline;
    dt_deadline.ParseDateTime(trial_date);

    wxDateTime now = wxDateTime::Now();

    if (now < dt_deadline)
        return false;
    else
        return true;
}

sTreeItem s_tokenizer(wxString str, wxString delim, size_t elements) {
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

    if (vs.size() != elements)
        return ST_ERROR;

    sTreeItem st(NULL, wxAtoi(vs[0]), wxAtoi(vs[1]), vs[2], wxAtoi(vs[3]), vs[4], vs[5]);

    return st;
}

Settings getSettingsFromFile(wxString fn, wxChar dir_separator) {
    Settings iniSettings;

    iniSettings.dataPath = wxGetCwd() + dir_separator;
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

} // namespace utils