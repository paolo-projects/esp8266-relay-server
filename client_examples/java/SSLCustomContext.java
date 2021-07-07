package client_examples.java;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.security.KeyManagementException;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;

import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManagerFactory;

public class SSLCustomContext {
    private static SSLContext context;

    public static SSLContext get() throws CertificateException, IOException, NoSuchAlgorithmException,
            KeyStoreException, KeyManagementException {
        if (context == null) {
            CertificateFactory cf = CertificateFactory.getInstance("X.509");
            X509Certificate cert = (X509Certificate) cf
                    .generateCertificate(new ByteArrayInputStream(Config.CERTIFICATE.getBytes(StandardCharsets.UTF_8)));
            TrustManagerFactory tmf = TrustManagerFactory.getInstance(TrustManagerFactory.getDefaultAlgorithm());
            KeyStore ks = KeyStore.getInstance(KeyStore.getDefaultType());
            ks.load(null);
            ks.setCertificateEntry("caCert", cert);
            tmf.init(ks);

            context = SSLContext.getInstance("TLS");
            context.init(null, tmf.getTrustManagers(), null);
        }
        return context;
    }

}
