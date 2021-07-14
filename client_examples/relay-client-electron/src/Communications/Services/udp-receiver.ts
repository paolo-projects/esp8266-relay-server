import dgram from 'dgram';
import { Subject } from 'rxjs';
import AppConfig from '../../Config/configuration';

export default class UDPBroadcastReceiver {
    address = new Subject<string>();
    port: number;
    listener: dgram.Socket;
    message: Buffer;

    constructor(port: number, closeWhenDone = true) {
        this.port = port;
        this.listener = dgram.createSocket('udp4');
        this.message = AppConfig.udpPacket;

        this.listener.on('message', (msg, rinfo) => {
            if (msg.equals(this.message)) {
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
