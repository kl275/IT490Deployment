echo "Extracting archive"
# Getting current package Number
package="$1"
mkdir $HOME/packaging/temp/
mkdir $HOME/Desktop/

tar -xzf $HOME/packaging/$package -C $HOME/packaging/temp/

echo "Unpacking MySQL files"
echo "Enter the MySQL password: "
	mysql -u root -p login < $HOME/packaging/temp/mySqlData/backup_sql.sql

	echo "BAckend files being done"
	cp -a -v $HOME/packaging/temp/it490_backend_final/* $HOME/Desktop/kevvBackendServer/
echo "Adjusting RabbitMQ .ini..."
	sed -i '2s/.*/BROKER_HOST = 192.168.1.32/g' $HOME/Desktop/kevvBackendServer/testRabbitMQ.ini
	sed -i '2s/.*/BROKER_HOST = 192.168.1.32/g' $HOME/Desktop/kevvBackendServer/apiRabbitMQ.ini
echo "Package installed successfully for backend production"

