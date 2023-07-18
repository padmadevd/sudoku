#include "customWidgets.hpp"
#include "wx/dcgraph.h"
#include "wx/gdicmn.h"
#include "wx/font.h"
#include <wx/dcclient.h>
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/event.h>
#include <wx/window.h>
#include "sudokuCtrl/drawingUtils.hpp"
#include "wx/colour.h"
#include "wx/control.h"

void customWindow::onEraseBG(wxEraseEvent &event){

}

void customLabel::onEraseBG(wxEraseEvent &event){

}

void customButton::onEraseBG(wxEraseEvent &event){

}

customWindow::customWindow(wxWindow *parent, wxWindowID id)
	: wxWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxFULL_REPAINT_ON_RESIZE), background("#131313")
{
	Bind(wxEVT_ERASE_BACKGROUND, &customWindow::onEraseBG, this, id);
	Bind(wxEVT_PAINT, &customWindow::onPaint, this, id);
}

void customWindow::onPaint(wxPaintEvent &event){
	wxPaintDC dc(this);
	dc.SetBackground(background);
	dc.Clear();
}

customLabel::customLabel(wxWindow *parent, wxWindowID id, wxString text)
	: wxWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxFULL_REPAINT_ON_RESIZE), label(text), background("#131313"), foreground("#DEDDDA"), font("ubuntu regular 16")
{
	Bind(wxEVT_ERASE_BACKGROUND, &customLabel::onEraseBG, this, id);
	Bind(wxEVT_PAINT, &customLabel::onPaint, this, id);
}

wxSize customLabel::DoGetBestClientSize() const{
	wxClientDC dc((wxWindow*)this);
	dc.SetFont(font);
	return dc.FromDIP(dc.GetTextExtent(label));
}
int customLabel::DoGetBestClientHeight() const{
	return DoGetBestSize().GetHeight();
}
int customLabel::DoGetBestClientWidth() const{
	return  DoGetBestClientSize().GetWidth();
}

void customLabel::onPaint(wxPaintEvent &event){
	wxBufferedPaintDC dc(this);
	dc.SetBackground(background);
	dc.Clear();

	dc.SetTextBackground(background);
	dc.SetTextForeground(foreground);
	dc.SetFont(font);
	drawCenteredText(dc, label, dc.FromDIP(wxPoint(0, 0)), dc.FromDIP(GetClientSize()));
}

customButton::customButton(wxWindow *parent, wxWindowID id, wxString text)
	: wxWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxFULL_REPAINT_ON_RESIZE), label(text), font("ubuntu regular 14")
{

	background = "#202020";
	foreground = "#DEDDDA";
	backgroundHover = "#272727";
	foregroundHover = foreground;
	backgroundPress = "#171717";
	foregroundPress = foreground;

	Bind(wxEVT_ERASE_BACKGROUND, &customButton::onEraseBG, this, id);
	Bind(wxEVT_PAINT, &customButton::onPaint, this, id);
	Bind(wxEVT_LEFT_DOWN, &customButton::onMouseDown, this, id);
	Bind(wxEVT_RIGHT_DOWN, &customButton::onMouseDown, this, id);
	Bind(wxEVT_LEFT_UP, &customButton::onMouseUp, this, id);
	Bind(wxEVT_RIGHT_DOWN, &customButton::onMouseUp, this, id);
	Bind(wxEVT_ENTER_WINDOW, &customButton::onMouseEnter, this, id);
	Bind(wxEVT_LEAVE_WINDOW, &customButton::onMouseLeave, this, id);
}

wxSize customButton::DoGetBestClientSize() const{
	wxClientDC dc((wxWindow*)this);
	dc.SetFont(font);
	wxSize size = dc.FromDIP(dc.GetTextExtent(label));
	size.x += dc.FromDIP(padding*6);
	size.y += dc.FromDIP(padding*2);
	return size;
}
int customButton::DoGetBestClientHeight() const{
	return DoGetBestSize().GetHeight();
}
int customButton::DoGetBestClientWidth() const{
	return  DoGetBestClientSize().GetWidth();
}

void customButton::onMouseDown(wxMouseEvent &event){
	prevState = state;
	state = state_pressed;
	Refresh();
}
void customButton::onMouseUp(wxMouseEvent &event){
	if(state == state_pressed){
		state = prevState;
		wxCommandEvent evt(wxEVT_BUTTON);
		evt.SetId(GetId());
		ProcessEvent(evt);
	}
	Refresh();
}
void customButton::onMouseEnter(wxMouseEvent &event){
	prevState = state;
	state = state_hover;
	Refresh();
}
void customButton::onMouseLeave(wxMouseEvent &event){
	prevState = state;
	state = state_normal;
	Refresh();
}

void customButton::onPaint(wxPaintEvent &event){
	wxBufferedPaintDC dc(this);

	wxColour bg;
	wxColour fg;

	if(state == state_normal){
		bg = background;
		fg = foreground;
	}else if(state == state_hover){
		bg = backgroundHover;
		fg = foregroundHover;
	}else{
		bg = backgroundPress;
		fg = foregroundPress;
	}

	dc.SetBackground(bg);
	dc.Clear();
	dc.SetFont(font);
	dc.SetTextForeground(fg);
	dc.SetTextBackground(bg);
	drawCenteredText(dc, label, dc.FromDIP(wxPoint(0, 0)), dc.FromDIP(GetClientSize()));
}