#include "mainframe.h"
#include "wxLogic.h"

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

bool MyApp::OnInit() {
    wxSetlocale(LC_ALL, "");

    wxLogic logic;

    wxString title("KEEPer");
    MainFrame* frame = new MainFrame(title, logic);
    frame->Show(true);

    return true;
}

IMPLEMENT_APP(MyApp)

