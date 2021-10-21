#include "mainframe.h"
#include "wxLogic.h"

class MyApp : public wxApp {
public:
    bool OnInit() override;
};

bool MyApp::OnInit() {
    wxSetlocale(LC_ALL, "");

    wxLogic logic;

    wxString title("Keeper");
    auto* frame = new MainFrame(title, logic);
    frame->Show(true);

    return true;
}

IMPLEMENT_APP(MyApp)
