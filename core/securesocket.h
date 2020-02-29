#ifndef SECURESOCKET_H
#define SECURESOCKET_H

#include <QObject>
#include <QString>
#include <QDebug>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>

#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/x509.h>
#include <openssl/buffer.h>
#include <openssl/x509v3.h>
#include <openssl/opensslconf.h>

class SecureSocket : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(type name READ name WRITE setName NOTIFY nameChanged)
public:
    explicit SecureSocket(QObject *parent = nullptr);
    ~SecureSocket();
    Q_INVOKABLE void sendInviteEmailAndKey(QString,QString);
    Q_INVOKABLE void test(QString);
private:
    bool setup();
    char buff[100] = {};
    SSL_CTX* ctx = NULL;
    BIO *web = NULL, *out = NULL;
    SSL *ssl = NULL;
    const char* const PREFERRED_CIPHERS = "HIGH:!aNULL:!kRSA:!SRP:!PSK:!CAMELLIA:!RC4:!MD5:!DSS";
    static void init_openssl_library(void);
    //static void print_cn_name(const char* label, X509_NAME* const name);
    //static void print_san_name(const char* label, X509* const cert);
    static int verify_callback(int preverify, X509_STORE_CTX* x509_ctx);
    static void print_error_string(unsigned long err, const char* const label);
signals:
};

#endif // SECURESOCKET_H
