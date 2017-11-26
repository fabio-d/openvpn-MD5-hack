# openvpn-MD5
Magic environment variables and .so hook to allow MD5 certificates in OpenVPN and OpenSSL 1.1.0+ in Linux

## The problem

As we all *are supposed to* know, MD5 is cryptographically broken and should not be used because it is insecure. OpenSSL decided to disable it by default long time ago, but it was still possible to set some environment variables to re-enable it until version 1.1.0. With OpenSSL 1.1.0, the program (i.e. OpenVPN) must also call <tt>[SSL_CTX_set_security_level](https://www.openssl.org/docs/manmaster/man3/SSL_CTX_set_security_level.html)(ctx, 0)</tt>, otherwise MD5 certificates will be rejected with the following message:

```VERIFY ERROR: depth=0, error=CA signature digest algorithm too weak```

However, OpenVPN developers have clearly stated that they will not do anything to support MD5. A good summary of the story can be read at [Red Hat Bugzilla – Bug 1443749](https://bugzilla.redhat.com/show_bug.cgi?id=1443749).

## The right solution

**Update your VPN infrastructure** so you **do not use MD5 hashes**. This is the only right thing to do. **However**, sometimes you are just a user who is just trying to connect to an OpenVPN server you have no control on. In this case, please accept that your VPN connection will be insecure and read on.

## The workaround (if you run OpenVPN on the command line)

Run `make` (**not** `make install`) and set the following three environment variables, then run `openvpn` as usual:

```bash
sudo su # <-- become root
export OPENSSL_ENABLE_MD5_VERIFY=1
export NSS_HASH_ALG_SUPPORT=+MD5
export LD_PRELOAD="/path/to/openvpn_md5_enabler.so"
openvpn example.conf
```

The `openvpn_md5_enabler.so` library will hook OpenSSL's context creation function and call <tt>[SSL_CTX_set_security_level](https://www.openssl.org/docs/manmaster/man3/SSL_CTX_set_security_level.html)(ctx, 0)</tt> in every OpenSSL context created by OpenVPN.

**Note**: Unless you edit `/etc/environment`, environment variables' values are not persistent. Given the invasivity of the `LD_PRELOAD` hook, it is advisable to keep them non persistent and set them only when you are about to run `openvpn`.

## The workaround (if you run OpenVPN through NetworkManager)

**In short**: just run `make && sudo make install` and connect normally.

`make install` will patch `/usr/lib/NetworkManager/VPN/nm-openvpn-service.name` as follows:

<blockquote>
<pre># original contents
[VPN Connection]
name=openvpn
service=org.freedesktop.NetworkManager.openvpn
<b>program=/usr/libexec/nm-openvpn-service</b>
supports-multiple-connections=true
</pre>
<pre># patched contents
[VPN Connection]
name=openvpn
service=org.freedesktop.NetworkManager.openvpn
<b>program=/usr/libexec/nm-openvpn-service-with-md5</b>
supports-multiple-connections=true
</pre>
</blockquote>

**Note #1**: Your system updater will probably restore the original contents whenever a NetworkManager update is installed. If that happens, just run `sudo make install` to patch it again.

**Note #2**: This method will probably work on any NetworkManager-based Linux distro. However, it has only been tested on Fedora 26.

# License

See [LICENSE](LICENSE) file in this repository.
