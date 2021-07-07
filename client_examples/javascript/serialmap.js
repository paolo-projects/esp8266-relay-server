class SerialMap {
    KEY_TYPE = 0x10;
    VALUE_TYPE = 0x11;
    /**
     * @type {[key:string]: string}
     */
    data = {};

    /**
     *
     * @param {Buffer} data
     */
    constructor(data = null) {
        if (data != null) {
            let cursor = 0;

            while (cursor < data.length) {
                if (data.readUInt8(cursor++) != this.KEY_TYPE) {
                    return;
                }
                let sz = data.readUInt8(cursor++);
                const key = data.slice(cursor, cursor + sz).toString("utf-8");
                cursor += sz;
                if (data.readUInt8(cursor++) != this.VALUE_TYPE) {
                    return;
                }
                sz = data.readUInt8(cursor++);
                const value = data.slice(cursor, cursor + sz).toString("utf-8");
                cursor += sz;

                this.data[key] = value;
            }
        }
    }

    /**
     *
     * @param {string} key
     * @param {string} value
     */
    put(key, value) {
        this.data[key] = value;
    }

    /**
     *
     * @param {string} key
     * @returns {string} The value
     */
    get(key) {
        return this.data[key];
    }

    /**
     *
     * @param {string} key
     * @returns {boolean} If the key exists
     */
    has(key) {
        return key in this.data;
    }

    /**
     * @returns {Buffer} The serialized buffer
     */
    serialize() {
        let size = 1;
        for (const [key, value] of Object.entries(this.data)) {
            size += key.length + value.length + 4;
        }

        let cursor = 0;
        const buf = Buffer.alloc(size);
        for (const [key, value] of Object.entries(this.data)) {
            cursor = buf.writeUInt8(this.KEY_TYPE, cursor);
            cursor = buf.writeUInt8(key.length, cursor);
            cursor += buf.write(key, cursor, "utf8");
            cursor = buf.writeUInt8(this.VALUE_TYPE, cursor);
            cursor = buf.writeUInt8(value.length, cursor);
            cursor += buf.write(value, cursor, "utf8");
        }

        cursor = buf.writeUInt8(0, cursor);

        return buf;
    }
}

module.exports = SerialMap;
