#include "window.h"
#include "graphicdisplay.h"
#include <iostream>
#include <string>

GraphicDisplay::GraphicDisplay()
{
	char c = ' ';
	for (int i = 0; i <= 7; i++)
	{
		for (int j = 0; j <= 7; j++)
		{
			w.fillRectangle(50 * i, 50 * j, 50, 50, (i + j) % 2);
			if (j == 0)
			{
				if (i == 0 || i == 7)
					c = 'r';
				else if (i == 1 || i == 6)
					c = 'n';
				else if (i == 2 || i == 5)
					c = 'b';
				else if (i == 3)
					c = 'q';
				else
					c = 'k';
			}
			else if (j == 1)
				c = 'p';
			else if (j == 6)
				c = 'P';
			else if (j == 7)
			{
				if (i == 0 || i == 7)
					c = 'R';
				else if (i == 1 || i == 6)
					c = 'N';
				else if (i == 2 || i == 5)
					c = 'B';
				else if (i == 3)
					c = 'Q';
				else
					c = 'K';
			}
			else
					c = ' ';

			w.drawBigString((50 * i) + 15, (50 * j) + 35, std::string(1, c), Xwindow::Blue);
		}
	}
}

void GraphicDisplay::notify(int x1, int y1, int x2, int y2, char piece)
{
	w.fillRectangle(50 * x1, 50 * y1, 50, 50, (x1 + y1) % 2);
	w.fillRectangle(50 * x2, 50 * y2, 50, 50, (x2 + y2) % 2);
	w.drawBigString((50 * x2) + 15, (50 * y2) + 35, std::string(1, piece), Xwindow::Blue);
}

