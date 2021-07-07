import socket
import ssl
import os
import time
import argparse

from dotenv import load_dotenv

from serial_map.serial_map import SerialMap

load_dotenv()

context = ssl.create_default_context()
context.check_hostname = False
context.verify_mode = ssl.CERT_REQUIRED
context.load_verify_locations(os.path.dirname(
    os.path.realpath(__file__)) + '\\.cert.pem')

HOST = os.getenv('SRV_ADDR')
PORT = os.getenv('SRV_PORT')
USER = os.getenv('SRV_USERNAME')
PASS = os.getenv('SRV_PASSWORD')
TIMEOUT = int(os.getenv('CONN_TIMEOUT', '10'))


args = argparse.ArgumentParser(
    description="Test Client for NodeMCU Realy Server")
args.add_argument('--get', help='Get the relay state', action='store_true')
args.add_argument('--set', help="Set the relay state", choices=['on', 'off'])
args.add_argument('--reset', help='Switch to AP mode', action='store_true')


def main():
    arguments = args.parse_args()

    action = SerialMap()
    if arguments.get:
        action.put('action', 'getstate')
    elif arguments.set:
        action['action'] = 'setstate'
        action['state'] = arguments.set
    elif arguments.reset:
        action['action'] = 'disconnect'
    else:
        action['action'] = 'getstate'

    with socket.create_connection((HOST, PORT), timeout=TIMEOUT) as sock:
        with context.wrap_socket(sock) as ssocket:

            auth = SerialMap(
                values={'username': USER, 'password': PASS})

            ssocket.send(auth.serialize())

            response = SerialMap(recv_until(ssocket, b'\x00', TIMEOUT))

            if 'result' in response and response['result'] == 'ok':

                ssocket.send(action.serialize())

                response = SerialMap(recv_until(ssocket, b'\x00', TIMEOUT))

                print(response)


def recv_until(socket: ssl.SSLSocket, termination: bytes, timeout: float):
    start_t = time.time()
    buffer = bytearray()

    try:
        data = b''

        while time.time() - start_t < timeout and (len(buffer) == 0 or data != termination):
            buffer.extend(data)
            data = socket.recv(1)

    except Exception as err:
        print(err)

    return bytes(buffer)


if __name__ == "__main__":
    main()
