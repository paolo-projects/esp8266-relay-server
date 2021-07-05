package client_examples.java;

import java.io.IOException;
import java.net.Socket;
import java.security.NoSuchAlgorithmException;
import java.util.concurrent.TimeoutException;

import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLSocketFactory;

public class Main {

    public static void main(String[] args) {
        SerialMap map = new SerialMap();
        map.put("action", "getstate");

        SerialMap auth = new SerialMap();
        auth.put("username", "carlo");
        auth.put("password", "magno");

        byte[] authData = auth.serialize();
        byte[] data = map.serialize();

        try {
            SSLContext context = SSLContext.getDefault();
            /* add key */
            SSLSocketFactory factory = context.getSocketFactory();

            try (Socket socket = factory.createSocket(/* ... */)) {
                /* authentication */
                socket.getOutputStream().write(authData);
                socket.getOutputStream().flush();

                /* ... read response ... */
                byte[] payload = socket.getInputStream().readAllBytes();
                SerialMap response = new SerialMap(payload);

                if ("ok".equals(response.get("result"))) {
                    socket.getOutputStream().write(data);
                    socket.getOutputStream().flush();

                    payload = socket.getInputStream().readAllBytes();
                    response = new SerialMap(payload);

                    /* do something with the response */
                }

            } catch (IOException e) {

            }
        } catch (NoSuchAlgorithmException e) {

        }
    }

}
