import React, { FormEvent, useState } from 'react';
import { ClipLoader } from 'react-spinners';
import { CHANNEL_SET_WIFI_OPTIONS } from '../../../channels';
import useIpc from '../../Hooks/use-ipc';
import './device-configuration.scss';

export default function DeviceConfiguration() {
    const [form, setForm] = useState({});
    const [submitting, setSubmitting] = useState(false);
    const [error, setError] = useState('');
    const [success, setSuccess] = useState('');

    const ipc = useIpc();

    const setWifiSubmit = (event: FormEvent<HTMLFormElement>) => {
        event.preventDefault();
        setSubmitting(true);
        setError('');
        setSuccess('');
        ipc.invoke(CHANNEL_SET_WIFI_OPTIONS, form)
            .then(() => {
                setSubmitting(false);
                setSuccess('Configuration has been sent successfully');
            })
            .catch((err) => {
                setSubmitting(false);
                setError(typeof err === 'string' ? err : err.message);
            });
    };

    return (
        <>
            <button className="btn btn-primary" disabled={submitting}>
                Set to AP
            </button>
            <hr />
            <form onSubmit={(event) => setWifiSubmit(event)}>
                <div className="input-group mb-2">
                    <input
                        type="text"
                        className="form-control"
                        placeholder="SSID"
                        onChange={(e) =>
                            setForm({ ...form, ssid: e.target.value })
                        }
                        required
                        disabled={submitting}
                    />
                    <span className="input-group-text">:</span>
                    <input
                        type="text"
                        className="form-control"
                        placeholder="Password"
                        onChange={(e) =>
                            setForm({ ...form, password: e.target.value })
                        }
                        required
                        disabled={submitting}
                    />
                </div>
                <div>
                    <button
                        type="submit"
                        className="btn btn-primary"
                        disabled={submitting}
                    >
                        Set device Wifi
                    </button>
                </div>
                <div>
                    <small className="text-muted">
                        You need a WiFi card to connect to the device
                    </small>
                </div>
                <div>
                    <small className="text-danger">{error}</small>
                </div>
                <div>
                    <small className="text-success">{success}</small>
                </div>
                <div className="loading-spinner">
                    <ClipLoader color="#ffffff" loading={submitting} />
                </div>
            </form>
        </>
    );
}
