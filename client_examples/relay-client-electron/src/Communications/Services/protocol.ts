import tls from 'tls';
import SerialMap from '../Data/serialmap';
import DataParser from './data-parser';
import StatusManager, { Status } from './communication-status';
import { Subscription } from 'rxjs';
import { Certificate } from '../Data/certificate';

export type CommunicationOptions = {
    host?: string;
    port: number;
    username: string;
    password: string;
};

export type Action =
    | { action: 'get' | 'reset' }
    | { action: 'set'; state: 'on' | 'off' }
    | { action: 'setwifi'; ssid: string; password: string };

export type ActionResult = { result: 'ok' | 'error' };
export type StateResult = { state: 'on' | 'off' };

export type Result = ActionResult | StateResult;

type RejectFunction = (reason?: any) => void;
type ResolveFunction = (result?: Result) => void;

const REQUEST_TIMEOUT = 10000;

export default class Communications {
    private _parser?: DataParser;
    private _statusManager?: StatusManager;
    private _address?: string;
    private _port: number;
    private _username: string;
    private _password: string;
    private _subscription?: Subscription;
    private _timeoutHandle?: number;
    private _client?: tls.TLSSocket;

    public set address(address: string) {
        this._address = address;
    }

    constructor(options: CommunicationOptions) {
        this._address = options.host;
        this._port = options.port;
        this._username = options.username;
        this._password = options.password;
    }

    request(action: Action): Promise<Result | void> {
        if (this._address) {
            return new Promise((resolve, reject) => {
                this._parser = new DataParser();
                this._statusManager = new StatusManager();

                console.log(
                    'opening connection on',
                    this._address,
                    'port',
                    this._port
                );

                if (this._subscription) {
                    this._subscription.unsubscribe();
                }
                this._subscription = this._parser.subscribe(
                    (message: Buffer) => {
                        this._statusManager?.dataReceived(message);
                    }
                );

                this._statusManager.setCallbacks({
                    authentication: () => this.onAuthenticated(resolve, action),
                    actionResult: (result: boolean) =>
                        this.onActionResult(resolve, result),
                    status: (state: 'on' | 'off') =>
                        this.onStatus(resolve, state),
                });

                this._client = tls.connect(
                    {
                        host: this._address,
                        port: this._port,
                        ca: Certificate,
                        rejectUnauthorized: true,
                        requestCert: true,
                        timeout: 3000,
                        checkServerIdentity: () => undefined,
                    },
                    () => this.onConnected(reject)
                );

                this._client.on('data', (data) => {
                    this._parser?.push(data);
                });

                this._client.on('error', (exc) => {
                    reject(exc);
                });

                this._client.on('close', () => {
                    //
                });
            });
        } else {
            return Promise.reject(new Error('No address provided'));
        }
    }

    timeoutError(reject: RejectFunction) {
        reject(new Error('Request timeout exceeded'));
    }

    onConnected(reject: RejectFunction) {
        this._timeoutHandle = setTimeout(
            () => this.timeoutError(reject),
            REQUEST_TIMEOUT
        ) as any as number;
        console.log('sending auth');

        const auth = new SerialMap();
        auth.put('username', this._username);
        auth.put('password', this._password);
        this._client?.write(auth.serialize());
    }

    onAuthenticated(resolve: ResolveFunction, action: Action) {
        console.log('authenticated. sending action');

        if (action.action === 'setwifi' && action.ssid && action.password) {
            const act = new SerialMap();
            act.put('action', 'setwifi');
            act.put('bssid', action.ssid);
            act.put('password', action.password);
            this._client?.write(act.serialize());
            this._client?.end();
            resolve();
        } else if (action.action === 'get') {
            // get is default
            const act = new SerialMap();
            act.put('action', 'getstate');
            this._client?.write(act.serialize());
        } else if (action.action === 'set') {
            const act = new SerialMap();
            act.put('action', 'setstate');
            act.put('state', action.state);
            this._client?.write(act.serialize());
        } else if (action.action === 'reset') {
            const act = new SerialMap();
            act.put('action', 'disconnect');
            this._client?.write(act.serialize());
        } else {
            console.log('No command supplied.');
            this._client?.end();
            throw new Error('No valid action supplied');
        }
    }

    onActionResult(resolve: ResolveFunction, result: boolean) {
        clearTimeout(this._timeoutHandle);
        this._client?.end();
        resolve({ result: result ? 'ok' : 'error' });
    }

    onStatus(resolve: ResolveFunction, state: 'on' | 'off') {
        clearTimeout(this._timeoutHandle);
        this._client?.end();
        resolve({ state });
    }
}
