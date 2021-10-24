#include "mainframe.h"
#include "wxLogic.h"

class MyApp : public wxApp {
public:
    bool OnInit() override;
};

bool MyApp::OnInit() {
    wxSetlocale(LC_ALL, ""); // to work with non-ascii chars in filenames

    wxLogic logic;

    wxString title("Keeper");
    auto* frame = new MainFrame(title, logic);
    frame->SetMinSize(wxSize(800, 800));
    frame->Show(true);

    return true;
}

IMPLEMENT_APP(MyApp)
