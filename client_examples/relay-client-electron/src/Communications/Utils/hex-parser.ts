export function parseHexString(str: string): Buffer {
    if (str) {
        let chunks = [];
        if (str.startsWith('\\x')) {
            let m;
            let reg = /\\x(.{2})/g;
            while ((m = reg.exec(str)) !== null) {
                chunks.push(Buffer.from([parseInt(m[1], 16)]));
            }
            return Buffer.concat(chunks);
        } else return Buffer.from(str, 'utf-8');
    } else {
        return Buffer.alloc(0);
    }
}

export function saveToHexString(data: Buffer): string {
    if (data && data.length > 0) {
        let outs = '';
        for (let i = 0; i < data.length; i++) {
            let num = data.readUInt8(i).toString(16);
            if (num.length < 2) num = '0' + num;
            outs += `\\x${num}`;
        }
        return outs;
    } else return '';
}
