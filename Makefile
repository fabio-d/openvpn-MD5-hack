.PHONY: all install uninstall clean

all: openvpn_md5_enabler.so

install: all
	@# this is ugly! patch file belonging to NetworkManager-openvpn
	perl -pi -e 's:^program=/usr/libexec/nm-openvpn-service$$:program=/usr/libexec/nm-openvpn-service-with-md5:' /usr/lib/NetworkManager/VPN/nm-openvpn-service.name

	@# install helper files
	install -t /usr/libexec nm-openvpn-service-with-md5 openvpn_md5_enabler.so

uninstall:
	perl -pi -e 's:^(.*)-with-md5$$:\1:' /usr/lib/NetworkManager/VPN/nm-openvpn-service.name
	$(RM) /usr/libexec/nm-openvpn-service-with-md5 /usr/libexec/openvpn_md5_enabler.so

openvpn_md5_enabler.so: openvpn_md5_enabler.c
	$(CC) -shared -fPIC -o $@ $< -ldl -Wall

clean:
	$(RM) openvpn_md5_enabler.so
