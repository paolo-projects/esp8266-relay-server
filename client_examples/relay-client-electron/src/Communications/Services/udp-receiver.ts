import dgram from 'dgram';
import { Subject } from 'rxjs';

export default class UDPBroadcastReceiver {
    address = new Subject<string>();
    port: number;
    listener: dgram.Socket;

    constructor(port: number, closeWhenDone = true) {
        this.port = port;
        this.listener = dgram.createSocket('udp4');

        this.listener.on('message', (msg, rinfo) => {
            if (msg.toString('utf8') === process.env.UDP_PACKET) {
                this.address.next(rinfo.address);
                if (closeWhenDone) {
                    this.listener.close();
                }
            }
        });

        this.listener.on('listening', () => {
            console.log(
                'Listening for incoming UDP packets on port ' + this.port
            );
        });
    }

    subscribe(receiver: (address: string) => void) {
        return this.address.subscribe(receiver);
    }

    start() {
        console.log('starting udp listener');

        this.listener.bind(this.port);
    }

    stop() {
        this.listener.close();
    }
}
