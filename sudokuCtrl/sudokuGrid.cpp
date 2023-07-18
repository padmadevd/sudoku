#include "sudokuGrid.hpp"
#include <wx/dcclient.h>
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/graphics.h>
#include "drawingUtils.hpp"
#include <wx/dcgraph.h>
#include <wx/event.h>
#include <wx/gdicmn.h>
#include <wx/window.h>
#include <wx/sizer.h>
#include <cstdint>
#include "solver.hpp"
#include "wx/chartype.h"
#include "wx/brush.h"
#include "wx/colour.h"
#include "wx/font.h"
#include "wx/pen.h"
#include "wx/wxcrtvararg.h"

sudokuGridCoord::sudokuGridCoord(int8_t row, int8_t col)
	: m_row(row), m_col(col)
{
}

sudokuGridCoord::sudokuGridCoord()
	: m_row(0), m_col(0)
{
}

bool sudokuGridCoord::isValid(){
	if(m_row > 8 || m_row < 0 || m_col > 8 || m_col < 0)
		return false;
	return true;
}

sudokuGridCoord sudokuGridCoord::right(){
	sudokuGridCoord adj;
	if(m_col == 8){
		adj = sudokuGridCoord(m_row+1, 0);
	}else{
		adj = sudokuGridCoord(m_row, m_col+1);
	}
	adj.clamp();
	return adj;
}

sudokuGridCoord sudokuGridCoord::down(){
	sudokuGridCoord adj;
	if(m_row == 8){
		adj = sudokuGridCoord(0, m_col+1);
	}else{
		adj = sudokuGridCoord(m_row+1, m_col);
	}
	adj.clamp();
	return adj;
}

sudokuGridCoord sudokuGridCoord::left(){
	sudokuGridCoord adj;
	if(m_col == 0){
		adj = sudokuGridCoord(m_row-1, 8);
	}else{
		adj = sudokuGridCoord(m_row, m_col-1);
	}
	adj.clamp();
	return adj;
}

sudokuGridCoord sudokuGridCoord::up(){
	sudokuGridCoord adj;
	if(m_row == 0){
		adj = sudokuGridCoord(8, m_col-1);
	}else{
		adj = sudokuGridCoord(m_row-1, m_col);
	}
	adj.clamp();
	return adj;
}

void sudokuGridCoord::clamp(){
	if(isValid())
		return;
	if(m_row < 0){
		m_row = 8;
	}
	if(m_col < 0){
		m_col = 8;
	}
	if(m_row > 8){
		m_row = 0;
	}
	if(m_col > 8){
		m_col = 0;
	}
}

sudokuGridStore::sudokuGridStore(){
	for(int r = 0; r < 9;r++){
		for(int c = 0; c < 9; c++){
			data[r][c] = 0;
		}
	}
}

void sudokuGridStore::load(gridStore& _data){
	clear();
	for (int r = 0; r < 9; ++r)
	{
		for (int c = 0; c < 9; ++c)
		{
			set(r, c, _data[r][c]);
		}
	}
}

void sudokuGridStore::clear(){
	for (int r = 0; r < 9; ++r)
	{
		for (int c = 0; c < 9; ++c)
		{
			data[r][c] = 0;
		}
	}
	invalidCells = 0;
	emptyCells = 81;
}

uint8_t sudokuGridStore::get(sudokuGridCoord coord){
	if(!coord.isValid())
		return  -1;
	// wxPrintf("store get\n");
	return data[coord.getRow()][coord.getCol()];
}

void sudokuGridStore::set(sudokuGridCoord coord, uint8_t value){
	set(coord.getRow(), coord.getCol(), value);
}

void sudokuGridStore::set(uint8_t row, uint8_t col, uint8_t value){
	sudokuGridCoord coord(row, col);
	if(!coord.isValid())
		return;

	if(value > 0 && value < 10){
		if(data[row][col] < 1 || data[row][col] > 9){
			emptyCells -= 1;
			// wxPrintf("emptyCells - %d\n", emptyCells);
		}
	}else{
		if(data[row][col] > 0 && data[row][col] < 10){
			emptyCells += 1;
			// wxPrintf("emptyCells - %d\n", emptyCells);
		}
	}

	bool alreadyValid = possible(data[row][col], row, col, data);
	bool valid = possible(value, row, col, data);

	if(alreadyValid && !valid){
		invalidCells += 1;
	}else if(!alreadyValid && valid){
		invalidCells -= 1;
	}
	data[row][col] = value;
	// wxPrintf("invalidCells : %d\n", invalidCells);
}

bool sudokuGridStore::isValidCell(uint8_t row, uint8_t col){
	// wxPrintf("possible\n");
	sudokuGridCoord coord(row, col);
	if(!coord.isValid())
		return false;
	return possible(data[row][col], row, col, data);
}

bool sudokuGridStore::isValidGrid(){
	return invalidCells == 0;
}

wxDEFINE_EVENT(EVT_SUDOKU_GRID_FILLED, wxCommandEvent);

