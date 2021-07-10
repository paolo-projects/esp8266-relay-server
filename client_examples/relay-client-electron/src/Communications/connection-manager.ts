import Communications, {
    ActionResult,
    CommunicationOptions,
    StateResult,
} from './Services/protocol';

type Task = {
    executor: () => void;
    started: boolean;
    completed: boolean;
};

const STATE_REQUESTS_INTERVAL = 10000;
const TASK_RUNNER_INTERVAL = 500;

export default class ConnectionManager {
    private _connection: Communications;
    private _state: StateResult = { state: 'off' };
    private _pool: Task[] = [];
    private _lastStateRequest = 0;
    private _taskRunnerInterval?: NodeJS.Timer;
    private _lastRequestStateCompleted = true;

    public get state(): StateResult {
        return this._state;
    }

    constructor(options: CommunicationOptions) {
        this._connection = new Communications(options);
        this._taskRunnerInterval = setInterval(
            () => this.taskRunner(),
            TASK_RUNNER_INTERVAL
        );
    }

    taskRunner() {
        if (this._lastRequestStateCompleted) {
            if (this._pool.length > 0) {
                if (!this._pool[0].started) {
                    this._pool[0].executor();
                    this._pool[0].started = true;
                } else if (this._pool[0].completed) {
                    this._pool.splice(0, 1);
                    this.taskRunner();
                }
            } else if (
                Date.now() >
                this._lastStateRequest + STATE_REQUESTS_INTERVAL
            ) {
                if (this._lastStateRequest === 0) {
                    this._lastStateRequest = Date.now();
                }
                this.requestState();
            }
        }
    }

    requestState() {
        this._lastRequestStateCompleted = false;
        (
            this._connection.request({
                action: 'get',
            }) as Promise<StateResult>
        )
            .then((state) => {
                console.log('received state', state);
                this._state = state;
            })
            .catch((err) => {
                console.error('request state error', err);
            })
            .finally(() => {
                this._lastStateRequest = Date.now();
                this._lastRequestStateCompleted = true;
            });
    }

    setState(state: 'on' | 'off') {
        const connectionManager = this;
        this._pool.push({
            executor() {
                (
                    connectionManager._connection.request({
                        action: 'set',
                        state,
                    }) as Promise<ActionResult>
                )
                    .catch((exc) =>
                        console.error('error while executing task', exc)
                    )
                    .finally(() => (this.completed = true));
            },
            started: false,
            completed: false,
        });
    }

    resetDevice() {
        const connectionManager = this;
        this._pool.push({
            executor() {
                (
                    connectionManager._connection.request({
                        action: 'reset',
                    }) as Promise<ActionResult>
                )
                    .catch((exc) =>
                        console.error('error while executing task', exc)
                    )
                    .finally(() => (this.completed = true));
            },
            started: false,
            completed: false,
        });
    }

    setAddress(address: string) {
        this._connection.address = address;
    }
}
