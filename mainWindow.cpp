#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include "mainWindow.h"

mainWindow::mainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	//设置主窗口大小
	this->setMinimumSize(1050, 800);
	this->setMaximumSize(1050, 800);
	QWidget *centerWindow = new QWidget();
	this->setCentralWidget(centerWindow);

	//绘制棋盘画布
	chess_board = new board(this);

	//获取对方ip等信息
	ip_lb=new QLabel("对方Ip:  ");
	ip_lb->setFixedSize(50, 20);
	ip_txt = new QLineEdit();
	ip_txt->setFixedSize(195, 20);
	export_lb = new QLabel("发送端口:");
	export_lb->setFixedSize(65, 20);
	export_txt = new QLineEdit();
	export_txt->setFixedSize(50, 20);
	import_lb = new QLabel("接收端口:");
	import_lb->setFixedSize(65, 20);
	import_txt = new QLineEdit();
	import_txt->setFixedSize(50, 20);
	set_bt = new QPushButton("配置");
	set_bt->setFixedSize(40, 30);
	//set_bt->setEnabled(false);
	QGridLayout *layout_ip = new QGridLayout();
	layout_ip->addWidget(ip_lb, 0, 0, Qt::AlignLeft);
	layout_ip->addWidget(ip_txt, 0, 1, Qt::AlignLeft);
	QGridLayout *layout_port = new QGridLayout();
	layout_port->addWidget(export_lb, 0, 0, Qt::AlignLeft);
	layout_port->addWidget(export_txt, 0, 1, Qt::AlignLeft);
	layout_port->addWidget(import_lb, 0, 2, Qt::AlignLeft);
	layout_port->addWidget(import_txt, 0, 3, Qt::AlignLeft);

	QGridLayout *layout_net = new QGridLayout();
	layout_net->addLayout(layout_ip, 0, 0, Qt::AlignLeft);
	layout_net->addLayout(layout_port, 1, 0, Qt::AlignLeft);
	layout_net->addWidget(set_bt, 2, 0, Qt::AlignRight);

	//决定执黑或者白棋
	start_with_black = new QPushButton("start with black");
	start_with_black->setEnabled(false);
	start_with_black->setFixedSize(150, 30);
	start_with_white = new QPushButton("start with white");
	start_with_white->setEnabled(false);
	start_with_white->setFixedSize(150, 30);
	restart = new QPushButton("Restart");
	restart->setEnabled(false);
	restart->setFixedSize(100, 30);
	your_color = new QLabel("所持棋子:");
	show_your_color = new QLabel();
	QGridLayout *layout_start = new QGridLayout();
	layout_start->addWidget(start_with_black, 0, 0, Qt::AlignLeft);
	layout_start->addWidget(start_with_white, 1, 0, Qt::AlignLeft);
	layout_start->addWidget(restart, 2, 0, Qt::AlignLeft);
	layout_start->addWidget(your_color, 3, 0, Qt::AlignLeft);
	layout_start->addWidget(show_your_color, 3, 1, Qt::AlignLeft);
	connect(start_with_black, SIGNAL(clicked()), this, SLOT(start_first()));
	connect(start_with_white, SIGNAL(clicked()), this, SLOT(not_start_first()));
	connect(restart, SIGNAL(clicked()), this, SLOT(restart_del()));

	//展示对手点击位置
	new_piece= new QLabel("The opponent put:");
	new_piece_post = new QLabel();
	QGridLayout *layout_pi = new QGridLayout();
	//layout_pi->SetFixedSize(200, 100);
	layout_pi->addWidget(new_piece, 0, 0, Qt::AlignLeft);
	layout_pi->addWidget(new_piece_post, 0, 1, Qt::AlignLeft);
	

	//聊天接收到的消息
	message = new QLabel("Message:");
	message->setFixedSize(100, 30);
	received_text = new QLabel();
	//received_text->setMinimumWidth(200);
	QGridLayout *layout_recv = new QGridLayout();
	layout_recv->addWidget(message, 0, 0, Qt::AlignLeft);
	layout_recv->addWidget(received_text, 1, 0, Qt::AlignLeft);

	//聊天发送消息
	//send = new QPushButton("send");
	send->setEnabled(false);
	send->setFixedSize(50, 30);
	text = new QLineEdit();
	text->setFixedSize(250,30);
	QGridLayout *layout_send = new QGridLayout();
	layout_send->addWidget(text, 0, 0, Qt::AlignLeft);
	layout_send->addWidget(send, 1, 0, Qt::AlignRight);

	//右侧信息展示布局
	QGridLayout *layout_chat = new QGridLayout();
	layout_chat->addLayout(layout_net, 0, 0, Qt::AlignLeft);
	layout_chat->addLayout(layout_start, 1, 0, Qt::AlignLeft);
	layout_chat->addLayout(layout_pi, 2, 0, Qt::AlignLeft);
	layout_chat->addLayout(layout_recv, 3, 0, Qt::AlignTop);
	layout_chat->addLayout(layout_send, 4, 0, Qt::AlignTop);

	QGridLayout *layout_main = new QGridLayout();
	layout_main->addWidget(chess_board, 0, 0, Qt::AlignLeft);
	layout_main->addLayout(layout_chat, 0, 1, Qt::AlignLeft,Qt::AlignTop);

	centerWindow->setLayout(layout_main);

	connect(set_bt, SIGNAL(clicked()), this, SLOT(set_IP()));
}
//mainWindow(QVector<QPointF> black_piece, QVector<QPointF> white_piece);

