#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QVector>
#include <QtCore/QString>
#include "include/rang.hpp"
using std::string;
using std::vector;
using namespace rang; /* брр так лучше конечно не делать, но ладно */

void uerr(string error, char type);
int quick(int argc, char** argv);
void shell();

/* Стырено со Stack Overflow, из-за "отличного" Switch/Case в C++. Ссылка: https://stackoverflow.com/questions/2111667/compile-time-string-hashing */
constexpr unsigned int str2int(const char* str, int h = 0) {
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

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
        t.write(d.toUtf8());
        t.flush();
        
	uerr("", 's');
        return(0);
}

void shell() {
        /* Ну всякие оптимизации ввода-вывода, почему нет? */
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr);
        /* Готовимся к работе */
        QTextStream qtin(stdin);
        
        std::cout << style::bold << "Консоль TcpPing" << style::reset << ": " << style::bold << fg::gray << "v0.1.2-C" << style::reset << "\n";
        std::cout.flush();
        
        QString addr = "127.0.0.1";
        int port = 25565; // ага, тот самый порт из майнкрафта
        QTcpSocket t;
        while(true) {
                std::cout << "[tcpping] > ";
                std::cout.flush();
                QString str = qtin.readLine();
                QVector<QString> v = str.split(" ");
                
                
                
                switch (str2int(v[0].toStdString().c_str())) {
                        case str2int("exit"):
                                exit(0);
                                break;
                        case str2int("connect"):
                                //try {
                                {
                                QString b[2] = {v[1].split(":")[0], v[1].split(":")[1]};
                                int c = b[1].toInt();
                                t.connectToHost(b[0], c);
                                if(!t.waitForConnected(3000)) {
                                         uerr("Не удалось подключится.", 'e');
                                } else {
                                        addr = b[0];
                                        port = c;
                                        uerr("", 's');
                                }
                                        
                                //} catch (...) {
                                //        uerr("", 's');
                                //}
                                }
                                break;
                        case str2int("send"):
                                {
                                QString message = str.section(' ', 1);
                                QByteArray data = message.toUtf8();
                                
                                //socketStream << data;
                                t.write(data);
                                t.flush();
                                uerr("", 's');
                                }
                                break;
                        case str2int("help"):
                                //cwritefln("<b>Команды:\n  connect [адрес:порт]</b> - Подключает к UDP серверу. Если адрес сервера не указан, Вас его спросят. <b><red>ИСПОЛЬЗОВАТЬ ДО \"send\"!</red>\n  send [сообщение]</b> - Отправляет сообщение на подключенный UDP сервер. <b><red>ИСПОЛЬЗОВАТЬ ПОСЛЕ \"connect\"!</red>\n  ver</b> - Выводит версию Консоли UdpPing.\n  <b>exit</b> - Выходит из Консоли UdpPing.");
                                break;
                        case str2int("ver"):
                                //cwritefln("<b>Консоль UdpPing</b>:<b><grey> v0.1.2-D</grey></b>");
                                break;
                        
                        default:
                                uerr("Неизвестная команда: \"" + v[0].toStdString() + "\"", 'e');
                }
        }
}
