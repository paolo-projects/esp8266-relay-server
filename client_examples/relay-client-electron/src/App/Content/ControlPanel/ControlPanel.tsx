import React, { Dispatch, SetStateAction, useEffect, useState } from 'react';
import { RelayStatus } from '../../Models/RelayStatus';
import './control-panel.scss';
import oven_off from '../../../../assets/oven_off.svg';
import oven_on from '../../../../assets/oven_on.svg';
import oven_undef from '../../../../assets/oven_undef.svg';
import { CHANNEL_REQUEST_STATE, CHANNEL_SET_STATE } from '../../../channels';
import { State } from '../../Models/State';
import useIpc from '../../Hooks/use-ipc';
import { StateResult } from '../../../Communications/Services/protocol';
import { ClipLoader } from 'react-spinners';

const INTERVAL = 10000;

export default function ControlPanel() {
    const [relayStatus, setRelayStatus] = useState(RelayStatus.UNKNOWN);
    const [awaitingStatus, setAwaitingStatus] = useState(true);

    const ipc = useIpc();

    useEffect(() => {
        function pollForStatus() {
            setAwaitingStatus(true);
            ipc.invoke(CHANNEL_REQUEST_STATE)
                .then((state: StateResult) => {
                    updateState(state);
                })
                .catch((err) => {
                    console.error('Error occurred during request', err);
                })
                .finally(() => {
                    if (!destroying) {
                        setAwaitingStatus(false);
                        timer = setTimeout(() => pollForStatus(), INTERVAL);
                    }
                });
        }

        //setStatusPoll(setInterval(() => pollForStatus(), INTERVAL));
        let timer = setTimeout(() => pollForStatus(), 0);
        let destroying = false;

        return () => {
            destroying = true;
            clearTimeout(timer);
        };
    }, []);

    const updateState = (state: State) => {
        switch (state.state) {
            case 'on':
                setRelayStatus(RelayStatus.ON);
                break;
            case 'off':
                setRelayStatus(RelayStatus.OFF);
                break;
            default:
                setRelayStatus(RelayStatus.UNKNOWN);
                break;
        }
    };

    const relayStatusMapper = (status: RelayStatus) => {
        switch (status) {
            case RelayStatus.OFF:
                return oven_off;
            case RelayStatus.ON:
                return oven_on;
            default:
            case RelayStatus.UNKNOWN:
                return oven_undef;
        }
    };

    const relayStatusText = (status: RelayStatus) => {
        switch (status) {
            case RelayStatus.OFF:
                return 'OFF';
            case RelayStatus.ON:
                return 'ON';
            default:
            case RelayStatus.UNKNOWN:
                return 'UNKNOWN';
        }
    };

    const changeStateClick = () => {
        setAwaitingStatus(true);

        if (relayStatus !== RelayStatus.UNKNOWN) {
            ipc.invoke(CHANNEL_SET_STATE, {
                state: relayStatus === RelayStatus.ON ? 'off' : 'on',
            });
        }
    };

    return (
        <div className="relay-status">
            <button
                className="btn btn-primary relay-status-btn"
                onClick={changeStateClick}
                disabled={awaitingStatus}
            >
                <img
                    className="relay-status-img"
                    src={relayStatusMapper(relayStatus)}
                />
            </button>
            <div>{relayStatusText(relayStatus)}</div>
            <div className="loading-spinner">
                <ClipLoader loading={awaitingStatus} color="#ffffff" />
            </div>
        </div>
    );
}
