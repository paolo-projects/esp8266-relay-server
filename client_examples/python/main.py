from .serial_map import SerialMap
import socket
import ssl

context = ssl.create_default_context()
host = "127.0.0.1"
port = 1234


def main():
    with socket.create_connection((host, port), timeout=3) as sock:
        with context.wrap_socket(sock) as ssocket:
            ssocket.do_handshake()

            auth = SerialMap(values={'username': 'john', 'password': 'connor'})
            ssocket.write(auth.serialize())

            response = SerialMap(ssocket.read(512))

            if response['result'] == "ok":
                command = SerialMap(values={
                    # ...
                })
