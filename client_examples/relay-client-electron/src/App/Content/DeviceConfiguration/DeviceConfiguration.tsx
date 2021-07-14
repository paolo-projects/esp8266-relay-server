import React, { FormEvent, useState } from 'react';
import { ClipLoader } from 'react-spinners';
import {
    CHANNEL_RESET_DEVICE,
    CHANNEL_SET_WIFI_OPTIONS,
} from '../../../channels';
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

    const setToAp = () => {
        setSubmitting(true);
        ipc.invoke(CHANNEL_RESET_DEVICE).finally(() => setSubmitting(false));
    };

    return (
        <div className="configuration-container">
            <button
                className="btn btn-primary"
                disabled={submitting}
                onClick={() => setToAp()}
            >
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
                <div className="loading-spinner">
                    <ClipLoader color="#ffffff" loading={submitting} />
                </div>
                <div>
                    <small className="text-muted">
                        <h6 className="mt-2">Instructions</h6>
                        <p>
                            First, you need to make sure your device is in AP
                            mode, then connect to your device WiFi through your
                            OS configuration panel. If you set the WiFi hidden
                            in the firmware configuration, you have to enter the
                            SSID and password manually.
                        </p>
                        <p>
                            After this step, you can type the SSID and password
                            into the field up here and send the new
                            configuration.
                        </p>
                    </small>
                </div>
                <div>
                    <small className="text-danger">{error}</small>
                </div>
                <div>
                    <small className="text-success">{success}</small>
                </div>
            </form>
        </div>
    );
}
