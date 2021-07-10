export type DataObject = { [key: string]: string };

export default class SerialMap {
    private KEY_TYPE = 0x10;
    private VALUE_TYPE = 0x11;
    private data: DataObject = {};

    constructor(data: Buffer | null = null) {
        if (data != null) {
            let cursor = 0;

            while (cursor < data.length) {
                if (data.readUInt8(cursor++) != this.KEY_TYPE) {
                    return;
                }
                let sz = data.readUInt8(cursor++);
                const key = data.slice(cursor, cursor + sz).toString('utf-8');
                cursor += sz;
                if (data.readUInt8(cursor++) != this.VALUE_TYPE) {
                    return;
                }
                sz = data.readUInt8(cursor++);
                const value = data.slice(cursor, cursor + sz).toString('utf-8');
                cursor += sz;

                this.data[key] = value;
            }
        }
    }

    put(key: string, value: string) {
        this.data[key] = value;
    }

    get(key: string): string {
        return this.data[key];
    }

    has(key: string): boolean {
        return key in this.data;
    }

    serialize(): Buffer {
        let size = 1;
        for (const [key, value] of Object.entries(this.data)) {
            size += key.length + value.length + 4;
        }

        let cursor = 0;
        const buf = Buffer.alloc(size);
        for (const [key, value] of Object.entries(this.data)) {
            cursor = buf.writeUInt8(this.KEY_TYPE, cursor);
            cursor = buf.writeUInt8(key.length, cursor);
            cursor += buf.write(key, cursor, 'utf8');
            cursor = buf.writeUInt8(this.VALUE_TYPE, cursor);
            cursor = buf.writeUInt8(value.length, cursor);
            cursor += buf.write(value, cursor, 'utf8');
        }

        cursor = buf.writeUInt8(0, cursor);

        return buf;
    }
}
