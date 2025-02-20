#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <QtNetwork/QTcpSocket>
#include "include/rang.hpp"
using std::string;
using namespace rang; /* брр так лучше конечно не делать, но ладно */

void uerr(string error, char type);
int quick(int argc, char** argv);
void shell();

int main(int argc, char** argv) {
	if (argc <= 1) {
	        shell();
	} else {
		if (strcmp(argv[1], "-q") == 0) {
        		quick(argc, argv);
        	} else {
        		uerr("Аргументы не распознаны.", 'e');
        		std::cout << argv[1];
        	}
        }
	return(0);
}

void uerr(string error, char type) {
	switch (type) {
		case 'e':
			std::cout << "tcpping: " << style::bold << fg::red << "Ошибка: " << style::reset << error << "\n";
			break;
		case 'i':
			std::cout << "tcpping: " << style::bold << fg::cyan << "Подсказка: " << style::reset << error << "\n";
			break;
		case 's':
			std::cout << "tcpping: " << style::bold << fg::green << "Успешно!" << style::reset << "\n";
			break;
		default:
			int a = 23434234;
	}
}

int quick(int argc, char** argv) {
        std::cout << style::bold << "Quick TcpPing" << style::reset << ": " << style::bold << fg::gray << "v0.1.2-C" << style::reset << "\n";
        if (argc < 6) {
		uerr("Недостаточно аргументов.", 'e');
		return(-1);
	}
	
	if (!(strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "--address"))) {
		uerr("Аргументы неверны.", 'e');
		uerr("Вторым аргументом должен быть -a [адрес:порт] или --address [адрес:порт]!", 'i');
		return(-1);
	}
	
	if (!(strcmp(argv[4], "-s") == 0 || strcmp(argv[4], "--send"))) {
		uerr("Аргументы неверны.", 'e');
		uerr("Третьим аргументом должен быть -s [сообщение] или --send [сообщение]!", 'i');
		return(-1);
	}
	
	/* Шедевриально я тут наговнокодил, я на D так еще не парился */
	QTcpSocket t;
	QString a = argv[3];
	QString b[2] = {a.split(":")[0], a.split(":")[1]};
	int c = b[1].toInt();
	QString d = argv[5];
	QByteArray byteArray = d.toUtf8();
        t.connectToHost(b[0], c);
        if(!t.waitForConnected(3000)) {
            std::cout << "oops";
        }
        QDataStream socketStream(&t);
        socketStream.setVersion(QDataStream::Qt_5_15);
        
        socketStream << d;
        t.flush();
        
	uerr("", 's');
        return(0);
}

void shell() {
        
}
