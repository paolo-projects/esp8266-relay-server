import AppConfig from '../../Config/configuration';
import Communications from './protocol';

export type WifiParams = { ssid: string; password: string };

export default class WifiSettings {
    send(settings: WifiParams) {
        // The automatic WiFi connection to the AP endpoint is not easy to implement using nodeJS,
        // requiring executing shell commands that are OS dependent and often require admin/root
        // privileges. An alternative would be calling the OS APIs directly, but it's a lot of
        // cross-platform work, not suited for nodejs, and admin/root privileges are still required
        // The solution is to make the user manually connect to the AP wifi and we only send the command

        const comms = new Communications({
            host: AppConfig.apIpAddr,
            port: AppConfig.apServerPort,
            username: AppConfig.apAuthUser,
            password: AppConfig.apAuthPass,
        });

        return comms.request({
            ...settings,
            action: 'setwifi',
        });
    }
}
