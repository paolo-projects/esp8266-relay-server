/**
 * @typedef {((data:Buffer)=>void)} MessageListener
 */

class DataParser {
    data = Buffer.alloc(0);
    /**
     * @type {MessageListener[]}
     */
    listeners = [];
    /**
     *
     * @param {Buffer} data
     */
    push(data) {
        this.data = Buffer.concat([this.data, data]);
        if (this.data.readUInt8(this.data.length - 1) === 0x00) {
            // Send complete packet to all listeners
            this.listeners.forEach((listener) => listener(this.data));
            // Reset buffer
            this.data = Buffer.alloc(0);
        }
    }
    /**
     *
     * @param {MessageListener} listener
     */
    addMessageListener(listener) {
        this.listeners.push(listener);
    }
}

module.exports = DataParser;
