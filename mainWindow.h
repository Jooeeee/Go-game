#pragma once

#include <QtWidgets/QMainWindow>
#include <QLayout>
#include "board.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include "DataReceiver.h"
#include "DataSender.h"
#include <QUdpSocket>

class mainWindow : public QMainWindow
{
	Q_OBJECT

public:
	mainWindow(QWidget *parent = Q_NULLPTR);
	
private:
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *e);

	// this is a referee, judging whether a piece or some pieces have no qi
	//delete all pieces which have no qi;
	bool referee(QVector<QPointF> &compiece);
	bool has_qi(const QPointF &);//判断单个棋子是否有气；
	void get_block(const QPointF &f, QVector<QPointF> &block);//由一个点形成一个块；
	bool block_has_qi(const QVector<QPointF>&);//whether the block has qi;	

	//对于下棋的点，若此处无气又不能提子，则此点不能下棋;
	bool pos_available(const QPointF &f);
private:
	board *chess_board;
	QVector<QPointF> my_piece, cmp_piece;
	size_t my_step = 0;
	size_t cmp_step = 0;
	int piece[19][19] = { 0 };//to store the color of pieces at eack pisition; 1 for black piece，0 for blank, -1 for white;
	size_t row = 19;
	size_t column = 19;
	static const size_t padding = 40;
	static const size_t interval = 40;
	bool my_tern = false;

	//用于定义聊天的变量
	QPushButton *send=new QPushButton("send");;
	QLineEdit *text;
	QLabel *received_text;
	QLabel *message;
	DataSender *sender;
	DataReceiver *receiver;

	QLabel *new_piece;
	QLabel *new_piece_post;
	
	//用于获取ip的变量
	QString Ip;
	quint16 export,import;
	QPushButton *set_bt;
	QLabel *ip_lb;
	QLineEdit *ip_txt;
	QLabel *export_lb;
	QLineEdit *export_txt;
	QLabel *import_lb;
	QLineEdit *import_txt;

	//指示游戏开始
	QPushButton *start_with_black,*start_with_white,*restart;
	QLabel *your_color;
	QLabel *show_your_color;
	bool my_first=true;
	bool game_start = false;


public slots:
	void send_text();
	void receive_text(std::string);
	void set_IP();//设置对战ip
	void start_first();//设置开始顺序；
	void not_start_first();
	void restart_del();//重新开始，清除所有数据；
};
