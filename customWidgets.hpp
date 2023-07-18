#ifndef CUSTOM_WIDGETS_HPP
#define CUSTOM_WIDGETS_HPP

#include "wx/event.h"
#include "wx/colour.h"
#include "wx/font.h"
#include "wx/string.h"
#include "wx/stringimpl.h"
#include "wx/window.h"
#include <wx/wx.h>

class customWindow : public wxWindow{
public:
	customWindow(wxWindow *parent, wxWindowID id);
	wxColour background;

protected:

	void onEraseBG(wxEraseEvent &event);
	void onPaint(wxPaintEvent &event);

};

class customLabel: public wxWindow{
public:
	customLabel(wxWindow *parent, wxWindowID id, wxString label = wxEmptyString);
	wxColour background;
	wxColour foreground;
	wxFont font;
	wxString label;

protected:

	wxSize DoGetBestClientSize() const;
	int DoGetBestClientHeight() const;
	int DoGetBestClientWidth() const;

	void onEraseBG(wxEraseEvent &event);
	void onPaint(wxPaintEvent &event);
};

enum customButtonState {
	state_normal,
	state_hover,
	state_pressed
};

class customButton : public wxWindow{
public:
	customButton(wxWindow *parent, wxWindowID id, wxString label = wxEmptyString);
	wxString label;
	wxColour background;
	wxColour foreground;
	wxColour backgroundHover;
	wxColour foregroundHover;
	wxColour backgroundPress;
	wxColour foregroundPress;
	wxFont font;
	int padding = 5;

protected:
	customButtonState state = state_normal;
	customButtonState prevState = state_normal;

	wxSize DoGetBestClientSize() const;
	int DoGetBestClientHeight() const;
	int DoGetBestClientWidth() const;

	void onEraseBG(wxEraseEvent &event);
	void onPaint(wxPaintEvent &event);

	void onMouseDown(wxMouseEvent &event);
	void onMouseUp(wxMouseEvent &event);
	void onMouseEnter(wxMouseEvent &event);
	void onMouseLeave(wxMouseEvent &event);
};

#endif