bool not_in(QVector<QPointF> val, QPointF n)
{
	for (auto itr : val)
	{
		if (itr == n)
			return false;
	}
	return true;
}

void mainWindow::mousePressEvent(QMouseEvent *e)
{
	if (game_start&&my_tern&&e->x() > 30 && e->x() < 780 && e->y() > 30 && e->y() < 780)
	{
		size_t row, column;
		column = 0.5 + ((e->x() - 40) / 40.0);
		row = ((e->y() - 40) / 40.0) + 0.5;
		if (not_in(my_piece, QPointF(row, column)) && not_in(cmp_piece, QPointF(row, column)))
		{
			++my_step;
			my_piece.push_back(QPointF(row, column));
			piece[row][column] = 1;
			if (!referee(cmp_piece))
			{
				if (!pos_available(QPointF(row, column)))
				{
					--my_step;
					my_piece.pop_back();
					piece[row][column] = 0;
				}
				else
				{
					std::string str;
					char a = row, b = column;
					str += a;
					str += b;
					sender->send(str.c_str(), str.length());
					my_tern = false;
				}
			}
			else
			{
				std::string str;
				char a = row, b = column;
				str += a;
				str += b;
				sender->send(str.c_str(), str.length());
				my_tern = false;
			}
		}
	}
	this->update();
};

void mainWindow::paintEvent(QPaintEvent *)
{
	const size_t row_pix = padding + interval*(row - 1) + padding;
	const size_t column_pix = padding + interval*(column - 1) + padding;

	QPainter painter(this);

	painter.setPen(Qt::black);

	for (size_t i = 0; i < row; ++i)
	{
		painter.drawLine(padding, padding + i*interval, column_pix - padding, padding + i*interval);
	}
	for (size_t j = 0; j < row; ++j)
	{
		painter.drawLine(padding + j*interval, padding, padding + j*interval, row_pix - padding);
	}
	painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));//毛刷：颜色，实图案
	painter.drawEllipse(QPointF(row_pix / 2, column_pix / 2), interval / 4, interval / 4);//画椭圆：中心点X,Y,宽度，高度


	//绘制我的棋子
	if(my_first)
		painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));//毛刷：颜色，实图案
	else
		painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
	for (auto itr : my_piece)
	{

		painter.drawEllipse(QPointF(40 + 40 * (itr.y()), 40 + 40 * (itr.x())), 40 / 3, 40 / 3);
	}

	//绘制对手的棋子
	if (!my_first)
		painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));//毛刷：颜色，实图案
	else
		painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
	for (auto itr : cmp_piece)
	{
		painter.drawEllipse(QPointF(40 + 40 * (itr.y()), 40 + 40 * (itr.x())), 40 / 3, 40 / 3);
	}
	painter.end();
};

