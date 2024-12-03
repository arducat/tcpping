use std::env;
use std::net::TcpStream;
use std::io::{self, Write};

fn main() -> io::Result<()> {
    let mut address: String = "127.0.0.1:3437".to_string();
    let mut message: String = "ping!".to_string();
    println!("TcpPing 0.1");

    let args: Vec<String> = env::args().collect();

    for i in 1..args.len() {
        let arg = &args[i];

        if arg == "-s" || arg == "--send" {
            address = args[(i.clone()) + {1}].to_string();
            message = args[(i.clone()) + {2}].to_string();

            println!("Сообщение \"{}\" на адрес \"{}\"", &message, &address);
        }
    }
    println!("Отправляем...");
    {
        let stream = TcpStream::connect(address);
        match stream {
            Ok(mut _s) => { _s.write(message.as_bytes()).expect("Ошибка отправки!"); },
            Err(_e) => { print!("Ошибка подключения"); }
        }
    }
    println!("Отправлено");
    Ok(())
}
