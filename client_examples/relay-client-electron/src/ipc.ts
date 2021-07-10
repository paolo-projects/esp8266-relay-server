import { ipcMain } from 'electron';
import { State } from './App/Models/State';
import {
    CHANNEL_REQUEST_STATE,
    CHANNEL_RESET_DEVICE,
    CHANNEL_R_STATE_RECEIVED,
    CHANNEL_SET_STATE,
    CHANNEL_SET_WIFI_OPTIONS,
} from './channels';
import ConnectionManager from './Communications/connection-manager';
import Communications, {
    CommunicationOptions,
    Result,
    StateResult,
} from './Communications/Services/protocol';
import UDPBroadcastReceiver from './Communications/Services/udp-receiver';
import WifiConfiguration, {
    WifiParams,
} from './Communications/Services/wifi-connector';

let bcast: UDPBroadcastReceiver;
//let comms: Communications;
let commOptions: CommunicationOptions;

export function handleIpc() {
    commOptions = {
        port: Number(process.env.SRV_PORT) || 0,
        username: process.env.SRV_USERNAME || '',
        password: process.env.SRV_PASSWORD || '',
    };

    bcast = new UDPBroadcastReceiver(
        parseInt(process.env.UDP_PORT || ''),
        false
    );

    bcast.subscribe((address: string) => (commOptions.host = address));

    bcast.start();

    ipcMain.handle(CHANNEL_REQUEST_STATE, handleRequestState);
    ipcMain.handle(CHANNEL_SET_STATE, handleSetState);
    ipcMain.handle(CHANNEL_RESET_DEVICE, handleResetDevice);
    ipcMain.handle(CHANNEL_SET_WIFI_OPTIONS, handleSetWifi);
}

async function handleRequestState(event: Electron.IpcMainInvokeEvent) {
    return new Communications(commOptions).request({ action: 'get' });
}

/**
 *
 * @param {Electron.IpcMainInvokeEvent} event
 * @param  {State} state
 * @returns {Result}
 */
async function handleSetState(
    event: Electron.IpcMainInvokeEvent,
    state: State
): Promise<Result | void> {
    return new Communications(commOptions).request({
        action: 'set',
        state: state.state,
    });
}

/**
 *
 * @param {Electron.IpcMainInvokeEvent} event
 * @returns {Result}
 */
async function handleResetDevice(
    event: Electron.IpcMainInvokeEvent
): Promise<Result | void> {
    return new Communications(commOptions).request({ action: 'reset' });
}

/**
 *
 * @param {Electron.IpcMainInvokeEvent} event
 * @param {WifiConfiguration.WifiParams} wifiOptions
 * @returns
 */
async function handleSetWifi(
    event: Electron.IpcMainInvokeEvent,
    wifiOptions: WifiParams
): Promise<Result | void> {
    return await new WifiConfiguration().send(wifiOptions);
}