bool mainWindow::has_qi(const QPointF &p)
{
	size_t row = p.x();
	size_t column = p.y();
	if (row > 0)
		if (piece[row - 1][column] == 0)
			return true;
	if (row < 19)
		if (piece[row + 1][column] == 0)
			return true;

	if (column > 0)
		if (piece[row][column - 1] == 0)
			return true;
	if (column < 19)
		if (piece[row][column + 1] == 0)
			return true;
	return false;
}

void mainWindow::get_block(const QPointF &f, QVector<QPointF> &block)
{
	if (not_in(block, f))
		block.push_back(f);
	size_t row = f.x();
	size_t column = f.y();
	if (row > 0)
	{
		if (piece[row - 1][column] == piece[row][column])
			if (not_in(block, QPointF(row - 1, column)))
			{
				block.push_back(QPointF(row - 1, column));
				get_block(QPointF(row - 1, column), block);
			}
	}

	if (row < 19)
	{
		if (piece[row + 1][column] == piece[row][column])
			if (not_in(block, QPointF(row + 1, column)))
			{
				block.push_back(QPointF(row + 1, column));
				get_block(QPointF(row + 1, column), block);
			}
	}

	if (column > 0)
	{
		if (piece[row][column - 1] == piece[row][column])
			if (not_in(block, QPointF(row, column - 1)))
			{
				block.push_back(QPointF(row, column - 1));
				get_block(QPointF(row, column - 1), block);
			}
	}

	if (column < 19)
	{
		if (piece[row][column + 1] == piece[row][column])
			if (not_in(block, QPointF(row, column + 1)))
			{
				block.push_back(QPointF(row, column + 1));
				get_block(QPointF(row, column + 1), block);
			}
	}
};
bool mainWindow::block_has_qi(const QVector<QPointF> &block)
{
	for (auto itr : block)
	{
		if (has_qi(itr))
			return true;
	}
	return false;
}

//删除棋子
void delete_piece(QPointF &del, QVector<QPointF> &p)
{
	for (auto itr = p.begin(); itr != p.end(); ++itr)
	{
		if (*itr == del)
		{
			p.erase(itr);
			break;
		}
	}
}

bool mainWindow::referee(QVector<QPointF> &compiece)
{
	//for each piece in white_piece and black_piece, to chech whether it have qi. 
	//if it has no qi, add it in the block_piece,then check the four piece around it have qi;
	//if it hs qi, check next;
	QVector<QPointF> wait_list;
	for (auto itr : compiece)
		wait_list.push_back(itr);
	bool remove = false;
	while (wait_list.size() != 0)
	{
		QVector<QPointF> block_piece;//store a block of interconnected pieces;
		get_block(wait_list.front(), block_piece);
		wait_list.pop_front();
		if (!block_has_qi(block_piece))
		{
			remove = true;
			while (block_piece.size() != 0)
			{
				QPointF delt;
				delt = block_piece.front();
				block_piece.pop_front();
				delete_piece(delt, compiece);
				delete_piece(delt, wait_list);
				size_t row = delt.x();
				size_t column = delt.y();
				piece[row][column] = 0;
			}

		}

	}
	return remove;
};

