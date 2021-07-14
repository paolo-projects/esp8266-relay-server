import electron from 'electron';
import fs from 'fs';
import path from 'path';
import yaml from 'yaml';
import {
    parseHexString,
    saveToHexString,
} from '../Communications/Utils/hex-parser';
import defaultConfig from '../../config.default.yaml';

const CONFIG_FILE = 'configuration.yaml';
const CONFIG_KEYS = {
    udpPacket: 'udp_packet',
    apIpAddr: 'ap_ip_address',
    apServerPort: 'ap_server_port',
    apAuthUser: 'ap_auth_user',
    apAuthPass: 'ap_auth_password',
    serverPort: 'server_port',
    serverUser: 'server_user',
    serverPassword: 'server_password',
    udpPort: 'udp_port',
};

export type ConfigurationObject = typeof CONFIG_KEYS;

class Configuration {
    private yamlConfig: { [key: string]: string } = {};

    // GETTERS
    get udpPacket(): Buffer {
        return parseHexString(
            this.yamlConfig[CONFIG_KEYS.udpPacket] || this._defUdpPacket
        );
    }
    get apIpAddr(): string {
        return this.yamlConfig[CONFIG_KEYS.apIpAddr] || this._defApIpAddr;
    }
    get apServerPort(): number {
        return Number(
            this.yamlConfig[CONFIG_KEYS.apServerPort] || this._defApServerPort
        );
    }
    get apAuthUser(): string {
        return this.yamlConfig[CONFIG_KEYS.apAuthUser] || this._defApAuthUser;
    }
    get apAuthPass(): string {
        return this.yamlConfig[CONFIG_KEYS.apAuthPass] || this._defApAuthPass;
    }
    get serverPort(): number {
        return Number(
            this.yamlConfig[CONFIG_KEYS.serverPort] || this._defServerPort
        );
    }
    get serverUser(): string {
        return this.yamlConfig[CONFIG_KEYS.serverUser] || this._defServerUser;
    }
    get serverPassword(): string {
        return (
            this.yamlConfig[CONFIG_KEYS.serverPassword] || this._defServerPass
        );
    }
    get udpPort(): number {
        return Number(this.yamlConfig[CONFIG_KEYS.udpPort] || this._defUdpPort);
    }

    get configuration(): ConfigurationObject {
        return {
            apIpAddr: this.apIpAddr,
            apAuthUser: this.apAuthUser,
            apAuthPass: this.apAuthPass,
            apServerPort: this.apServerPort.toString(),
            serverUser: this.serverUser,
            serverPassword: this.serverPassword,
            serverPort: this.serverPort.toString(),
            udpPacket: saveToHexString(this.udpPacket),
            udpPort: this.udpPort.toString(),
        };
    }

    // SETTERS
    set udpPacket(packet: Buffer) {
        this.yamlConfig[CONFIG_KEYS.udpPacket] = saveToHexString(packet);
    }
    set apIpAddr(ipAddr: string) {
        this.yamlConfig[CONFIG_KEYS.apIpAddr] = ipAddr;
    }
    set apServerPort(port: number) {
        this.yamlConfig[CONFIG_KEYS.apServerPort] = port.toString();
    }
    set apAuthUser(apUser: string) {
        this.yamlConfig[CONFIG_KEYS.apAuthUser] = apUser;
    }
    set apAuthPass(apPass: string) {
        this.yamlConfig[CONFIG_KEYS.apAuthPass] = apPass;
    }
    set serverPort(serverPort: number) {
        this.yamlConfig[CONFIG_KEYS.serverPort] = serverPort.toString();
    }
    set serverUser(serverUser: string) {
        this.yamlConfig[CONFIG_KEYS.serverUser] = serverUser;
    }
    set serverPassword(serverPass: string) {
        this.yamlConfig[CONFIG_KEYS.serverPassword] = serverPass;
    }
    set udpPort(udpPort: number) {
        this.yamlConfig[CONFIG_KEYS.udpPort] = udpPort.toString();
    }

    private _defUdpPacket = process.env.UDP_PACKET || '';
    private _defApIpAddr = process.env.AP_IP_ADDR || '';
    private _defApServerPort = process.env.AP_SERVER_PORT || '';
    private _defApAuthUser = process.env.AP_AUTH_USER || '';
    private _defApAuthPass = process.env.AP_AUTH_PASS || '';
    private _defServerPort = process.env.SRV_PORT || '';
    private _defServerUser = process.env.SRV_USERNAME || '';
    private _defServerPass = process.env.SRV_PASSWORD || '';
    private _defUdpPort = process.env.UDP_PORT || '';

    constructor() {
        const defaultYamlPath = path.join(__dirname, defaultConfig);

        if (
            fs.existsSync(defaultYamlPath) &&
            fs.lstatSync(defaultYamlPath).isFile()
        ) {
            this.yamlConfig = yaml.parse(
                fs.readFileSync(defaultYamlPath, 'utf-8')
            );
        }
        this.reload();
    }

    save() {
        const appPath = electron.remote
            ? electron.remote.app.getAppPath()
            : electron.app.getAppPath();

        const configPath = path.join(appPath, CONFIG_FILE);

        fs.writeFileSync(configPath, yaml.stringify(this.yamlConfig));
    }

    reload() {
        const appPath = electron.remote
            ? electron.remote.app.getAppPath()
            : electron.app.getAppPath();

        const configPath = path.join(appPath, CONFIG_FILE);

        if (fs.existsSync(configPath) && fs.lstatSync(configPath).isFile()) {
            this.yamlConfig = {
                ...this.yamlConfig,
                ...yaml.parse(fs.readFileSync(configPath).toString('utf-8')),
            };
        }
    }

    mergeAndSave(configuration: Partial<ConfigurationObject>) {
        Object.entries(configuration).forEach(([key, value]) => {
            const confKey = (CONFIG_KEYS as any)[key];
            if (confKey) {
                this.yamlConfig[confKey as string] = value;
            }
        });
        this.save();
    }

    private getKeyByValue(object: any, value: string): string | undefined {
        return Object.keys(object).find((key) => object[key] === value);
    }
}

const AppConfig = new Configuration();

export default AppConfig;