sudokuGridCtrl::sudokuGridCtrl(wxWindow *parent, wxWindowID id, int cellSize, wxPoint position)
	: wxWindow(parent, id, position, wxDefaultSize, wxBORDER_NONE|wxFULL_REPAINT_ON_RESIZE)
{
	if(cellSize > 20){
		m_cellSize = cellSize;
	}
	int winSize = m_cellSize*9+12;
	// SetMinClientSize(wxSize(winSize, winSize));
	SetMinSize(wxSize(winSize, winSize));

	font = wxFont("monospace bold 14");
	m_colours[solBack] = "#272727";
	m_colours[solFore] = "#DEDDDA";
	m_colours[back] = "#303030";
	m_colours[fore] = "#DEDDDA";
	m_colours[errorBack] = "#622727";
	m_colours[errorFore] = "#DEDDDA";
	m_colours[highlightBack] = "#6C91BD";
	m_colours[highlightFore] = "#272727";
	m_colours[gridLine] = "#9A9996";
	m_colours[subGridLine] = "#404040";

	Bind(wxEVT_PAINT, &sudokuGridCtrl::onPaint, this, id);
	Bind(wxEVT_ERASE_BACKGROUND, &sudokuGridCtrl::onEraseBG, this, id);
	Bind(wxEVT_KEY_DOWN, &sudokuGridCtrl::onKeyDown, this, id);
	Bind(wxEVT_LEFT_DOWN, &sudokuGridCtrl::onMouseClick, this, id);
	Bind(wxEVT_RIGHT_DOWN, &sudokuGridCtrl::onMouseClick, this, id);
	Bind(wxEVT_KILL_FOCUS, &sudokuGridCtrl::onFocusLost, this, id);
}

wxSize sudokuGridCtrl::DoGetBestClientSize() const{
	int winSize = m_cellSize*9+12;
	return FromDIP(wxSize(winSize, winSize));
}
int sudokuGridCtrl::DoGetBestClientHeight() const{
	return m_cellSize*9+12;
}
int sudokuGridCtrl::DoGetBestClientWidth() const{
	return m_cellSize*9+12;
}

void sudokuGridCtrl::load(sudokuGridStore &solutionGrid, sudokuGridStore &puzzleGrid){
	m_solutionGrid = solutionGrid;
	m_puzzleGrid = puzzleGrid;
}

void sudokuGridCtrl::load(sudokuGridStore &solutionGrid){
	m_solutionGrid = solutionGrid;
}

void sudokuGridCtrl::load(gridStore &solutionGrid, gridStore &puzzleGrid){
	m_solutionGrid.load(solutionGrid);
	m_puzzleGrid.load(puzzleGrid);
}

void sudokuGridCtrl::load(gridStore &solutionGrid){
	m_solutionGrid.load(solutionGrid);
}

void sudokuGridCtrl::clear(){
	m_solutionGrid.clear();
	m_puzzleGrid.clear();
	Refresh();
}

void sudokuGridCtrl::setCellSize(int cellSize){
	if(cellSize > 20){
		m_cellSize = cellSize;
	}
	int winSize = m_cellSize*9+12;
	SetMinClientSize(FromDIP(wxSize(winSize, winSize)));
}

void sudokuGridCtrl::onEraseBG(wxEraseEvent &event){

}

