import React, { useState } from 'react';
import { RelayStatus } from '../../Models/RelayStatus';
import './control-panel.scss';
import oven_off from '../../../../assets/oven_off.svg';
import oven_on from '../../../../assets/oven_on.svg';
import oven_undef from '../../../../assets/oven_undef.svg';

export default function ControlPanel() {
    const [relayStatus, setRelayStatus] = useState(RelayStatus.UNKNOWN);

    const relayStatusMapper = (status: RelayStatus) => {
        switch (status) {
            case RelayStatus.OFF:
                return <img className="relay-status-img" src={oven_off} />;
            case RelayStatus.ON:
                return <img className="relay-status-img" src={oven_on} />;
            default:
            case RelayStatus.UNKNOWN:
                return <img className="relay-status-img" src={oven_undef} />;
        }
    };

    return (
        <>
            <div className="relay-status">
                Status: {relayStatusMapper(relayStatus)}
            </div>
        </>
    );
}
