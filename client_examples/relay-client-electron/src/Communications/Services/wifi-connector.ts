import wifi from 'node-wifi';
import Communications, { Action, Result } from './protocol';

/**
 * @typedef
 */
export type WifiParams = { ssid: string; password: string };

export default class WifiSettings {
    constructor() {
        wifi.init({ iface: null });
    }

    /**
     *
     * @param {WifiParams} settings
     * @returns {Promise}
     */
    send(settings: WifiParams) {
        // This library requires the Wifi to not be hidden
        // because it first scans the available networks and then connects
        // only if it finds the given one
        //
        // Instead of searching for an alterative or modifying the library,
        // I just changed the wifi from hidden to publicly accessible as it's
        // still secured by WPA
        console.log('wifi settings', settings);

        return wifi
            .connect({
                ssid: process.env.AP_SSID,
                password: process.env.AP_PASS,
            })
            .then(() => {
                console.log('connected. sending data');
                return this.connectionCallback({
                    ...settings,
                    action: 'setwifi',
                });
            })
            .then((result: void | Result) => {
                wifi.disconnect();
                return result;
            });
    }

    /**
     *
     * @param {WifiParams} settings
     */
    connectionCallback(settings: Action): Promise<void | Result> {
        console.log('opening connection');

        const comms = new Communications({
            host: process.env.AP_IP_ADDR || '',
            port: Number(process.env.AP_SERVER_PORT) || 0,
            username: process.env.AP_AUTH_USER || '',
            password: process.env.AP_AUTH_PASS || '',
        });

        return comms.request(settings);
    }
}