void sudokuGridCtrl::onPaint(wxPaintEvent &event){
	wxBufferedPaintDC dc(this);
	wxGCDC gc(dc);

	gc.SetFont(font);
	gc.SetBackground(m_colours[back]);
	gc.Clear();

	wxSize windowSize = GetClientSize();
	wxSize gridSize = wxSize(m_cellSize*9+12, m_cellSize*9+12);
	wxPoint gridOrigin;
	gridOrigin.x = windowSize.x/2 - gridSize.x/2;
	gridOrigin.y = windowSize.y/2 - gridSize.y/2;

	gc.SetPen(*wxTRANSPARENT_PEN);
	gc.SetBrush(m_colours[subGridLine]);
	gc.DrawRectangle(dc.FromDIP(gridOrigin), dc.FromDIP(gridSize));

	wxPoint cellOrigin(gridOrigin.x + 1, gridOrigin.y + 1);

	wxColour cellBack;
	wxColour cellFore;

	for (int r = 0; r < 9; r++)
	{
		cellOrigin.y = gridOrigin.y + r*(m_cellSize+1) + r/3 + 1;
		for (int c = 0; c < 9; c++)
		{
			cellOrigin.x = gridOrigin.x + c*(m_cellSize+1) + c/3 + 1;
			uint8_t value = m_solutionGrid.get(sudokuGridCoord(r, c));
			wxString valStr;
			if(value > 0 && value < 10)
				valStr.Printf("%d" , (int)value);

			if(!m_solutionGrid.isValidCell(r, c)){
				// wxPrintf("debug paint\n");
				cellBack = m_colours[errorBack];
				cellFore = m_colours[errorFore];
			}else if(value != 0 && m_solutionGrid.get(m_cursorCoord) == value){
				// wxPrintf("debug paint\n");
				cellBack = m_colours[highlightBack];
				cellFore = m_colours[highlightFore];
			}else if(m_puzzleGrid.get(sudokuGridCoord(r, c)) != 0){
				// wxPrintf("debug paint\n");
				cellBack = m_colours[back];
				cellFore = m_colours[fore];
			}else{
				// wxPrintf("debug paint\n");
				cellBack = m_colours[solBack];
				cellFore = m_colours[solFore];
			}
			gc.SetPen(*wxTRANSPARENT_PEN);
			gc.SetBrush(cellBack);
			gc.DrawRectangle(dc.FromDIP(cellOrigin.x), dc.FromDIP(cellOrigin.y), dc.FromDIP(m_cellSize), dc.FromDIP(m_cellSize));
			if(m_cursorCoord.m_col == c && m_cursorCoord.m_row == r){
				gc.SetBrush(*wxWHITE);
				gc.DrawRectangle(dc.FromDIP(cellOrigin.x), dc.FromDIP(cellOrigin.y), dc.FromDIP(m_cellSize), dc.FromDIP(m_cellSize));
				gc.SetBrush(cellBack);
				gc.DrawRectangle(dc.FromDIP(cellOrigin.x+2), dc.FromDIP(cellOrigin.y+2), dc.FromDIP(m_cellSize-4), dc.FromDIP(m_cellSize-4));
			}
			dc.SetTextForeground(cellFore);
			dc.SetFont(font);
			drawCenteredText(dc, valStr, dc.FromDIP(cellOrigin), dc.FromDIP(wxSize(m_cellSize, m_cellSize)));
		}
	}
	wxPoint linestart = gridOrigin;
	gc.SetPen(*wxTRANSPARENT_PEN);
	gc.SetBrush(m_colours[gridLine]);
	for(int i = 1; i < 3; i++){
		linestart.x += (3*m_cellSize+4);
		linestart.y += (3*m_cellSize+4);
		gc.DrawRectangle(dc.FromDIP(linestart.x-1), dc.FromDIP(gridOrigin.y), dc.FromDIP(2), dc.FromDIP(gridSize.y));
		gc.DrawRectangle(dc.FromDIP(gridOrigin.x), dc.FromDIP(linestart.y-1), dc.FromDIP(gridSize.x), dc.FromDIP(2));
	}
}
void sudokuGridCtrl::onKeyDown(wxKeyEvent &event){
	int key = event.GetKeyCode();
	if(key == WXK_BACK || (key <= '9' && key >= '0') ){
		if(key == WXK_BACK)
			key = '0';
		if(m_puzzleGrid.get(m_cursorCoord) == 0 && m_editable){
			m_solutionGrid.set(m_cursorCoord, key - '0');
			if(m_solutionGrid.emptyCells == 0){
				wxCommandEvent evt(EVT_SUDOKU_GRID_FILLED);
				evt.SetId(GetId());
				ProcessEvent(evt);
			}
		}
	}else{
		switch (key){
			case WXK_LEFT:
				// wxPrintf("key down\n");
				m_cursorCoord = m_cursorCoord.left();
				break;
			case WXK_UP:
				// wxPrintf("key down\n");
				m_cursorCoord = m_cursorCoord.up();
				break;
			case WXK_RIGHT:
				// wxPrintf("key down\n");
				m_cursorCoord = m_cursorCoord.right();
				break;
			case WXK_DOWN:
				// wxPrintf("key down\n");
				m_cursorCoord = m_cursorCoord.down();
				break;
		}
	}
	Refresh();
}
void sudokuGridCtrl::onMouseClick(wxMouseEvent &event){
	wxPoint mousePos = event.GetPosition();
	sudokuGridCoord subGrid;

	wxSize windowSize = GetClientSize();
	wxSize gridSize = wxSize(m_cellSize*9+12, m_cellSize*9+12);
	wxPoint gridOrigin;
	gridOrigin.x = windowSize.x/2 - gridSize.x/2;
	gridOrigin.y = windowSize.y/2 - gridSize.y/2;
	mousePos.x -= gridOrigin.x;
	mousePos.y -= gridOrigin.y;
	subGrid.m_row = mousePos.y/(3*m_cellSize+4);
	subGrid.m_col = mousePos.x/(3*m_cellSize+4);
	// wxPrintf("grid : %d, %d\n", subGrid.m_col, subGrid.m_row);
	mousePos.x -= subGrid.m_col+1;
	mousePos.y -= subGrid.m_row+1;
	if(mousePos.x < 0 || mousePos.y < 0){
		m_cursorCoord.m_row = -1;
		m_cursorCoord.m_col = -1;
	}else{
		m_cursorCoord.m_row = mousePos.y/(m_cellSize+1);
		m_cursorCoord.m_col = mousePos.x/(m_cellSize+1);
	}
	event.Skip();
	// wxPrintf("mouse : %d, %d\n", mousePos.x, mousePos.y);
	// wxPrintf("cursor : %d, %d\n", (int)m_cursorCoord.m_col, (int)m_cursorCoord.m_row);
	Refresh();
}
void sudokuGridCtrl::onFocusLost(wxFocusEvent &event){
	// wxPrintf("onFocusLost\n");
	m_cursorCoord.set(-1, -1);
	Refresh();
}