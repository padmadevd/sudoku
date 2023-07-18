#include "drawingUtils.hpp"
#include "wx/gdicmn.h"

void drawCenteredText(wxDC &dc, wxString text, wxPoint position, wxSize size){
	wxSize textSize = dc.FromDIP(dc.GetTextExtent(text));
	wxPoint origin;
	origin.x = position.x + size.x/2 - textSize.x/2;
	origin.y = position.y + size.y/2 - textSize.y/2;
	dc.DrawText(text, origin);
}