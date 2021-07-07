package client_examples.java;

import java.io.IOException;
import java.net.Socket;
import java.security.KeyManagementException;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.cert.CertificateException;
import javax.net.ssl.SSLSocketFactory;

public class Main {

    public static void main(String[] args) {
        SerialMap action = new SerialMap();

        if (args.length > 0) {
            switch (args[0]) {
                default:
                case "--get":
                    action.put("action", "getstate");
                    break;
                case "--set":
                    action.put("action", "setstate");
                    if (args.length > 1) {
                        action.put("state", args[1].equals("on") ? "on" : "off");
                    } else {
                        action.put("state", "off");
                    }
                    break;
                case "--reset":
                    action.put("action", "disconnect");
                    break;
            }
        } else {
            action.put("action", "getstate");
        }

        SerialMap auth = new SerialMap();
        auth.put("username", Config.USERNAME);
        auth.put("password", Config.PASSWORD);

        try {
            SSLSocketFactory factory = SSLCustomContext.get().getSocketFactory();

            try (Socket socket = factory.createSocket(Config.HOSTNAME, Config.PORT)) {
                /* authentication */
                auth.write(socket.getOutputStream(), true);

                System.out.println("Sent authentication");

                /* ... read response ... */
                SerialMap response = new SerialMap(socket.getInputStream());

                if ("ok".equals(response.get("result"))) {
                    /* authenticated */
                    System.out.println("Authentication OK. Sending action");

                    action.write(socket.getOutputStream(), true);

                    SerialMap payload = new SerialMap(socket.getInputStream());

                    if (action.get("action").equals("getstate") && payload.has("state")) {
                        System.out.println("State: " + payload.get("state").toUpperCase());
                    } else {
                        System.out.println("Action: " + ("ok".equals(payload.get("result")) ? "SUCCESS" : "FAILURE"));
                    }
                }

            } catch (IOException e) {
                e.printStackTrace();
            }
        } catch (NoSuchAlgorithmException | CertificateException | KeyStoreException | IOException
                | KeyManagementException e) {
            e.printStackTrace();
        }
    }

}
