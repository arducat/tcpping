use std::env;
use std::net::TcpStream;
use std::io::{self, Write};
use colored::Colorize;

fn print_error(error: &str, type_e: &str) {
    match type_e {
        "err" => {
            println!("tcpping: {}: {}", "Ошибка".bright_red().bold(), error.bold());
        }
        "hlp" => {
            println!("tcpping: {}: {}", "Подсказка".blue().bold(), error.bold());
        }
        "suc" => {
            println!("tcpping: {}", "Успешно!".green().bold());
        }
        _ => {}
    }
}

fn quick_mode(args: Vec<String>) -> Result<(), Box<dyn std::error::Error>> {
    println!("{}: {}", "Quick TcpPing".bold(), "v0.1.2".bright_black().bold());

    if args[2] != "-a" && args[2] != "--address" {
        print_error("Неверные аргументы", "err");
        print_error("Вторым аргументом должен быть -a [адрес tcp сервера] или --address [адрес tcp сервера]!", "hlp");
        return Ok(());
    }

    if args[4] != "-s" && args[4] != "--send" {
        print_error("Неверные аргументы", "err");
        print_error("Третьим аргументом должен быть -s [сообщение] или --send [сообщение]!", "hlp");
        return Ok(());
    }

    let server_addr: &str = args[3].trim();
    let message: &str = args[5].trim();

    match TcpStream::connect(server_addr) {
        Ok(mut stream) => {
            stream.write_all(message.as_bytes())?;
            print_error(" ", "suc");
        },
        Err(e) => {
            print_error(&format!("{}", e), "err");
        }
    }

    Ok(())
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let args: Vec<String> = env::args().collect();

    let mut server_addr: String = "127.0.0.1:3437".to_string();
    let mut message: String = "ping!".to_string();

    if args.len() > 1 && args[1] == "-q" {
        return quick_mode(args);
    }

    println!("{}: {}", "Консоль TcpPing".bold(), "v0.1.2".bright_black().bold());
    loop {
        print!("[tcpping] > ");
        io::stdout().flush().unwrap();

        let mut command = String::new();
        io::stdin().read_line(&mut command).expect("Не удалось прочитать строку");

        let split = command
            .trim()
            .split(' ')
            .filter(|v| !v.is_empty())
            .collect::<Vec<_>>();

        let (command, args) = match split.split_first() {
            Some(val) => val,
            None => {
                println!();
                continue;
            },
        };

        match command.trim() {
            "ver" => println!("{}: {}", "Консоль TcpPing".bold(), "v0.1.2".bright_black().bold()),
            "send" => {
                let arg = if args.is_empty() {
                    print!("Сообщение: ");
                    io::stdout().flush().unwrap();

                    let mut input = String::new();
                    io::stdin().read_line(&mut input).expect("Не удалось прочитать строку");
                    input.trim().to_string()
                } else {
                    args.join(" ")
                };

                match TcpStream::connect(server_addr.clone()) {
                    Ok(mut stream) => {
                        stream.write_all(arg.as_bytes())?;
                        print_error(" ", "suc");
                    },
                    Err(e) => {
                        print_error(&format!("{}", e), "err");
                    }
                }
            },
            "help" => {
                println!("{}", "Команды:". bold());
                println!("  {} - Подключает к TCP серверу. Если адрес сервера не указан, Вас его спросят. {}", "connect [адрес]".bold(), "ИСПОЛЬЗОВАТЬ ДО \"send\"!".bright_red().bold());
                println!("  {} - Отправляет сообщение на подключенный TCP сервер. {}", "send [сообщение]".bold(), "ИСПОЛЬЗОВАТЬ ПОСЛЕ \"connect\"!".bright_red().bold());
                println!("  {} - Выводит версию Консоли TcpPing.", "ver".bold());
                println!("  {} - Выходит из Консоли TcpPing.", "exit".bold());
            },
            "exit" => break,
            "connect" => {
                let mut ip = String::new();
                let arg = if args.is_empty() {
                    print!("Адрес TCP сервера: ");
                    io::stdout().flush().unwrap();

                    io::stdin().read_line(&mut ip).expect("Не удалось прочитать строку");
                    ip.trim()   
                } else {
                    args[0]
                };

                server_addr = arg.to_string();
                print_error(" ", "suc");
            },
            command => print_error(&format!("Неверная команда: \"{}\"!", command), "err"),
        }
    }

    Ok(())
}
