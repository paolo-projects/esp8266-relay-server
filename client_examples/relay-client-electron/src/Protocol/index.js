const tls = require("tls"),
    fs = require("fs"),
    yargs = require("yargs/yargs"),
    { hideBin } = require("yargs/helpers"),
    SerialMap = require("./serialmap"),
    DataParser = require("./data-parser"),
    StatusManager = require("./communication-status");

require("dotenv").config();

const HOST = process.env.SRV_ADDR;
const PORT = process.env.SRV_PORT;
const USER = process.env.SRV_USERNAME;
const PASS = process.env.SRV_PASSWORD;

const args = yargs(hideBin(process.argv))
    .demandCommand()
    .strict()
    .command("get", "Get the relay state", {}, (argv) => (argv.get = true))
    .command(
        "set [state]",
        "Set the relay state",
        (yargs) => {
            yargs
                .positional("state", {
                    type: "string",
                    choices: ["on", "off"],
                    describe: "The relay state",
                })
                .demandOption("state");
        },
        (argv) => (argv.set = true)
    )
    .command("reset", "Switch the relay to AP mode", {}, (argv) => (argv.reset = true)).argv;

const parser = new DataParser();
const statusManager = new StatusManager();

parser.addMessageListener((message) => {
    statusManager.dataReceived(message);
});

const client = tls.connect(
    PORT,
    HOST,
    {
        ca: fs.readFileSync("./.cert.pem"),
        rejectUnauthorized: true,
        requestCert: true,
        timeout: 3000,
        checkServerIdentity: () => undefined,
    },
    () => {
        const auth = new SerialMap();
        auth.put("username", USER);
        auth.put("password", PASS);
        const data = auth.serialize();
        client.write(auth.serialize());
    }
);

client.on("data", (data) => {
    parser.push(data);
});

client.on("close", () => {
    //
});

statusManager.setCallbacks({
    authentication: () => {
        if (args.get) {
            // get is default
            const action = new SerialMap();
            action.put("action", "getstate");
            client.write(action.serialize());
        } else if (args.set) {
            const action = new SerialMap();
            action.put("action", "setstate");
            action.put("state", args.state);
            client.write(action.serialize());
        } else if (args.reset) {
            const action = new SerialMap();
            action.put("action", "disconnect");
            client.write(action.serialize());
        } else {
            console.log("No command supplied.");
            client.end();
        }
    },
    actionResult: (result) => {
        if (result) {
            console.log("Command executed successfully");
        } else {
            console.log("Error while executing command");
        }
    },
    status: (status) => {
        console.log("Relay status: " + status);
    },
});
