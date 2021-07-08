/**
 * @typedef {("not-authenticated" | "authenticated" | "completed")} Status
 * @typedef {() => void} AuthenticationCallback
 * @typedef {(status: string) => void} StatusCallback
 * @typedef {(result: boolean) => void} ActionResultCallback
 * @typedef {{authentication: AuthenticationCallback; status: StatusCallback; actionResult: ActionResultCallback}} Callbacks
 */

const SerialMap = require("./serialmap");

class StatusManager {
    /**
     * @type {Status}
     */
    status = "not-authenticated";

    /**
     * @type {Callbacks}
     */
    callbacks = {
        authentication: () => {},
        status: () => {},
        actionResult: () => {},
    };

    /**
     *
     * @param {Callbacks} callbacks
     */
    setCallbacks(callbacks) {
        this.callbacks = callbacks;
    }

    /**
     *
     * @param {Buffer} data
     */
    dataReceived(data) {
        const response = new SerialMap(data);
        if (this.status === "not-authenticated") {
            if (response.has("result") && response.get("result") === "ok") {
                this.status = "authenticated";
                this.callbacks.authentication();
            }
        } else if (this.status === "authenticated") {
            this.status = "completed";
            if (response.has("result")) {
                this.callbacks.actionResult(response.get("result") === "ok");
            } else if (response.has("state")) {
                this.callbacks.status(response.get("state"));
            }
        }
    }
}

module.exports = StatusManager;
