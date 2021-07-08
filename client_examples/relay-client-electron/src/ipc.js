const { ipcMain, ipcRenderer, contextBridge } = require('electron');

function handleIpc() {
    ipcMain.handle();

    contextBridge.exposeInMainWorld('ipc', ipcRenderer);
}

module.exports = {
    handleIpc,
};
