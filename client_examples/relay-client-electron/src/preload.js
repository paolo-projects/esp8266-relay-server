const { contextBridge, ipcRenderer } = require('electron');

const CHANNELS_ALLOWED = [
    'relayserver_request_state',
    'relayserver_send_state',
    'relayserver_reset_device',
    'relayserver_set_wifi_options',
    'relayserver_renderer_state_received',
];

contextBridge.exposeInMainWorld('api', {
    invoke: (channel, data) => {
        if (CHANNELS_ALLOWED.indexOf(channel) >= 0) {
            return ipcRenderer.invoke(channel, data);
        }
    },
    send: (channel, data) => {
        if (CHANNELS_ALLOWED.indexOf(channel) >= 0) {
            ipcRenderer.send(channel, data);
        }
    },
    on: (channel, handler) => {
        if (CHANNELS_ALLOWED.indexOf(channel) >= 0) {
            ipcRenderer.on(channel, handler);
        }
    },
});
