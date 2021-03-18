if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

systemctl stop folderDetect.service
systemctl disable folderDetect.service
rm -f /etc/systemd/system/folderDetect.service
rm -f /etc/systemd/system/folderDetect.service # and symlinks that might be related
rm -f /usr/lib/systemd/system/folderDetect.service 
rm -f /usr/lib/systemd/system/folderDetect.service # and symlinks that might be related
systemctl daemon-reload
systemctl reset-failed

rm -rf /usr/local/lib/folderDetect

echo "You may want to uninstall the following pip packages: requests, watchdog, pyyaml (dont forget to uninstall them as root)"
