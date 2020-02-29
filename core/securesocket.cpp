#include "securesocket.h"
#include "config.cpp"

SecureSocket::SecureSocket(QObject *parent):QObject(parent)
{

}

SecureSocket::~SecureSocket()
{
    if(out)
        BIO_free(out);

    if(web != NULL)
        BIO_free_all(web);

    if(NULL != ctx)
        SSL_CTX_free(ctx);
}
void SecureSocket::send(QString msg)
{
    BIO_puts(web, msg.toLocal8Bit().data());
    BIO_puts(out, "ResponseFromServer: \n");

    int len = 0;
    do {
        /* https://www.openssl.org/docs/crypto/BIO_read.html */
        len = BIO_read(web, buff, sizeof(buff));

        if(len > 0)
            BIO_write(out, buff, len);

        /* BIO_should_retry returns TRUE unless there's an  */
        /* error. We expect an error when the server        */
        /* provides the response and closes the connection. */

    } while (len > 0 || BIO_should_retry(web));
}
bool SecureSocket::setup()
{
    long res = 1;
    unsigned long ssl_err = 0;



    // do {

    /* Internal function that wraps the OpenSSL init's   */
    /* Cannot fail because no OpenSSL function fails ??? */
    init_openssl_library();

    /* https://www.openssl.org/docs/ssl/SSL_CTX_new.html */
    const SSL_METHOD* method = SSLv23_method();
    ssl_err = ERR_get_error();

    if(!(NULL != method))
    {
        print_error_string(ssl_err, "SSLv23_method");
        return false;
    }

    /* http://www.openssl.org/docs/ssl/ctx_new.html */
    ctx = SSL_CTX_new(method);
    /* ctx = SSL_CTX_new(TLSv1_method()); */
    ssl_err = ERR_get_error();

    if(!(ctx != NULL))
    {
        print_error_string(ssl_err, "SSL_CTX_new");
        return false;
    }

    /* https://www.openssl.org/docs/ssl/ctx_set_verify.html */
    //SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, SecureSocket::verify_callback);
    /* Cannot fail ??? */

    /* https://www.openssl.org/docs/ssl/ctx_set_verify.html */
    //SSL_CTX_set_verify_depth(ctx, 5);
    /* Cannot fail ??? */

    /* Remove the most egregious. Because SSLv2 and SSLv3 have been      */
    /* removed, a TLSv1.0 handshake is used. The client accepts TLSv1.0  */
    /* and above. An added benefit of TLS 1.0 and above are TLS          */
    /* extensions like Server Name Indicatior (SNI).                     */
    //const long flags = SSL_OP_ALL | SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_COMPRESSION;
    //long old_opts = SSL_CTX_set_options(ctx, flags);

    /* http://www.openssl.org/docs/ssl/SSL_CTX_load_verify_locations.html */
    // res = SSL_CTX_load_verify_locations(ctx, "fullchain.pem", NULL);
    // ssl_err = ERR_get_error();

    //if(!(1 == res))
    //{
    /* Non-fatal, but something else will probably break later */
    // print_error_string(ssl_err, "SSL_CTX_load_verify_locations");
    /* break; */
    //}

    /* https://www.openssl.org/docs/crypto/BIO_f_ssl.html */
    web = BIO_new_ssl_connect(ctx);
    ssl_err = ERR_get_error();

    if(!(web != NULL))
    {
        print_error_string(ssl_err, "BIO_new_ssl_connect");
        return false;
    }

    /* https://www.openssl.org/docs/crypto/BIO_s_connect.html */
    QString connectionAddress = socketHostName+":"+socketPort;
    res = BIO_set_conn_hostname(web, connectionAddress.toLocal8Bit().data());
    ssl_err = ERR_get_error();

    if(!(1 == res))
    {
        print_error_string(ssl_err, "BIO_set_conn_hostname");
        return false;
    }

    /* https://www.openssl.org/docs/crypto/BIO_f_ssl.html */
    /* This copies an internal pointer. No need to free.  */
    BIO_get_ssl(web, &ssl);
    ssl_err = ERR_get_error();

    if(!(ssl != NULL))
    {
        print_error_string(ssl_err, "BIO_get_ssl");
        return false;
    }

    /* https://www.openssl.org/docs/ssl/ssl.html#DEALING_WITH_PROTOCOL_CONTEXTS */
    /* https://www.openssl.org/docs/ssl/SSL_CTX_set_cipher_list.html            */
    res = SSL_set_cipher_list(ssl, PREFERRED_CIPHERS);
    ssl_err = ERR_get_error();

    if(!(1 == res))
    {
        print_error_string(ssl_err, "SSL_set_cipher_list");
        return false;
    }

    /* No documentation. See the source code for tls.h and s_client.c */
    res = SSL_set_tlsext_host_name(ssl, socketHostName.toLocal8Bit().data());
    ssl_err = ERR_get_error();

    if(!(1 == res))
    {
        /* Non-fatal, but who knows what cert might be served by an SNI server  */
        /* (We know its the default site's cert in Apache and IIS...)           */
        print_error_string(ssl_err, "SSL_set_tlsext_host_name");
        return false;
    }

    /* https://www.openssl.org/docs/crypto/BIO_s_file.html */
    out = BIO_new_fp(stdout, BIO_NOCLOSE);
    ssl_err = ERR_get_error();

    if(!(NULL != out))
    {
        print_error_string(ssl_err, "BIO_new_fp");
        return false;
    }

    /* https://www.openssl.org/docs/crypto/BIO_s_connect.html */
    res = BIO_do_connect(web);
    ssl_err = ERR_get_error();

    if(!(1 == res))
    {
        print_error_string(ssl_err, "BIO_do_connect");
        return false;
    }

    /* https://www.openssl.org/docs/crypto/BIO_f_ssl.html */
    res = BIO_do_handshake(web);
    ssl_err = ERR_get_error();

    if(!(1 == res))
    {
        print_error_string(ssl_err, "BIO_do_handshake");
        return false;
    }

    /**************************************************************************************/
    /**************************************************************************************/
    /* You need to perform X509 verification here. There are two documents that provide   */
    /*   guidance on the gyrations. First is RFC 5280, and second is RFC 6125. Two other  */
    /*   documents of interest are:                                                       */
    /*     Baseline Certificate Requirements:                                             */
    /*       https://www.cabforum.org/Baseline_Requirements_V1_1_6.pdf                    */
    /*     Extended Validation Certificate Requirements:                                  */
    /*       https://www.cabforum.org/Guidelines_v1_4_3.pdf                               */
    /*                                                                                    */
    /* Here are the minimum steps you should perform:                                     */
    /*   1. Call SSL_get_peer_certificate and ensure the certificate is non-NULL. It      */
    /*      should never be NULL because Anonymous Diffie-Hellman (ADH) is not allowed.   */
    /*   2. Call SSL_get_verify_result and ensure it returns X509_V_OK. This return value */
    /*      depends upon your verify_callback if you provided one. If not, the library    */
    /*      default validation is fine (and you should not need to change it).            */
    /*   3. Verify either the CN or the SAN matches the host you attempted to connect to. */
    /*      Note Well (N.B.): OpenSSL prior to version 1.1.0 did *NOT* perform hostname   */
    /*      verification. If you are using OpenSSL 0.9.8 or 1.0.1, then you will need     */
    /*      to perform hostname verification yourself. The code to get you started on     */
    /*      hostname verification is provided in print_cn_name and print_san_name. Be     */
    /*      sure you are sensitive to ccTLDs (don't navively transform the hostname       */
    /*      string). http://publicsuffix.org/ might be helpful.                           */
    /*                                                                                    */
    /* If all three checks succeed, then you have a chance at a secure connection. But    */
    /*   its only a chance, and you should either pin your certificates (to remove DNS,   */
    /*   CA, and Web Hosters from the equation) or implement a Trust-On-First-Use (TOFU)  */
    /*   scheme like Perspectives or SSH. But before you TOFU, you still have to make     */
    /*   the customary checks to ensure the certifcate passes the sniff test.             */
    /*                                                                                    */
    /* Happy certificate validation hunting!                                              */
    /**************************************************************************************/
    /**************************************************************************************/


    /* Step 1: verify a server certifcate was presented during negotiation */
    /* https://www.openssl.org/docs/ssl/SSL_get_peer_certificate.html          */
    X509* cert = SSL_get_peer_certificate(ssl);
    if(cert) { X509_free(cert); } /* Free immediately */

    if(NULL == cert)
    {
        /* Hack a code for print_error_string. */
        print_error_string(X509_V_ERR_APPLICATION_VERIFICATION, "SSL_get_peer_certificate");
        return false;
        // break; /* failed */
    }

    /* Step 2: verify the result of chain verifcation             */
    /* http://www.openssl.org/docs/ssl/SSL_get_verify_result.html */
    /* Error codes: http://www.openssl.org/docs/apps/verify.html  */
    // res = SSL_get_verify_result(ssl);

    //if(!(X509_V_OK == res))
    //{
    /* Hack a code into print_error_string. */
    // print_error_string((unsigned long)res, "SSL_get_verify_results");
    //printf("Step 2 error");
    //break; /* failed */
    //}

    /* Step 3: hostname verifcation.   */
    /* An exercise left to the reader. */

    /**************************************************************************************/
    /**************************************************************************************/
    /* Now, we can finally start reading and writing to the BIO...                        */
    /**************************************************************************************/
    /**************************************************************************************/


    //BIO_puts(out, "\nFetching: \n\n");

    //int len = 0;
    //do {
    /* https://www.openssl.org/docs/crypto/BIO_read.html */
    // len = BIO_read(web, buff, sizeof(buff));

    //if(len > 0)
    //   BIO_write(out, buff, len);

    /* BIO_should_retry returns TRUE unless there's an  */
    /* error. We expect an error when the server        */
    /* provides the response and closes the connection. */

    //} while (len > 0 || BIO_should_retry(web));


    //} while (0);

    /*if(out)
        BIO_free(out);

    if(web != NULL)
        BIO_free_all(web);

    if(NULL != ctx)
        SSL_CTX_free(ctx);*/
    return true;
}
int SecureSocket::verify_callback(int preverify, X509_STORE_CTX* x509_ctx)
{
    /* For error codes, see http://www.openssl.org/docs/apps/verify.html  */

    //int depth = X509_STORE_CTX_get_error_depth(x509_ctx);
    int err = X509_STORE_CTX_get_error(x509_ctx);

    //X509* cert = X509_STORE_CTX_get_current_cert(x509_ctx);
    //X509_NAME* iname = cert ? X509_get_issuer_name(cert) : NULL;
    //X509_NAME* sname = cert ? X509_get_subject_name(cert) : NULL;

    //fprintf(stdout, "verify_callback (depth=%d)(preverify=%d)\n", depth, preverify);

    /* Issuer is the authority we trust that warrants nothing useful */
    //print_cn_name("Issuer (cn)", iname);

    /* Subject is who the certificate is issued to by the authority  */
    //print_cn_name("Subject (cn)", sname);


    if(preverify == 0)
    {
        if(err == X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY)
            fprintf(stdout, "  Error = X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY\n");
        else if(err == X509_V_ERR_CERT_UNTRUSTED)
            fprintf(stdout, "  Error = X509_V_ERR_CERT_UNTRUSTED\n");
        else if(err == X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN)
            fprintf(stdout, "  Error = X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN\n");
        else if(err == X509_V_ERR_CERT_NOT_YET_VALID)
            fprintf(stdout, "  Error = X509_V_ERR_CERT_NOT_YET_VALID\n");
        else if(err == X509_V_ERR_CERT_HAS_EXPIRED)
            fprintf(stdout, "  Error = X509_V_ERR_CERT_HAS_EXPIRED\n");
        else if(err == X509_V_OK)
            fprintf(stdout, "  Error = X509_V_OK\n");
        else
            fprintf(stdout, "  Error = %d\n", err);
    }

#if !defined(NDEBUG)
    return 1;
#else
    return preverify;
#endif
}

