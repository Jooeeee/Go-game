#include "board.h"

board::board(QWidget *parent)
	:QLabel(parent)
{
	this->setWindowTitle("Go");
	this->setMaximumSize(800, 780);
	this->setMinimumSize(800, 780);
}