import socket

def start_tcp_server(host='127.0.0.1', port=65432):
    # Создаем сокет
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        # Привязываем сокет к адресу и порту
        server_socket.bind((host, port))
        # Начинаем слушать входящие соединения
        server_socket.listen()
        print(f"Сервер запущен и слушает на {host}:{port}")

        while True:
            # Принимаем входящее соединение
            client_socket, client_address = server_socket.accept()
            with client_socket:
                print(f"Подключено к {client_address}")
                while True:
                    # Получаем данные от клиента
                    data = client_socket.recv(1024)
                    if not data:
                        break  # Если данные не получены, выходим из цикла
                    print(f"Получено: {data.decode()}")
                    # Отправляем данные обратно клиенту
                    client_socket.sendall(data)

if __name__ == "__main__":
    start_tcp_server()
