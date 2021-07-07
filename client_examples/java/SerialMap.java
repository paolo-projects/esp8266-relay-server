package client_examples.java;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.EOFException;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.LinkedHashMap;
import java.util.Map;

public class SerialMap {
    private static final byte KEY_TYPE = '\u0010';
    private static final byte VALUE_TYPE = '\u0011';

    private Map<String, String> data = new LinkedHashMap<>();

    public SerialMap() {
    }

    public SerialMap(InputStream input) throws IOException {
        DataInputStream dataIs = new DataInputStream(input);

        try {
            while (true) {
                byte b = dataIs.readByte();

                if (b != KEY_TYPE) {
                    break;
                }

                int l = dataIs.readUnsignedByte();
                byte[] buffer = new byte[l];
                dataIs.readFully(buffer);

                String key = new String(buffer, StandardCharsets.UTF_8);

                b = dataIs.readByte();
                if (b != VALUE_TYPE) {
                    break;
                }

                l = dataIs.readUnsignedByte();
                buffer = new byte[l];
                dataIs.readFully(buffer);
                String value = new String(buffer, StandardCharsets.UTF_8);

                data.put(key, value);
            }
        } catch (EOFException e) {
        }
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

        ByteBuffer out = ByteBuffer.allocate(size);

        for (Map.Entry<String, String> entry : data.entrySet()) {
            out.put(KEY_TYPE).put((byte) entry.getKey().length()).put(entry.getKey().getBytes(StandardCharsets.UTF_8))
                    .put(VALUE_TYPE).put((byte) entry.getValue().length())
                    .put(entry.getValue().getBytes(StandardCharsets.UTF_8));
        }

        return out.array();
    }

    public void write(OutputStream output) throws IOException {
        write(output, false);
    }

    public void write(OutputStream output, boolean flush) throws IOException {
        DataOutputStream dOs = new DataOutputStream(output);

        for (Map.Entry<String, String> entry : data.entrySet()) {
            dOs.write(KEY_TYPE);
            dOs.write((byte) entry.getKey().length());
            dOs.write(entry.getKey().getBytes(StandardCharsets.UTF_8));
            dOs.write(VALUE_TYPE);
            dOs.write((byte) entry.getValue().length());
            dOs.write(entry.getValue().getBytes(StandardCharsets.UTF_8));
        }

        dOs.write('\0');

        if (flush) {
            output.flush();
        }
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("[ ");
        for (Map.Entry<String, String> e : data.entrySet()) {
            sb.append("\"").append(e.getKey()).append("\"").append(" => ").append("\"").append(e.getValue())
                    .append("\", ");
        }
        sb.append("]");
        return sb.toString();
    }
}