#!/bin/sh

echo "====================[ Auto-Installation for GNU/Linux ]===================="
echo "###########################################################################"

echo " This is free software, and you are welcome to redistribute it
 under the terms of the GNU General Public License.  See LICENSE file
  for details about using this software."  
	  echo "\nSee the detail: http://hackertarget.com/install-openvas-7-ubuntu/."
	  echo "###########################################################################"

OPENVAS_DIR=/tmp/openvas

LIB_VERSION=7.0.6
SCANNER_VERSION=4.0.5
MANAGER_VERSION=5.0.7
GSA_VERSION=5.0.4
CLI_VERSION=1.3.1

openvas_prework(){
	sudo apt-get install libglib3.0-cil-dev libpcap-dev libgnutls-dev build-essential bison flex cmake pkg-config libgpgme11 libgpgme11-dev doxygen libuuid1 uuid-dev libsql-translator-perl xmltoman sqlite3 libsqlite3-dev libxml2-dev libxslt1.1 libxslt1-dev xsltproc libmicrohttpd-dev

	cd $OPENVAS_DIR

	wget http://wald.intevation.org/frs/download.php/1833/openvas-libraries-$LIB_VERSION.tar.gz
	wget http://wald.intevation.org/frs/download.php/1844/openvas-scanner-$SCANNER_VERSION.tar.gz
	wget http://wald.intevation.org/frs/download.php/1849/openvas-manager-$MANAGER_VERSION.tar.gz
	wget http://wald.intevation.org/frs/download.php/1799/greenbone-security-assistant-$GSA_VERSION.tar.gz	
	wget http://wald.intevation.org/frs/download.php/1803/openvas-cli-$CLI_VERSION.tar.gz	
	
	mkdir src
	cd src
	tar xf ../greenbone-security-assistant-$GSA_VERSION.tar.gz
	tar xf ../openvas-libraries-$LIB_VERSION.tar.gz 
	tar xf ../openvas-scanner-$SCANNER_VERSION.tar.gz 
	tar xf ../openvas-cli-$CLI_VERSION.tar.gz 
	tar xf ../openvas-manager-$MANAGER_VERSION.tar.gz
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
	cd openvas-libraries-$LIB_VERSION
	openvas_build
	cd ..

	for i in $DIR
	do
		if [ $i != "openvas-libraries-$LIB_VERSION" ]; then
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
