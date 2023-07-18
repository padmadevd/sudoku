#include "customWidgets.hpp"
#include "sudokuCtrl/solver.hpp"
#include "wx/button.h"
#include "wx/event.h"
#include "wx/gdicmn.h"
#include "sudokuCtrl/generator.hpp"
#include "wx/app.h"
#include "wx/frame.h"
#include "wx/stattext.h"
#include "wx/sizer.h"
#include "wx/toplevel.h"
#include "wx/window.h"
#include <cstdio>
#include <vector>
#include <wx/wx.h>
#include "sudokuCtrl/sudokuGrid.hpp"
#include "wx/wxcrtvararg.h"

class gameScreen : public customWindow{
public:
	customButton *backbtn;
	sudokuGridCtrl *grid;
	customButton *solvebtn;
	wxBoxSizer *sizer;
	gameScreen(wxWindow *parent, wxWindowID id);
};

class menuScreen : public customWindow{
public:
	customButton *easybtn;
	customButton *mediumbtn;
	customButton *hardbtn;
	customButton *quitbtn;
	wxBoxSizer *sizer;
	menuScreen(wxWindow *parent, wxWindowID id);
};

class finishScreen : public customWindow{
public:
	customLabel *label1;
	customLabel *label2;
	customButton *menubtn;
	wxBoxSizer *sizer;
	finishScreen(wxWindow *parent, wxWindowID id);
};

class solutionScreen : public customWindow{
public:
	customLabel *label;
	sudokuGridCtrl *grid;
	customButton *menubtn;
	wxBoxSizer *sizer;
	solutionScreen(wxWindow *parent, wxWindowID id);
};

gameScreen::gameScreen(wxWindow *parent, wxWindowID id)
	: customWindow(parent, id)
{
	backbtn = new customButton(this, wxID_ANY, "back");
	backbtn->padding = 10;
	grid = new sudokuGridCtrl(this, wxID_ANY, 40);
	solvebtn = new customButton(this, wxID_ANY, "solve");
	solvebtn->padding = 10;

	sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(backbtn, wxSizerFlags(0).Left().Border(wxALL, 20));
	sizer->Add(grid, wxSizerFlags(0).Center().Border(wxLEFT|wxRIGHT, 10));
	sizer->Add(solvebtn, wxSizerFlags(0).Right().Border(wxALL, 20));

	SetSizer(sizer);

	// wxPrintf("game screen\n");

}

menuScreen::menuScreen(wxWindow *parent, wxWindowID id)
	: customWindow(parent, id)
{
	easybtn = new customButton(this, wxID_ANY, "easy");
	easybtn->padding = 20;
	mediumbtn = new customButton(this, wxID_ANY, "medium");
	mediumbtn->padding = 20;
	hardbtn = new customButton(this, wxID_ANY, "hard");
	hardbtn->padding = 20;
	quitbtn = new customButton(this, wxID_ANY, "quit");
	quitbtn->padding = 10;

	sizer = new wxBoxSizer(wxVERTICAL);

	sizer->AddStretchSpacer();
	sizer->Add(easybtn, wxSizerFlags(0).Expand().Border(wxLEFT|wxRIGHT, 50));
	sizer->Add(mediumbtn, wxSizerFlags(0).Expand().Border(wxLEFT|wxRIGHT, 50));
	sizer->Add(hardbtn, wxSizerFlags(0).Expand().Border(wxLEFT|wxRIGHT, 50));
	sizer->AddStretchSpacer();
	sizer->Add(quitbtn, wxSizerFlags(0).Right().Border(wxALL, 50));

	SetSizer(sizer);

	// wxPrintf("menu screen\n");

}

finishScreen::finishScreen(wxWindow *parent, wxWindowID id)
	: customWindow(parent, id)
{
	label1 = new customLabel(this, wxID_ANY, "congratulation!!!");
	label2 = new customLabel(this, wxID_ANY, "you won");
	menubtn = new customButton(this, wxID_ANY, "menu");
	menubtn->padding = 10;
	sizer = new wxBoxSizer(wxVERTICAL);

	sizer->AddStretchSpacer();
	sizer->Add(label1, wxSizerFlags(0).Center().Border(wxALL, 5));
	sizer->Add(label2, wxSizerFlags(0).Center().Border(wxALL, 5));
	sizer->AddStretchSpacer();
	sizer->Add(menubtn, wxSizerFlags(0).Center().Border(wxALL, 5));
	sizer->AddStretchSpacer();

	SetSizer(sizer);

	// wxPrintf("finish screen\n");

}

solutionScreen::solutionScreen(wxWindow *parent, wxWindowID id)
	: customWindow(parent, id)
{

	label = new customLabel(this, wxID_ANY, "solution");
	grid = new sudokuGridCtrl(this, wxID_ANY, 42);
	grid->setEditable(false);
	grid->m_colours[grid->solBack] = background; 
	grid->m_colours[grid->back] = background;
	grid->m_colours[grid->solFore] = "#26A269";
	menubtn = new customButton(this, wxID_ANY, "menu");
	menubtn->padding = 10;
	sizer = new wxBoxSizer(wxVERTICAL);

	sizer->AddStretchSpacer();
	sizer->Add(label, wxSizerFlags(0).Center().Border(wxALL, 10));
	// sizer->AddStretchSpacer();
	sizer->Add(grid, wxSizerFlags(0).Center().Border(wxALL, 10));
	sizer->AddStretchSpacer();
	sizer->Add(menubtn, wxSizerFlags(0).Right().Border(wxALL, 20));
	// sizer->AddStretchSpacer();

	SetSizer(sizer);

	// wxPrintf("solution screen\n");
}

