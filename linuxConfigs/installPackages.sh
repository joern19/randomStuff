#!/bin/bash

# Syncronize everything
# Expecting an ready to use system. Only installing and configuring tools

if [ "$EUID" -ne 0 ]; then
  echo "Using $USER as an already configured user. Skipping user Setup..."
else
  echo "Root detected. Please setup a user to configure the applications for."
  echo "exiting"
  exit 1
fi

function printTitle() {
  echo ""
  echo ""
  echo $(tput bold)$1$(tput sgr0)
}


printTitle "Updateing system"
sudo pacman -Syu

printTitle "Installing packages"

essentail="wget git unzip python sudo"
niceToHaveCli="tree yay ufw snapd python-pip youtube-dl"
desktop="i3-gaps i3blocks feh dmenu ttf-font-awesome lightdm-webkit2-greeter playerctl xclip"
guiApps="gparted firefox kdeconnect ksystemlog ksysguard pavucontrol rxvt-unicode xfce4-terminal"
sudo pacman -S --noconfirm --needed $essentail $niceToHaveCli $desktop $guiApps

#echo "idk"
#sudo pacman -S spotifyd xsel

printTitle "Installing snap apps"
sudo systemctl enable --now snapd 
sudo snap install code postman 

printTitle "Enabeling firewall"
sudo ufw enable

if [[ -f "~/.fonts/System\ San\ Francisco\ Display\ Bold.ttf" ]]; then # The font actually consists of multiple files. But checking only one should be enogh to check if it is already installed
  printTitle "Installing San Francisco Font"
  wget https://github.com/supermarin/YosemiteSanFranciscoFont/archive/master.zip -O tmpFile.zip
  unzip tmpFile.zip
  rm tmpFile.zip
  mv YosemiteSanFranciscoFont-master/*.ttf ~/.fonts/
  rm -r YosemiteSanFranciscoFont-master/
fi
