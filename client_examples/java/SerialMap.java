package client_examples.java;

import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.Map;

class SerialMap {
    private static final byte KEY_TYPE = 0x10;
    private static final byte VALUE_TYPE = 0x11;

    private HashMap<String, String> data = new HashMap<>();

    public SerialMap() {
    }

    public SerialMap(byte[] payload) {
        ByteBuffer reader = ByteBuffer.wrap(payload);
        byte[] buffer = new byte[257];

        while (reader.hasRemaining()) {
            if (reader.get() != KEY_TYPE) {
                return;
            }

            int len = reader.get() & 0xFF;
            reader.get(buffer, 0, len);

            String key = new String(buffer, 0, len, StandardCharsets.UTF_8);

            if (reader.get() != VALUE_TYPE) {
                return;
            }

            len = reader.get() & 0xFF;
            reader.get(buffer, 0, len);

            String value = new String(buffer, 0, len, StandardCharsets.UTF_8);

            data.put(key, value);
        }
    }

    public void put(String key, String value) {
        data.put(key, value);
    }

    public boolean has(String key) {
        return data.containsKey(key);
    }

    public String get(String key) {
        return data.get(key);
    }

    public byte[] serialize() {
        int size = 0;
        for (Map.Entry<String, String> entry : data.entrySet()) {
            size += entry.getKey().length() + entry.getValue().length() + 4;
        }

        byte[] result = new byte[size];
        ByteBuffer out = ByteBuffer.wrap(result);

        for (Map.Entry<String, String> entry : data.entrySet()) {
            out.put(KEY_TYPE);
            out.put((byte) entry.getKey().length());
            out.put(entry.getKey().getBytes(StandardCharsets.UTF_8));

            out.put(VALUE_TYPE);
            out.put((byte) entry.getValue().length());
            out.put(entry.getValue().getBytes(StandardCharsets.UTF_8));
        }

        return result;
    }
}