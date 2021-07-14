export const CHANNEL_REQUEST_STATE = 'relayserver_request_state';
export const CHANNEL_SET_STATE = 'relayserver_send_state';
export const CHANNEL_RESET_DEVICE = 'relayserver_reset_device';
export const CHANNEL_SET_WIFI_OPTIONS = 'relayserver_set_wifi_options';
export const CHANNEL_R_STATE_RECEIVED = 'relayserver_renderer_state_received';
export const CHANNEL_GET_CONFIG = 'relayserver_get_configuration';
export const CHANNEL_SAVE_CONFIG = 'relayserver_save_configuration';

export const CHANNELS_ALLOWED = [
    CHANNEL_REQUEST_STATE,
    CHANNEL_SET_STATE,
    CHANNEL_RESET_DEVICE,
    CHANNEL_SET_WIFI_OPTIONS,
    CHANNEL_R_STATE_RECEIVED,
    CHANNEL_GET_CONFIG,
    CHANNEL_SAVE_CONFIG,
];
