import { Subject } from 'rxjs';

export type MessageListener = (data: Buffer) => void;

export default class DataParser {
    private data = Buffer.alloc(0);
    private dataReceived = new Subject<Buffer>();

    push(data: Buffer) {
        console.log('read data', data);

        this.data = Buffer.concat([this.data, data]);
        if (this.data.readUInt8(this.data.length - 1) === 0x00) {
            // Send full packet to all listeners
            this.dataReceived.next(this.data);
            // Reset buffer
            this.data = Buffer.alloc(0);
            console.log('resetting buffer');
        }
    }

    subscribe(observer: (data: Buffer) => void) {
        return this.dataReceived.subscribe(observer);
    }
}
