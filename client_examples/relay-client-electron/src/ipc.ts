import { ipcMain } from 'electron';
import { State } from './App/Models/State';
import {
    CHANNEL_GET_CONFIG,
    CHANNEL_REQUEST_STATE,
    CHANNEL_RESET_DEVICE,
    CHANNEL_SAVE_CONFIG,
    CHANNEL_SET_STATE,
    CHANNEL_SET_WIFI_OPTIONS,
} from './channels';
import Communications, {
    CommunicationOptions,
    Result,
} from './Communications/Services/protocol';
import UDPBroadcastReceiver from './Communications/Services/udp-receiver';
import WifiConfiguration, {
    WifiParams,
} from './Communications/Services/wifi-connector';
import AppConfig, { ConfigurationObject } from './Config/configuration';

let bcast: UDPBroadcastReceiver;
//let comms: Communications;
let commOptions: CommunicationOptions;

export function handleIpc() {
    commOptions = {
        port: Number(AppConfig.serverPort) || 0,
        username: AppConfig.serverUser || '',
        password: AppConfig.serverPassword || '',
    };

    bcast = new UDPBroadcastReceiver(AppConfig.udpPort, false);

    bcast.subscribe((address: string) => (commOptions.host = address));

    bcast.start();

    ipcMain.handle(CHANNEL_REQUEST_STATE, handleRequestState);
    ipcMain.handle(CHANNEL_SET_STATE, handleSetState);
    ipcMain.handle(CHANNEL_RESET_DEVICE, handleResetDevice);
    ipcMain.handle(CHANNEL_SET_WIFI_OPTIONS, handleSetWifi);
    ipcMain.handle(CHANNEL_GET_CONFIG, handleGetConfiguration);
    ipcMain.handle(CHANNEL_SAVE_CONFIG, handleSaveConfiguration);
}

async function handleRequestState(event: Electron.IpcMainInvokeEvent) {
    return new Communications(commOptions).request({ action: 'get' });
}

async function handleSetState(
    event: Electron.IpcMainInvokeEvent,
    state: State
): Promise<Result | void> {
    return new Communications(commOptions).request({
        action: 'set',
        state: state.state,
    });
}

async function handleResetDevice(
    event: Electron.IpcMainInvokeEvent
): Promise<Result | void> {
    return new Communications(commOptions).request({ action: 'reset' });
}

async function handleSetWifi(
    event: Electron.IpcMainInvokeEvent,
    wifiOptions: WifiParams
): Promise<Result | void> {
    return await new WifiConfiguration().send(wifiOptions);
}

async function handleGetConfiguration(
    event: Electron.IpcMainInvokeEvent
): Promise<ConfigurationObject> {
    return AppConfig.configuration;
}

async function handleSaveConfiguration(
    event: Electron.IpcMainInvokeEvent,
    configuration: Partial<ConfigurationObject>
): Promise<void> {
    AppConfig.mergeAndSave(configuration);
}
