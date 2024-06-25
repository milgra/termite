rm -rf termite.app
mkdir termite.app
mkdir -p termite.app/bin
mkdir -p termite.app/lib
ldd build/termite | awk '{print $3}' | xargs -I {} cp {} termite.app/lib/
rm termite.app/lib/libc.so.*
rm termite.app/lib/libm.so.*
cp -R build/termite termite.app/bin/
echo 'PTH=$(dirname $(readlink -f $0))' > termite.app/termite
echo "LD_LIBRARY_PATH=\$PTH/lib \$PTH/bin/termite" >> termite.app/termite
chmod +x termite.app/termite
tar czf termite.app.tar.gz termite.app