class sudokuApp : public wxApp{
public:
	bool OnInit();
};

wxDECLARE_APP(sudokuApp);
wxIMPLEMENT_APP(sudokuApp);

class sudokuFrame : public wxFrame{
public:
	sudokuFrame();
	wxBoxSizer *sizer;
	gameScreen *gamepage;
	menuScreen *menupage;
	finishScreen *finishpage;
	solutionScreen *solutionpage;

	void onSolve(wxCommandEvent &event);
	void onEasy(wxCommandEvent &event);
	void onMedium(wxCommandEvent &event);
	void onHard(wxCommandEvent &event);
	void onMenu(wxCommandEvent &event);
	void onComplete(wxCommandEvent &event);
	void onQuit(wxCommandEvent &event);
};

bool sudokuApp::OnInit(){
	sudokuFrame *frame = new sudokuFrame;
	frame->Show();
	return true;
}

sudokuFrame::sudokuFrame()
	: wxFrame(NULL, wxID_ANY, "sudoku", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE - wxMAXIMIZE_BOX - wxRESIZE_BORDER)
{
	SetClientSize(FromDIP(wxSize(500, 535)));

	gamepage = new gameScreen(this, wxID_ANY);
	menupage = new menuScreen(this, wxID_ANY);
	finishpage = new finishScreen(this, wxID_ANY);
	solutionpage = new solutionScreen(this, wxID_ANY);

	sizer = new wxBoxSizer(wxHORIZONTAL);

	sizer->Add(gamepage, wxSizerFlags(1).Expand());
	sizer->Add(menupage, wxSizerFlags(1).Expand());
	sizer->Add(finishpage, wxSizerFlags(1).Expand());
	sizer->Add(solutionpage, wxSizerFlags(1).Expand());

	SetSizer(sizer);

	sizer->Show(menupage, true, true);
	sizer->Show(gamepage, false, true);
	sizer->Show(finishpage, false, true);
	sizer->Show(solutionpage, false, true);
	sizer->Layout();

	Bind(wxEVT_BUTTON, &sudokuFrame::onSolve, this, gamepage->solvebtn->GetId());
	Bind(wxEVT_BUTTON, &sudokuFrame::onEasy, this, menupage->easybtn->GetId());
	Bind(wxEVT_BUTTON, &sudokuFrame::onMedium, this, menupage->mediumbtn->GetId());
	Bind(wxEVT_BUTTON, &sudokuFrame::onHard, this, menupage->hardbtn->GetId());
	Bind(wxEVT_BUTTON, &sudokuFrame::onQuit, this, menupage->quitbtn->GetId());
	Bind(wxEVT_BUTTON, &sudokuFrame::onMenu, this, gamepage->backbtn->GetId());
	Bind(wxEVT_BUTTON, &sudokuFrame::onMenu, this, finishpage->menubtn->GetId());
	Bind(wxEVT_BUTTON, &sudokuFrame::onMenu, this, solutionpage->menubtn->GetId());
	Bind(EVT_SUDOKU_GRID_FILLED, &sudokuFrame::onComplete, this, gamepage->grid->GetId());

}

void sudokuFrame::onSolve(wxCommandEvent &event){

	gridStore pg = gamepage->grid->m_solutionGrid.data;
	std::vector<gridStore> sols;
	solve(0, 0, pg, sols, 2);

	if(sols.size() == 0){
		pg = gamepage->grid->m_puzzleGrid.data;
		solve(0, 0, pg, sols, 2);
	}

	solutionpage->grid->load(sols[0], pg);

	sizer->Show(menupage, false, true);
	sizer->Show(gamepage, false, true);
	sizer->Show(finishpage, false, true);
	sizer->Show(solutionpage, true, true);
	sizer->Layout();
}

void sudokuFrame::onEasy(wxCommandEvent &event){

	gridStore pg;
	generatePuzzle(pg, 30);
	gamepage->grid->load(pg, pg);

	sizer->Show(menupage, false, true);
	sizer->Show(gamepage, true, true);
	sizer->Show(finishpage, false, true);
	sizer->Show(solutionpage, false, true);
	sizer->Layout();
}
void sudokuFrame::onMedium(wxCommandEvent &event){
	gridStore pg;
	generatePuzzle(pg, 45);
	gamepage->grid->load(pg, pg);

	sizer->Show(menupage, false, true);
	sizer->Show(gamepage, true, true);
	sizer->Show(finishpage, false, true);
	sizer->Show(solutionpage, false, true);
	sizer->Layout();
}
void sudokuFrame::onHard(wxCommandEvent &event){
	gridStore pg;
	generatePuzzle(pg, 60);
	gamepage->grid->load(pg, pg);

	sizer->Show(menupage, false, true);
	sizer->Show(gamepage, true, true);
	sizer->Show(finishpage, false, true);
	sizer->Show(solutionpage, false, true);
	sizer->Layout();
}

void sudokuFrame::onQuit(wxCommandEvent &event){
	Close(true);
}

void sudokuFrame::onMenu(wxCommandEvent &event){
	sizer->Show(menupage, true, true);
	sizer->Show(gamepage, false, true);
	sizer->Show(finishpage, false, true);
	sizer->Show(solutionpage, false, true);
	sizer->Layout();
}
void sudokuFrame::onComplete(wxCommandEvent &event){
	// wxPrintf("completed\n");
	if(gamepage->grid->m_solutionGrid.isValidGrid()){
		// wxPrintf("completed\n");
		sizer->Show(menupage, false, true);
		sizer->Show(gamepage, false, true);
		sizer->Show(finishpage, true, true);
		sizer->Show(solutionpage, false, true);
		sizer->Layout();
	}
}