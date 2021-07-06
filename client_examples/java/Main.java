package client_examples.java;

import java.io.IOException;
import java.net.Socket;
import java.security.NoSuchAlgorithmException;

import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLSocketFactory;

public class Main {

    public static void main(String[] args) {
        SerialMap action = new SerialMap();
        action.put("action", "getstate");

        SerialMap auth = new SerialMap();
        auth.put("username", "carlo");
        auth.put("password", "magno");

        try {
            SSLContext context = SSLContext.getDefault();
            /* add key */
            SSLSocketFactory factory = context.getSocketFactory();

            try (Socket socket = factory.createSocket("127.0.0.1", 1234)) {
                /* authentication */
                auth.write(socket.getOutputStream(), true);

                /* ... read response ... */
                SerialMap response = new SerialMap(socket.getInputStream());

                if ("ok".equals(response.get("result"))) {
                    action.write(socket.getOutputStream(), true);

                    SerialMap payload = new SerialMap(socket.getInputStream());

                    /* do something with the response */
                }

            } catch (IOException e) {
                e.printStackTrace();
            }
        } catch (NoSuchAlgorithmException e) {

        }
    }

}
