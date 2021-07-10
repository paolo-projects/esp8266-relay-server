import SerialMap from '../Data/serialmap';

export type Status = 'not-authenticated' | 'authenticated' | 'completed';
export type AuthenticationCallback = () => void;
export type StatusCallback = (status: 'on' | 'off') => void;
export type ActionResultCallback = (result: boolean) => void;
export type Callbacks = {
    authentication: AuthenticationCallback;
    status: StatusCallback;
    actionResult: ActionResultCallback;
};

export default class StatusManager {
    status: Status = 'not-authenticated';
    callbacks: Callbacks = {
        authentication: () => {},
        status: () => {},
        actionResult: () => {},
    };

    setCallbacks(callbacks: Callbacks) {
        this.callbacks = callbacks;
    }

    dataReceived(data: Buffer) {
        const response = new SerialMap(data);
        if (this.status === 'not-authenticated') {
            if (response.has('result') && response.get('result') === 'ok') {
                this.status = 'authenticated';
                this.callbacks.authentication();
            }
        } else if (this.status === 'authenticated') {
            this.status = 'completed';
            if (response.has('result')) {
                this.callbacks.actionResult(response.get('result') === 'ok');
            } else if (response.has('state')) {
                this.callbacks.status(
                    response.get('state') === 'on' ? 'on' : 'off'
                );
            }
        }
    }
}
