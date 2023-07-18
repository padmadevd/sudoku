#ifndef DRAWING_UTILS_HPP
#define DRAWING_UTILS_HPP

#include "wx/gdicmn.h"
#include "wx/string.h"
#include <wx/dcgraph.h>
#include <wx/wx.h>

void drawCenteredText(wxDC &dc, wxString text, wxPoint position, wxSize size);

#endif