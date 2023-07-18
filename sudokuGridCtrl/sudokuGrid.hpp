#ifndef SUDOKU_GRID_HPP
#define SUDOKU_GRID_HPP

#include "solver.hpp"
#include "wx/string.h"
#include <wx/event.h>
#include <wx/gdicmn.h>
#include <wx/colour.h>
#include <wx/font.h>
#include <array>
#include <cstdint>
#include <sys/types.h>
#include <wx/wx.h>
#include <wx/window.h>

class sudokuGridCoord{
public:
	sudokuGridCoord(int8_t row, int8_t col);
	sudokuGridCoord();
	bool isValid();

	sudokuGridCoord right();
	sudokuGridCoord down();
	sudokuGridCoord left();
	sudokuGridCoord up();

	void clamp();

	inline int8_t getRow(){
		return m_row;
	}
	inline int8_t getCol(){
		return m_col;
	}

	inline void set(int8_t row, int8_t col){
		m_row = row, m_col = col;
	}

public:
	int8_t m_row;
	int8_t m_col;
};

class sudokuGridStore{

public:
	sudokuGridStore();
	void load(gridStore& _data);
	void clear();

	uint8_t get(sudokuGridCoord coord);
	void set(sudokuGridCoord coord, uint8_t value);
	void set(uint8_t row, uint8_t col, uint8_t value);
	bool isValidCell(uint8_t row, uint8_t col);
	bool isValidGrid();

	int8_t invalidCells = 0;
	int8_t emptyCells = 81;
	gridStore data;
};

wxDECLARE_EVENT(EVT_SUDOKU_GRID_FILLED, wxCommandEvent);

class sudokuGridCtrl : public wxWindow{

public:
	sudokuGridCtrl(wxWindow *parent, wxWindowID id, int cellSize = 0, wxPoint position = wxDefaultPosition);
	void load(sudokuGridStore &solutionGrid, sudokuGridStore &puzzleGrid);
	void load(sudokuGridStore &solutionGrid);
	void load(gridStore &solutionGrid, gridStore &puzzleGrid);
	void load(gridStore &solutionGrid);
	void clear();

	void setCellSize(int cellSize = 40);
	inline void setEditable(bool editable){
		m_editable = editable;
	}

public:
	enum {
		back = 0,
		fore,
		solBack,
		solFore,
		errorBack,
		errorFore,
		highlightBack,
		highlightFore,
		gridLine,
		subGridLine
	};

	wxColour m_colours[10];
	wxFont font;

	sudokuGridStore m_puzzleGrid;
	sudokuGridStore m_solutionGrid;

protected:
	int m_cellSize = 40;
	bool m_editable = true;
	sudokuGridCoord m_cursorCoord = sudokuGridCoord(-1, -1);

	wxSize DoGetBestClientSize() const;
	int DoGetBestClientHeight() const;
	int DoGetBestClientWidth() const;

protected:
	void onEraseBG(wxEraseEvent &event);
	void onPaint(wxPaintEvent &event);
	void onKeyDown(wxKeyEvent &event);
	void onMouseClick(wxMouseEvent &event);
	void onFocusLost(wxFocusEvent &event);
};

#endif