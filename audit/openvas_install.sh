#!/bin/sh

OPENVAS_DIR=/tmp/openvas


openvas_prework(){
	sudo apt-get install libglib3.0-cil-dev libpcap-dev libgnutls-dev build-essential bison flex cmake pkg-config libgpgme11 libgpgme11-dev doxygen libuuid1 uuid-dev libsql-translator-perl xmltoman sqlite3 libxml2-dev libxslt1.1 libxslt1-dev xsltproc libmicrohttpd-dev

	cd $OPENVAS_DIR

	wget http://wald.intevation.org/frs/download.php/1638/openvas-libraries-7.0.1.tar.gz
	wget http://wald.intevation.org/frs/download.php/1640/openvas-scanner-4.0.1.tar.gz
	wget http://wald.intevation.org/frs/download.php/1637/openvas-manager-5.0.0.tar.gz
	wget http://wald.intevation.org/frs/download.php/1639/greenbone-security-assistant-5.0.0.tar.gz
	wget http://wald.intevation.org/frs/download.php/1633/openvas-cli-1.3.0.tar.gz
	
	mkdir src
	cd src
	tar xf ../greenbone-security-assistant-5.0.0.tar.gz
	tar xf ../openvas-libraries-7.0.1.tar.gz 
	tar xf ../openvas-scanner-4.0.1.tar.gz 
	tar xf ../openvas-cli-1.3.0.tar.gz 
	tar xf ../openvas-manager-5.0.0.tar.gz
}

openvas_build() {
	mkdir build
	cd build
	cmake ..
	make
	sudo make install
	cd ..
}

openvas_compile() {
	cd openvas-libraries-7.0.1
	openvas_build

	for i in $DIR
	do
		if [ $i != "openvas-libraries-7.0.1" ]; then
			cd $i
			openvas_build
			cd ..
		fi
	done
}

mkdir $OPENVAS_DIR
echo "[+] Creating directory $OPENVAS_DIR"
openvas_prework
echo "[+] Preparation done"

DIR=`ls $OPENVAS_DIR/src`
openvas_compile $DIR
echo "[+] Compile completed"

echo "[+] Creating certs and starting OpenVAS..."
sudo openvas-mkcert -f
sudo ldconfig
sudo openvassd

sudo openvas-nvt-sync
sudo openvas-scapdata-sync
sudo openvas-certdata-sync
echo "[+] NVT/SCAP/CERT data has been updated"

sudo openvasmd --create-user=admin --role=Admin
sudo openvas-mkcert-client -n -i
sudo openvasmd --rebuild --progress
echo "[+] Done"

sudo openvasmd
sudo gsad
