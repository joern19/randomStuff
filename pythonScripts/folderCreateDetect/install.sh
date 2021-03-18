if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

mkdir -p /usr/local/lib/folderDetect
cp ./folderDetect.py /usr/local/lib/folderDetect/
cp ./folderDetectConfig.yaml /usr/local/lib/folderDetect/

cp ./folderDetect.service /etc/systemd/system/folderDetect.service
chmod 664 /etc/systemd/system/folderDetect.service

systemctl enable --now folderDetect.service

pip3 install pyyaml watchdog requests
