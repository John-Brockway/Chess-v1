#ifndef GRAPHICDISPLAY_H
#define GRAPHICDISPLAY_H
#include "window.h"

class GraphicDisplay
{
	Xwindow w;
public:
	GraphicDisplay();
	void defaultPieces();
	void notify(int x1, int y1, int x2, int y2, char piece);
};

#endif
