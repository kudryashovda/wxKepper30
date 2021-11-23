#include "mainframe.h"
#include "wxLogic.h"

class MyApp : public wxApp {
public:
    bool OnInit() override;
};

bool MyApp::OnInit() {    
    wxSetlocale(LC_ALL, ""); // to work with non-ascii chars in filenames

    wxLogic logic;

    const wxString title("Keeper");
    auto* frame = new MainFrame(title, logic);
    const auto window_size = wxSize(1000, 800);
    frame->SetMinSize(window_size);
    frame->SetSize(window_size);

    frame->Show(true);

    return true;
}

IMPLEMENT_APP(MyApp)
