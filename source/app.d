import std.stdio;
import std.socket;
import std.algorithm;
import std.array;
import std.string;
import std.conv;
import consolecolors;

void main(string[] args) {
	enableConsoleUTF8(); /* Нужно для корректной работы на Windows */
	if (args.length <= 1) {
		shell();
	} else {
		if (args[1] == "-q") {
        		quick(args);
        	} else {
        		writeln("Аргументы не распознаны.");
        	}
        }
}

void uerr(string error, char type) {
	switch (type) {
		case 'e':
			cwritefln("tcpping: <b><red>Ошибка:</red> %s</b>", error);
			break;
		case 'i':
			cwritefln("tcpping: <b><cyan>Подсказка:</cyan> %s</b>", error);
			break;
		case 's':
			cwritefln("tcpping: <b><lgreen>Успешно!</lgreen></b>");
			break;
		default:
			return;
	}
}

void tcpsend(string addr_s, int port, string msg) {
	auto tcps = new TcpSocket();
        auto addr = new InternetAddress(std.socket.InternetAddress.parse(addr_s), to!ushort(port));
        tcps.connect(addr);
        tcps.send(msg);
        uerr("", 's');
	tcps.close();
}

int quick(string[] args) {
	cwritefln("<b>Quick TcpPing</b>:<b><grey> v0.1.3</grey></b>");
	
	if (args.length < 6) {
		uerr("Недостаточно аргументов.", 'e');
		return(-1);
	}
	
	if (!(args[2] == "-a" || args[2] == "--address")) {
		uerr("Неверное использование.", 'e');
		uerr("Вторым аргументом должен быть -a [адрес:порт] или --address [адрес:порт]!", 'i');
		return(-1);
	}
	
	if (!(args[4] == "-s" || args[4] == "--send")) {
		uerr("Неверное использование.", 'e');
		uerr("Третьим аргументом должен быть -s [сообщение] или --send [сообщение]!", 'i');
		return(-1);
	}
	
	string[] tmp = args[3].split(":");
	tcpsend(tmp[0], to!int(tmp[1]), args[5]);
	
	return(0);
}

void shell() {
	cwritefln("<b>Консоль TcpPing</b>:<b><grey> v0.1.3</grey></b>");
        bool gogo = true;
	string addr_s = "127.0.0.1";
	int port = 5555;
        while (gogo) {
        	write("[tcpping] > ");
		stdout.flush();
                string[] input = (stdin.readln().strip()).split();
                switch (input[0]) {
                        case "exit":
                                gogo = false;
                                break;
                        case "connect":
                                if (input.length == 2) {
                                	auto tmp = input[1].findSplit(":");
                                	if (tmp[2] != "") {
                                		addr_s = tmp[0];
						port = to!int(tmp[2]);
						uerr("", 's');
					} else {
						uerr("Неверное использование.", 'e');
						uerr("Адрес должен быть в формате айпи:порт!", 'i');
					}
                                } else {
                                	write("Адрес TCP сервера: ");
					stdout.flush();
                                	auto tmp = (stdin.readln().strip()).findSplit(":");
                                	if (tmp[2] != "") {
                                		addr_s = tmp[0];
                                                port = to!int(tmp[2]);
                                                uerr("", 's');
                                	} else {
                                		uerr("Неверное использование.", 'e');
						uerr("Адрес должен быть в формате айпи:порт!", 'i');
                                	}
                                }
                                break;
                        case "send":
                                if (input.length == 2) {
                                	tcpsend(addr_s, port, input[1]);
                                } else {
                                	write("Сообщение: ");
					stdout.flush();
                                	string tmp = stdin.readln().strip();
                                	tcpsend(addr_s, port, tmp);
                                }
                                break;
                        case "help":
                                cwritefln("<b>Команды:\n  connect [адрес:порт]</b> - Подключает к UDP серверу. Если адрес сервера не указан, Вас его спросят. <b><red>ИСПОЛЬЗОВАТЬ ДО \"send\"!</red>\n  send [сообщение]</b> - Отправляет сообщение на подключенный UDP сервер. <b><red>ИСПОЛЬЗОВАТЬ ПОСЛЕ \"connect\"!</red>\n  ver</b> - Выводит версию Консоли TcpPing.\n  <b>exit</b> - Выходит из Консоли TcpPing.");
                                break;
                        case "ver":
                                cwritefln("<b>Консоль TcpPing</b>:<b><grey> v0.1.3</grey></b>");
                                break;
                        
                        default:
                                uerr("Неизвестная команда: \"" ~ input[0] ~ "\"", 'e');
                }
        }
}