void SecureSocket::print_error_string(unsigned long err, const char* const label)
{
    const char* const str = ERR_reason_error_string(err);
    if(str)
        fprintf(stderr, "%s\n", str);
    else
        fprintf(stderr, "%s failed: %lu (0x%lx)\n", label, err, err);
}
void SecureSocket::init_openssl_library(void)
{
    /* https://www.openssl.org/docs/ssl/SSL_library_init.html */
    (void)SSL_library_init();
    /* Cannot fail (always returns success) ??? */

    /* https://www.openssl.org/docs/crypto/ERR_load_crypto_strings.html */
    SSL_load_error_strings();
    /* Cannot fail ??? */

    /* SSL_load_error_strings loads both libssl and libcrypto strings */
    /* ERR_load_crypto_strings(); */
    /* Cannot fail ??? */

    /* OpenSSL_config may or may not be called internally, based on */
    /*  some #defines and internal gyrations. Explicitly call it    */
    /*  *IF* you need something from openssl.cfg, such as a         */
    /*  dynamically configured ENGINE.                              */
    OPENSSL_config(NULL);
    /* Cannot fail ??? */

    /* Include <openssl/opensslconf.h> to get this define     */
    //#if defined (OPENSSL_THREADS)
    /* TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO */
    /* https://www.openssl.org/docs/crypto/threads.html */
    //fprintf(stdout, "Warning: thread locking is not implemented\n");
    //#endif
}
