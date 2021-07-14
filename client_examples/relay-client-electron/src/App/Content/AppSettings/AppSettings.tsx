import React, { ChangeEvent, useEffect, useState } from 'react';
import { CHANNEL_GET_CONFIG, CHANNEL_SAVE_CONFIG } from '../../../channels';
import { ConfigurationObject } from '../../../Config/configuration';
import useIpc from '../../Hooks/use-ipc';
import './app-settings.scss';

export default function AppSettings() {
    const [isChanged, setIsChanged] = useState(false);
    const [configuration, setConfiguration]: [
        Partial<ConfigurationObject>,
        Function
    ] = useState({});

    const ipc = useIpc();

    useEffect(() => {
        ipc.invoke(CHANNEL_GET_CONFIG).then((config) =>
            setConfiguration(config as ConfigurationObject)
        );
    }, []);

    const withTriggerChanged =
        (func: Function) => (e: ChangeEvent<HTMLInputElement>) => {
            if (!isChanged) setIsChanged(true);
            func(e);
        };

    const apIpChange = withTriggerChanged(
        (e: ChangeEvent<HTMLInputElement>) =>
            (configuration.apIpAddr = e.target.value)
    );

    const apUserChange = withTriggerChanged(
        (e: ChangeEvent<HTMLInputElement>) =>
            (configuration.apAuthUser = e.target.value)
    );

    const apPassChange = withTriggerChanged(
        (e: ChangeEvent<HTMLInputElement>) =>
            (configuration.apAuthPass = e.target.value)
    );

    const apPortChange = withTriggerChanged(
        (e: ChangeEvent<HTMLInputElement>) =>
            (configuration.apServerPort = e.target.value)
    );

    const srvUserChange = withTriggerChanged(
        (e: ChangeEvent<HTMLInputElement>) =>
            (configuration.serverUser = e.target.value)
    );

    const srvPassChange = withTriggerChanged(
        (e: ChangeEvent<HTMLInputElement>) =>
            (configuration.serverPassword = e.target.value)
    );

    const srvPortChange = withTriggerChanged(
        (e: ChangeEvent<HTMLInputElement>) =>
            (configuration.serverPort = e.target.value)
    );

    const udpPortChange = withTriggerChanged(
        (e: ChangeEvent<HTMLInputElement>) =>
            (configuration.udpPort = e.target.value)
    );

    const udpPacketChange = withTriggerChanged(
        (e: ChangeEvent<HTMLInputElement>) =>
            (configuration.udpPacket = e.target.value)
    );

    const saveChanges = () => {
        ipc.invoke(CHANNEL_SAVE_CONFIG, configuration);
        setIsChanged(false);
    };

    return (
        <div className="settings-container">
            <div className="mb-2">
                <label htmlFor="ap_ip_addr">Access Point IP Address</label>
                <input
                    type="text"
                    className="form-control"
                    id="ap_ip_addr"
                    defaultValue={configuration.apIpAddr}
                    onChange={(e) => apIpChange(e)}
                />
            </div>
            <div className="mb-2">
                <label htmlFor="ap_port">Access Point Port</label>
                <input
                    type="text"
                    className="form-control"
                    id="ap_port"
                    defaultValue={configuration.apServerPort}
                    onChange={(e) => apPortChange(e)}
                />
            </div>
            <div className="mb-2">
                <label htmlFor="ap_user">Access Point Username</label>
                <input
                    type="text"
                    className="form-control"
                    id="ap_user"
                    defaultValue={configuration.apAuthUser}
                    onChange={(e) => apUserChange(e)}
                />
            </div>
            <div className="mb-2">
                <label htmlFor="ap_pass">Access Point Password</label>
                <input
                    type="text"
                    className="form-control"
                    id="ap_pass"
                    defaultValue={configuration.apAuthPass}
                    onChange={(e) => apPassChange(e)}
                />
            </div>
            <div className="mb-2">
                <label htmlFor="server_port">Main Server Port</label>
                <input
                    type="text"
                    className="form-control"
                    id="server_port"
                    defaultValue={configuration.serverPort}
                    onChange={(e) => srvPortChange(e)}
                />
            </div>
            <div className="mb-2">
                <label htmlFor="server_user">Main Server Username</label>
                <input
                    type="text"
                    className="form-control"
                    id="server_user"
                    defaultValue={configuration.serverUser}
                    onChange={(e) => srvUserChange(e)}
                />
            </div>
            <div className="mb-2">
                <label htmlFor="server_pass">Main Server Password</label>
                <input
                    type="text"
                    className="form-control"
                    id="server_pass"
                    defaultValue={configuration.serverPassword}
                    onChange={(e) => srvPassChange(e)}
                />
            </div>
            <div className="mb-2">
                <label htmlFor="udp_port">UDP Broadcaster Port</label>
                <input
                    type="text"
                    className="form-control"
                    id="udp_port"
                    defaultValue={configuration.udpPort}
                    onChange={(e) => udpPortChange(e)}
                />
            </div>
            <div className="mb-2">
                <label htmlFor="udp_packet">UDP Broadcaster Packet</label>
                <input
                    type="text"
                    className="form-control"
                    id="udp_packet"
                    defaultValue={configuration.udpPacket}
                    onChange={(e) => udpPacketChange(e)}
                />
            </div>
            <div>
                <button
                    className="btn btn-primary"
                    disabled={!isChanged}
                    onClick={() => saveChanges()}
                >
                    Save
                </button>
            </div>
        </div>
    );
}
