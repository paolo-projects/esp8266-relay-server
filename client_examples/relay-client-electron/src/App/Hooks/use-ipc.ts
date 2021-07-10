import { IpcRendererEvent } from 'electron/main';

export type IPCApi = {
    invoke: (channel: string, data?: any) => Promise<any>;
    send: (channel: string, data: any) => void;
    on: (
        channel: string,
        handler: (event: IpcRendererEvent, ...args: any) => void
    ) => void;
};

declare var window: {
    api?: IPCApi;
};

export default function useIpc(): IPCApi {
    if (window.api) {
        return window.api;
    } else throw new Error('Renderer IPC api is not in the window context');
}
