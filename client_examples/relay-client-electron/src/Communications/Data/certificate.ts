import fs from 'fs';
import path from 'path';
import cert from '../.cert.pem';

export const Certificate = fs.readFileSync(path.join(__dirname, cert));