//
bool mainWindow::pos_available(const QPointF &p)
{
	size_t row = p.x();
	size_t column = p.y();
	if (row > 0)
		if (piece[row - 1][column] == 0 || piece[row - 1][column] == piece[row][column])
			return true;
	if (row < 19)
		if (piece[row + 1][column] == 0 || piece[row + 1][column] == piece[row][column])
			return true;

	if (column > 0)
		if (piece[row][column - 1] == 0 || piece[row][column - 1] == piece[row][column])
			return true;
	if (column < 19)
		if (piece[row][column + 1] == 0 || piece[row][column + 1] == piece[row][column])
			return true;
	return false;
};

void mainWindow::send_text()
{
	std::string str = text->text().toStdString();
	sender->send(str.c_str(), str.length());
}
/*
//int to string 
std::string itostr(const int a)
{
	char c;
	int w;
	int b = a;
	std::string str;
	while (b>9)
	{
		w = b % 10;
		c = w + '0';
		str += c;
		b = b / 10;
	}
	c = b + '0';
	str += c;
	int len = str.size();

	for (int i = 0; i<len / 2; ++i)
	{
		char c = str[i];
		str[i] = str[len - 1 - i];
		str[len - 1 - i] = c;
	}
	return str;
}

//这是一段坐标，形如(5,16)
bool is_pos(const std::string &str)
{
	return str.size() == 5 && str[0] == '(' &&
		str[1] >= 0 && str[1] <=18 &&
		str[2] == ',' &&
		str[3] >= 0 && str[3] <= 18 &&
		str[4] == ')';
}
*/
void mainWindow::receive_text(std::string str)
{
	
	if (str.size() == 2 && str[0] >= 0 && str[0] <= 18 && str[1] >= 0 && str[1] <=18)
	{
		std::string a_str="(";
		char a;
		size_t row= str[0], column= str[1];
		cmp_piece.push_back(QPoint(row, column));
		++cmp_step;
		piece[row][column] = -1;
		my_tern = true;
		++row; ++column;
		if (row > 9)
		{
			a= (char)1 + '0';
			a_str += a;
		}		
		a = row % 10 + '0';
		a_str += a;
		a_str += ',';
		if (column > 9)
		{
			a = (char)1 + '0';
			a_str += a;
		}
		a = column % 10 + '0';
		a_str += a;
		a_str += ')';
		new_piece_post->setText(QString::fromStdString(a_str));
		this->update();
	}
		
	else
	received_text->setText(QString::fromStdString(str));
}

void mainWindow::set_IP()
{
	Ip = ip_txt->text();
	send->setEnabled(true);
	start_with_black->setEnabled(true);
	start_with_white->setEnabled(true);
	restart->setEnabled(true);
	export = export_txt->text().toInt();
	import = export_txt->text().toInt();
	sender = new DataSender(Ip, export, this);
	receiver = new DataReceiver(Ip, import, this);
	connect(send, SIGNAL(clicked()), this, SLOT(send_text()));
	connect(receiver, SIGNAL(dataReady(std::string)), this, SLOT(receive_text(std::string)));
}

void mainWindow::start_first()
{
	my_first = true;
	game_start= true;
	my_tern = true;
	start_with_black->setEnabled(false);
	start_with_white->setEnabled(false);
	show_your_color->setText("Black");
	my_piece.clear();
	my_step = 0;
	cmp_piece.clear();
	cmp_step = 0;
	this->update();
}
void mainWindow::not_start_first()
{
	my_first = false;
	game_start = true;
	my_tern = false;
	start_with_black->setEnabled(false);
	start_with_white->setEnabled(false);
	show_your_color->setText("White");
	my_piece.clear();
	my_step = 0;
	cmp_piece.clear();
	cmp_step = 0;
	this->update();
}

void mainWindow::restart_del()
{
	start_with_black->setEnabled(true);
	start_with_white->setEnabled(true);
	my_piece.clear();
	my_step = 0;
	cmp_piece.clear();
	cmp_step = 0;
	for (auto itr : piece)
		itr = 0;
	this->update();
};